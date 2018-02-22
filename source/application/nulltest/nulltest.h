#ifndef QS_NULLTEST_H
#define QS_NULLTEST_H
#include "core/gatewayabstract.h"
#include "core/strategyabstract.h"
#include "core/datatypes.h"
#include "core/defaultengine.h"
#include <QCoreApplication>
#include <QThread>
#include <QTimer>
#include <QUuid>
#include <QDebug>
namespace Qs {
const quint64 elapsed = 100000; // 100 seconds
class NullTickInfo : public TickInfo {
public:
    virtual QString ticker() const {
        return QString("null");
    }
    virtual QString tickerName() const {
        return QString();
    }
    virtual QUuid gateway() const {
        return gatewayId;
    }
    virtual double averagePrice() const {
        return 0.0;
    }
    virtual double lastPrice() const {
        return 0.0;
    }
    virtual double preClosePrice() const {
        return 0.0;
    }
    virtual double openPrice() const {
        return 0.0;
    }
    virtual double closePrice() const {
        return 0.0;
    }
    virtual double highPrice() const {
        return 0.0;
    }
    virtual double lowPrice() const {
        return 0.0;
    }
    virtual quint64 volume() const {
        return 0;
    }
    virtual double turnover() const {
        return 0.0;
    }
    virtual double bidPrice(OrderSequenceType which) const {
        return 0.0;
    }
    virtual quint64 bidVolume(OrderSequenceType which) const {
        return 0;
    }
    virtual double askPrice(OrderSequenceType which) const {
        return 0.0;
    }
    virtual quint64 askVolume(OrderSequenceType which) const {
        return 0;
    }
    virtual quint64 datetime() const {
        return 0;
    } // YYYYMMDD
    virtual qint32 date() const {
        return 0;
    } // YYYYMMDD
    virtual qint32 day() const {
        return 0;
    }
    virtual qint32 hour() const {
        return 0;
    }
    virtual qint32 minute() const {
        return 0;
    }
    virtual qint32 second() const {
        return 0;
    }
    virtual qint32 millisecond() const {
        return 0;
    }
    virtual quint64 msecsSinceEpoch() const {
        return 0;
    }
    QUuid gatewayId;
};

class NullOrderRequest : public OrderRequest {};

class NullStrategy : public StrategyAbstract {
public:
    NullStrategy()
      : received(0) {
    }
    ~NullStrategy() {
    }
    virtual const QUuid& uuid() {
        if (id.isNull()) {
            id = QUuid::createUuid();
        }
        return id;
    }
    void count() {
        qCritical() << "Tick Received" << received;
    }

    virtual void onTick(TickInfoPointer& info) override {
        received++;
        static OrderRequestPointer request(new NullOrderRequest);
        auto g = info->gateway();
        if (g == gateway) {
            emit order(request, gateway);
        }
    }
    virtual void onTrade(TradeInfoPointer& info) override {
    }
    virtual void onOrder(OrderInfoPointer& info) override {
    }
    virtual void addGateway(const QUuid& gatewayId) override {
        gateway = gatewayId;
    }
    QUuid gateway; // allowed gateway
    QUuid id;
    quint64 received;
};

class NullGateway : public GatewayAbstract {
    Q_OBJECT
public:
    NullGateway() {
        received = 0;
        toExit = false;
    }
    ~NullGateway() {
    }
    virtual const QUuid& uuid() {
        if (id.isNull()) {
            id = QUuid::createUuid();
        }
        return id;
    }
    virtual void connectServer() {
    }
    virtual void closeServer() {
    }
    virtual void queryAccount() {
    }
    virtual void queryPosition() {
    }
    virtual void sendOrder(OrderRequestPointer& request) {
        received++;
    }
    virtual void cancelOrder(CancelOrderRequestPointer& request) {
    }
    virtual void subscribe(SubscribeRequestPointer& request) {
    }
    Q_INVOKABLE void run() {
        while (!toExit) {
            // FIXME
            static TickInfoPointer tick = QSharedPointer<NullTickInfo>::create();
            static_cast<NullTickInfo*>(tick.data())->gatewayId = uuid();
            emit hasTick(tick);
        }
    }
    void count() {
        qCritical() << "Order Received" << received;
        toExit = true;
    }

private:
    quint64 received;
    QUuid id;
    volatile bool toExit;
};
} // namespace Qs
#endif // QS_NULLTEST_H
