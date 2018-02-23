#ifndef QS_STRATEGYABSTRACT_H
#define QS_STRATEGYABSTRACT_H
#include <QObject>
#include <QtPlugin>
#include "datatypes.h"
namespace Qs {
class EngineAbstract;
class StrategyAbstract : public QObject {
    Q_OBJECT
public:
    virtual void onTick(TickInfoPointer& info) = 0;
    virtual void onTrade(TradeInfoPointer& info) = 0;
    virtual void onOrder(OrderInfoPointer& info) = 0;
    // Generate on call
    virtual const QUuid& uuid() = 0;
    // Add gateway to whitelist, drop ticks if not matched
    virtual void acceptGateway(const QUuid& uuid) = 0;
signals:
    void order(OrderRequestPointer& request, QUuid& gatewayId);
    void cancelOrder(CancelOrderRequestPointer& request, QUuid& gatewayId);
};
} // namespace Qs
Q_DECLARE_INTERFACE(Qs::StrategyAbstract, "com.investredcat.QuantitativeSystem.StrategyAbstract")
#endif // QS_STRATEGYABSTRACT_H
