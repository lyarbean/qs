#ifndef OZ_NULLTEST_H
#define OZ_NULLTEST_H
#include "core/gatewayabstract.h"
#include "core/strategyabstract.h"
#include "core/datatypes.h"
#include "core/defaultengine.h"
#include <QCoreApplication>
#include <QThread>
#include <QTimer>
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
};

class NullOrderRequest : public OrderRequest {};

class NullStrategy : public StrategyAbstract {
public:
    NullStrategy()
      : received(0) {
    }
    ~NullStrategy() {
    }
    void count() {
        qCritical() << "Tick Received" << received;
    }

    virtual void onTick(TickInfoPointer& info) override {
        received++;
        OrderRequestPointer request(new NullOrderRequest);
        emit order(request);
    }
    virtual void onTrade(TradeInfoPointer& info) override {
    }
    virtual void onOrder(OrderInfoPointer& info) override {
    }
    quint64 received;
};

class NullGateway : public GatewayAbstract {
    Q_OBJECT
public:
    NullGateway() {
        received = 0;
        moveToThread(&thread);
        thread.start();
    }
    ~NullGateway() {
        thread.quit();
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
    virtual void Subscribe(SubscribeRequestPointer& request) {
    }
    Q_INVOKABLE void run() {
        while (true) {
            // FIXME
            TickInfoPointer tick = QSharedPointer<NullTickInfo>::create();
            emit hasTick(tick);
        }
    }
    void count() {
        qCritical() << "Order Received" << received;
    }

private:
    QThread thread;
    quint64 received;
};
} // namespace Qs
#endif // OZ_NULLTEST_H
