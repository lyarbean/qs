#ifndef OZ_STRATEGYABSTRACT_H
#define OZ_STRATEGYABSTRACT_H
#include <QtPlugin>
namespace Qs {
class TickInfo;
class TradeInfo;
class OrderInfo;

class EngineAbstract;
class StrategyAbstract {
public:
    virtual void setEngine(EngineAbstract* engine) = 0;
    virtual void onTick(TickInfo* info) = 0;
    virtual void onTrade(TradeInfo* info) = 0;
    virtual void onOrder(OrderInfo* info) = 0;
};
} // namespace Qs
Q_DECLARE_INTERFACE (Qs::StrategyAbstract, "com.investredcat.QTrader.StrategyAbstract")
#endif // OZ_STRATEGYABSTRACT_H
