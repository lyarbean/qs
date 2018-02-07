#include "defaultengine.h"
#include "defaultengine_p.h"
#include "strategyabstract.h"
#include "gatewayabstract.h"
#include "datatypes.h"
#include "riskmanager.h"
#include <QDebug>
using namespace Qs;

DefaultEnginePrivate::DefaultEnginePrivate(DefaultEngine* q)
  : q(q) {
}
DefaultEnginePrivate::~DefaultEnginePrivate() {
    thread.quit();
}

DefaultEngine::DefaultEngine()
  : d(new DefaultEnginePrivate(this)) {
    qRegisterMetaType<TickInfoPointer>("TickInfoPointer&");
    moveToThread(&d->thread);
    d->thread.start();
}

DefaultEngine::~DefaultEngine() {
    delete d;
}

void DefaultEngine::connectServers() {
    for (auto g : d->gateways)
        g->connectServer();
}

void DefaultEngine::connectServer(const QUuid& uuid) {
    auto g = d->gateways.find(uuid);
    if (g == d->gateways.end()) {
        return;
    }
    g.value()->connectServer();
}

QUuid DefaultEngine::addStrategy(StrategyAbstract* strategy) {
    QUuid uuid = QUuid::createUuid();
    d->strategies[uuid] = strategy;
    return uuid;
}

QUuid DefaultEngine::addGateway(GatewayAbstract* gateway) {
    QUuid uuid = QUuid::createUuid();
    d->gateways[uuid] = gateway;
    connect(gateway, &GatewayAbstract::hasTick, this, &DefaultEngine::onTick);
    connect(gateway, &GatewayAbstract::hasTrade, this, &DefaultEngine::onTrade);
    connect(gateway, &GatewayAbstract::hasOrder, this, &DefaultEngine::onOrder);
    connect(gateway, &GatewayAbstract::hasPoisition, this, &DefaultEngine::onPoisition);
    connect(gateway, &GatewayAbstract::hasAccount, this, &DefaultEngine::onAccount);
    connect(gateway, &GatewayAbstract::hasContract, this, &DefaultEngine::onContract);
    connect(gateway, &GatewayAbstract::hasLog, this, &DefaultEngine::onLog);
    return uuid;
}

RiskManager* DefaultEngine::riskManager() const {
    return nullptr;
}

PositionManager* DefaultEngine::positionManager() const {
    return nullptr;
}

void DefaultEngine::onTick(TickInfoPointer& info) {
    for (auto s : d->strategies) {
        s->onTick(info);
    }
    d->tickInfos[info->ticker()].append(info);
}

void DefaultEngine::onTrade(TradeInfoPointer& info) {
    for (auto s : d->strategies) {
        s->onTrade(info);
    }
    d->tradeInfos[info->ticker()].append(info);
}

void DefaultEngine::onOrder(OrderInfoPointer& info) {
    for (auto s : d->strategies) {
        s->onOrder(info);
    }
    d->orderInfos[info->ticker()].append(info);
}

void DefaultEngine::onPoisition(PoisitionInfoPointer& info) {
}

void DefaultEngine::onAccount(AccountInfoPointer& info) {
}

void DefaultEngine::onContract(ContractInfoPointer& info) {
}

void DefaultEngine::onLog(LogInfoPointer& info) {
}

void DefaultEngine::sendOrder(OrderRequestPointer& request, const QUuid& gateway) {

    auto g = d->gateways.find(gateway);
    if (g == d->gateways.end()) {
        // TODO
        // request->status = NoGatway;
        d->abandonedOrderRequests[gateway].append(request);
        return;
    }
    if (!d->riskManager->check(request, gateway)) {
        // TODO
        // request->status = HasRisk;
        d->orderRequests[gateway].append(request);
        return;
    }
    g.value()->sendOrder(request);
    d->orderRequests[gateway].append(request);
}

void DefaultEngine::sendOrder(OrderRequestPointer& request) {
    for (auto g : d->gateways) {
        g->sendOrder(request);
    }
    // TODO
    // d->orderRequests[gateway].append(request);
}

void DefaultEngine::cancelOrder(CancelOrderRequestPointer& request, const QUuid& gateway) {
    // d->riskManager->check()
    auto g = d->gateways.find(gateway);
    if (g == d->gateways.end()) {
        // TODO
        // request->status = NoGatway;
        d->abandonedCancelOrderRequests[gateway].append(request);
        return;
    }
    g.value()->cancelOrder(request);
    d->cancelOrderRequests[gateway].append(request);
}

// TODO use a map to ease onTick
// {strategy + gateway => symbol}
// {symbol + gateway ==> strategy}
// When a gateway emits a TickInfo, it adds gatewayname to TickInfo::gateway
// SubscribeRequest has a uuid to indicate which strategy sends the request

void DefaultEngine::Subscribe(SubscribeRequestPointer& request, const QUuid& gateway) {
    // d->riskManager->check()
    auto g = d->gateways.find(gateway);
    if (g == d->gateways.end()) {
        // TODO
        // request->status = NoGatway;
        d->abandonedSubscribeRequests[gateway].append(request);
        qCritical() << "Not gateway found for" << gateway;
        return;
    }
    g.value()->Subscribe(request);
    d->subscribeRequests[gateway].append(request);
}
