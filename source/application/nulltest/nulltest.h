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
    virtual QString ticker() const {}
    virtual QString tickerName() const {}
    virtual double averagePrice() const {}
    virtual double lastPrice() const {}
    virtual double preClosePrice() const {}
    virtual double openPrice() const {}
    virtual double closePrice() const {}
    virtual double highPrice() const {}
    virtual double lowPrice() const {}
    virtual quint64 volume() const {}
    virtual double turnover() const {}
    virtual double bidPrice(OrderSequenceType which) const {}
    virtual quint64 bidVolume(OrderSequenceType which) const {}
    virtual double askPrice(OrderSequenceType which) const {}
    virtual quint64 askVolume(OrderSequenceType which) const {}
    virtual quint64 datetime() const {} // YYYYMMDD
    virtual qint32 date() const {} // YYYYMMDD
    virtual qint32 day() const {}
    virtual qint32 hour() const {}
    virtual qint32 minute() const {}
    virtual qint32 second() const {}
    virtual qint32 millisecond() const {}
    virtual quint64 msecsSinceEpoch() const {}
};

class NullOrderRequest : public OrderRequest {};

class NullStrategy : public StrategyAbstract {
public:
    NullStrategy() : received(0) {}
    ~NullStrategy() {}
    void count() {
        qCritical() << "Tick Received" << received;
    }
    virtual void setEngine(EngineAbstract* engine) {
        this->engine = engine;
    }
    virtual void onTick(TickInfo* info) override {
        received++;
        static NullOrderRequest request;
        // I DO KNOW IT IS
       static_cast<DefaultEngine*>(engine)->sendOrder(&request);
    }
    virtual void onTrade(TradeInfo* info) override {}
    virtual void onOrder(OrderInfo* info) override {}
    quint64 received;
    EngineAbstract* engine;
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
    virtual void connectServer() {}
    virtual void closeServer() {}
    virtual void queryAccount() {}
    virtual void queryPosition() {}
    virtual void sendOrder(OrderRequest* request) {
        received ++;
    }
    virtual void cancelOrder(CancelOrderRequest* request) {}
    virtual void Subscribe(SubscribeRequest* request) {}
    Q_INVOKABLE void run() {
        while (true) {
            auto tick = new NullTickInfo;
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
}
#endif // OZ_NULLTEST_H
