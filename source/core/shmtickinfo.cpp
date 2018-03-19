#include "shmtickinfo.h"
#include "shmtickinfo_p.h"
#include "utils/utils.h"
#include "sharedobjectmanager.h"
#include <QDebug>
using namespace Qs;

const int MaxTickCount = 100000; // for One trade day

QByteArray toShmBytes(const TickInfo& t) {
    QByteArrayList p;
    p << t.gateway().toByteArray();
    p << t.ticker();
    p << QByteArray::number(t.datetime());
    return p.join('/');
}

QsTick fromTickInfo(const TickInfo& t) {
    QsTick tick;
    auto ticker = t.ticker();
    memcpy(tick.ticker, ticker.data(), ticker.count());
    auto gateway = t.gateway().toByteArray();
    memcpy(tick.gateway, gateway.data(), gateway.count());
    tick.datetime = t.datetime();
    tick.bidPrice[FirstOrder] = t.bidPrice(FirstOrder);
    return tick;
}

ShmTickInfoPrivate::ShmTickInfoPrivate(ShmTickInfo* q)
  : q(q) {
}
ShmTickInfoPrivate::~ShmTickInfoPrivate() {
}

// gateway_ticker_datetime
void ShmTickInfoPrivate::set(const TickInfo& t) {
//     QByteArrayList p;
//     p << t.gateway().toByteArray().replace('{', '_').replace('}', '_');
//     p << t.ticker();
//     p << QByteArray::number(t.datetime());
//     path = "/";
//     path.append(p.join('_'));
    // QByteArray& gateway, InfoKind infoKind, QByteArray& contract, QByteArray& date
    path = infoPathOf(t.gateway().toByteArray(), TickInfoKind, t.ticker(),
                      QByteArray::number( dateOf(t.datetime())));
    QsTick* tick = nullptr;
    bool ok = SharedObjectManager::instance().allocate<QsTick>(path, tick, index);
    if (!ok) {
        // issue error
        Q_ASSERT(tick);
        qCritical() << "No tick";
        return;
    }
    QsTick qsTick = fromTickInfo(t);
    memcpy(tick, &qsTick, sizeof(QsTick));
    if (index) {
        auto t = SharedObjectManager::instance().get<QsTick>(path, index - 1);
        qCritical() << t->gateway << t->bidPrice[FirstOrder];
    }
}

QsTick* ShmTickInfoPrivate::tick() {
    return SharedObjectManager::instance().get<QsTick>(path, index);
}

ShmTickInfo::ShmTickInfo(const QUuid& gateway)
  : TickInfo(gateway)
  , d(new ShmTickInfoPrivate(this)) {
}


Qs::ShmTickInfo::~ShmTickInfo()
{
    delete d;
}


ShmTickInfo ShmTickInfo::create(const TickInfo& t) {
    ShmTickInfo info(t.gateway());
    info.d->set(t);
    return info;
}

QByteArray ShmTickInfo::ticker() const {
    QsTick* t = d->tick();
    if (!t) {
        return QByteArray();
    }
    return t->ticker;
}

// TODO
QByteArray ShmTickInfo::tickerName() const {
    d->tick()->ticker;
}

// TODO
QUuid ShmTickInfo::gateway() const {
    d->tick()->gateway;
}

// TODO
double ShmTickInfo::lastPrice() const {
//     d->tick()->lastPrice();
}

// TODO
double ShmTickInfo::preClosePrice() const {
//     d->tick()->preClosePrice();
}

// TODO
double ShmTickInfo::openPrice() const {
//     d->tick()->openPrice();
}

// TODO
double ShmTickInfo::closePrice() const {
}

// TODO
double ShmTickInfo::highPrice() const {
}

// TODO
double ShmTickInfo::lowPrice() const {
}

// TODO
double ShmTickInfo::preOpenInterest() const {
}

// TODO
double ShmTickInfo::openInterest() const {
}

// TODO
double ShmTickInfo::preSettlementPrice() const {
}

// TODO
double ShmTickInfo::settlementPrice() const {
}

// TODO
double ShmTickInfo::upperLimitPrice() const {
}

// TODO
double ShmTickInfo::lowerLimitPrice() const {
}

// TODO
double ShmTickInfo::previousDelta() const {
}

// TODO
double ShmTickInfo::currentDelta() const {
}

// TODO
quint64 ShmTickInfo::volume() const {
}

// TODO
double ShmTickInfo::turnover() const {
}

// TODO
double ShmTickInfo::averagePrice() const {
}

// TODO
quint64 ShmTickInfo::datetime() const {
}

// TODO
double ShmTickInfo::bidPrice(Qs::QuoteOrderType which) const {
}

// TODO
quint64 ShmTickInfo::bidVolume(Qs::QuoteOrderType which) const {
}

// TODO
double ShmTickInfo::askPrice(Qs::QuoteOrderType which) const {
}
// TODO

quint64 ShmTickInfo::askVolume(Qs::QuoteOrderType which) const {
}

// TODO
qint64 ShmTickInfo::tradeCount() const {
}

// TODO
quint64 ShmTickInfo::totalBidVolume() const {
}


// TODO
quint64 ShmTickInfo::totalAskVolume() const {
}

// TODO
double ShmTickInfo::maBidPrice() const {
}

// TODO
double ShmTickInfo::maAskPrice() const {
}

// TODO
double ShmTickInfo::maBondBidPrice() const {
}

// TODO
double ShmTickInfo::maBondAskPrice() const {
}

// TODO
double ShmTickInfo::yieldToMaturity() const {
}

// TODO
double ShmTickInfo::iopv() const {
}

// TODO
quint64 ShmTickInfo::etfBuyCount() const {
}

// TODO
quint64 ShmTickInfo::etfSellCount() const {
}

// TODO
double ShmTickInfo::etfBuyVolume() const {
}

// TODO
double ShmTickInfo::etfBuyMoney() const {
}

// TODO
double ShmTickInfo::etfSellVolume() const {
}

// TODO
double ShmTickInfo::etfSellMoney() const {
}

// TODO
double ShmTickInfo::totalWarrantExecVolume() {
}

// TODO
double ShmTickInfo::warrantLowerPrice() {
}

// TODO
double ShmTickInfo::warrantUpperPrice() {
}

// TODO
quint64 ShmTickInfo::cancelBuyCount() const {
}

// TODO
quint64 ShmTickInfo::cancelSellCount() const {
}

// TODO
double ShmTickInfo::cancelBuyVolume() const {
}

// TODO
double ShmTickInfo::cancelSellVolume() const {
}

// TODO
quint64 ShmTickInfo::totalBuyCount() const {
}

// TODO
quint64 ShmTickInfo::totalSellCount() const {
}

// TODO
quint32 ShmTickInfo::durationAfterBuy() const {
}

// TODO
quint32 ShmTickInfo::durationAfterSell() const {
}

// TODO
quint32 ShmTickInfo::countOfBidOrders() const {
}

// TODO
quint32 ShmTickInfo::countOfAskOrders() const {
}

// TODO
quint32 ShmTickInfo::execTime() const {
}

// TODO
bool ShmTickInfo::isMarketClosed() const {
}

// TODO
double ShmTickInfo::totalPosition() const {
}

// TODO
double ShmTickInfo::peRatio1() const {
}

// TODO
double ShmTickInfo::peRatio2() const {
}
