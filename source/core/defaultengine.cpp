#include "defaultengine.h"
#include "defaultengine_p.h"
#include "strategyabstract.h"
#include "gatewayabstract.h"
#include "datatypes.h"
#include "riskmanager.h"
#include <QDebug>
#include <QCoreApplication>
using namespace Qs;

DefaultEnginePrivate::DefaultEnginePrivate(DefaultEngine* q)
  : q(q) {
}
DefaultEnginePrivate::~DefaultEnginePrivate() {
}

DefaultEngine::DefaultEngine()
  : d(new DefaultEnginePrivate(this)) {
    qRegisterMetaType<TickInfoPointer>("TickInfoPointer&");
}

DefaultEngine::~DefaultEngine() {
    delete d;
}

void DefaultEngine::connectServers() {
    for (auto g : d->gateways)
        g.data()->connectToServer();
}

void DefaultEngine::connectServer(const QUuid& uuid) {
    auto g = d->gateways.find(uuid);
    if (g == d->gateways.end()) {
        return;
    }
    auto ref = g.value().lock();
    if (ref) {
        ref->connectToServer();
    }
}

void DefaultEngine::addStrategy(StrategySharedPointer& strategy) {
    const QUuid& uuid = strategy->uuid();
    d->strategies[uuid] = strategy;
}

void DefaultEngine::addGateway(GatewaySharedPointer& gateway) {
    const QUuid& uuid = gateway->uuid();
    d->gateways[uuid] = gateway;
    connect(gateway.data(), &GatewayAbstract::hasTick, this, &DefaultEngine::onTick);
    connect(gateway.data(), &GatewayAbstract::hasTrade, this, &DefaultEngine::onTrade);
    connect(gateway.data(), &GatewayAbstract::hasOrder, this, &DefaultEngine::onOrder);
    connect(gateway.data(), &GatewayAbstract::hasPoisition, this, &DefaultEngine::onPoisition);
    connect(gateway.data(), &GatewayAbstract::hasAccount, this, &DefaultEngine::onAccount);
    connect(gateway.data(), &GatewayAbstract::hasContract, this, &DefaultEngine::onContract);
    connect(gateway.data(), &GatewayAbstract::hasLog, this, &DefaultEngine::onLog);
}

RiskManager* DefaultEngine::riskManager() const {
    return nullptr;
}

PositionManager* DefaultEngine::positionManager() const {
    return nullptr;
}

void DefaultEngine::onTick(TickInfoPointer& info) {
    for (auto s : d->strategies) {
        auto ref = s.lock();
        if (ref) {
            ref->onTick(info);
        }
    }
    //  d->tickInfos[info->ticker()].append(info);
}

void DefaultEngine::onTrade(TradeInfoPointer& info) {
    for (auto s : d->strategies) {
        auto ref = s.lock();
        if (ref) {
            ref->onTrade(info);
        }
    }
    //  d->tradeInfos[info->ticker()].append(info);
}

void DefaultEngine::onOrder(OrderInfoPointer& info) {
    for (auto s : d->strategies) {
        auto ref = s.lock();
        if (ref) {
            ref->onOrder(info);
        }
    }
    //  d->orderInfos[info->ticker()].append(info);
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
        // request->status = NoGateway;
        //         d->abandonedOrderRequests[gateway].append(request);
        return;
    }
    if (!d->riskManager->check(request, gateway)) {
        // TODO
        // request->status = HasRisk;
        d->orderRequests[gateway].append(request);
        return;
    }
    auto ref = g.value().lock();
    if (ref) {
        ref->sendOrder(request);
        //         d->orderRequests[gateway].append(request);
    } else {
        // request->status = GatewayIsGone;
        //         d->abandonedOrderRequests[gateway].append(request);
    }
}

void DefaultEngine::sendOrder(OrderRequestPointer& request) {
    for (auto g : d->gateways) {
        auto ref = g.lock();
        if (ref) {
            ref->sendOrder(request);
        }
    }
    // TODO
    // d->orderRequests[gateway].append(request);
}

void DefaultEngine::cancelOrder(CancelOrderRequestPointer& request, const QUuid& gateway) {
    // d->riskManager->check()
    auto g = d->gateways.find(gateway);
    if (g == d->gateways.end()) {
        // TODO
        // request->status = NoGateway;
        //         d->abandonedCancelOrderRequests[gateway].append(request);
        return;
    }
    auto ref = g.value().lock();
    if (ref) {
        ref->cancelOrder(request);
        //         d->cancelOrderRequests[gateway].append(request);
    } else {
        // request->status = GatewayIsGone;
        //         d->abandonedCancelOrderRequests[gateway].append(request);
    }
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
        // request->status = NoGateway;
        d->abandonedSubscribeRequests[gateway].append(request);
        qCritical() << "Not gateway found for" << gateway;
        return;
    }
    auto ref = g.value().lock();
    if (ref) {
        ref->subscribe(request);
        //         d->subscribeRequests[gateway].append(request);
    } else {
        // request->status = GatewayIsGone;
        //         d->abandonedSubscribeRequests[gateway].append(request);
    }
}
