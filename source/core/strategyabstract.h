#ifndef OZ_STRATEGYABSTRACT_H
#define OZ_STRATEGYABSTRACT_H
#include <QObject>
#include <QtPlugin>
#include "datatypes.h"
namespace Qs {
// class TickInfoPointer;
// class TradeInfoPointer;
// class OrderInfoPointer;

class EngineAbstract;
class StrategyAbstract : public QObject {
    Q_OBJECT
public:
    virtual void onTick(TickInfoPointer& info) = 0;
    virtual void onTrade(TradeInfoPointer& info) = 0;
    virtual void onOrder(OrderInfoPointer& info) = 0;
signals:
    void order(OrderRequestPointer& request);
    void cancelOrder(CancelOrderRequestPointer& request);
};
} // namespace Qs
Q_DECLARE_INTERFACE(Qs::StrategyAbstract, "com.investredcat.QuantitativeSystem.StrategyAbstract")
#endif // OZ_STRATEGYABSTRACT_H
