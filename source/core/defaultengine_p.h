#ifndef DEFAULTENGINEPRIVATE_H
#define DEFAULTENGINEPRIVATE_H
#include <QMap>
#include <QVector>
#include <QThread>
namespace Qs {
class StrategyAbstract;
class RiskManager;
class PositionManager;
class DefaultEnginePrivate {
public:
    DefaultEnginePrivate(DefaultEngine* q);
    virtual ~DefaultEnginePrivate();

private:
    friend class DefaultEngine;
    DefaultEngine* const q;
    QThread thread;
    RiskManager* riskManager;
    PositionManager* positionManager;
    QMap<QUuid, StrategyAbstract*> strategies;
    QMap<QUuid, GatewayAbstract*> gateways;
    QMap<QString, QVector<TickInfoPointer>> tickInfos;
    QMap<QUuid, QVector<OrderInfoPointer>> orderInfos;
    QMap<QUuid, QVector<TradeInfoPointer>> tradeInfos;
    QMap<QUuid, QVector<OrderRequestPointer>> orderRequests;
    QMap<QUuid, QVector<OrderRequestPointer>> abandonedOrderRequests;
    QMap<QUuid, QVector<CancelOrderRequestPointer>> cancelOrderRequests;
    QMap<QUuid, QVector<CancelOrderRequestPointer>> abandonedCancelOrderRequests;
    QMap<QUuid, QVector<SubscribeRequestPointer>> subscribeRequests;
    QMap<QUuid, QVector<SubscribeRequestPointer>> abandonedSubscribeRequests;
    // SubscribeRequest
};
} // namespace Qs

#endif // DEFAULTENGINEPRIVATE_H
