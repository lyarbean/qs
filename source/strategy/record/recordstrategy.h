#ifndef OZ_RECORDSTRATEGY_H
#define OZ_RECORDSTRATEGY_H
#include "core/strategyabstract.h"
namespace Qs {
class RecordStrategyPrivate;
class RecordStrategy : public StrategyAbstract {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.investredcat.QuantitativeSystem.StrategyAbstract" FILE "recordstrategy.json")
    Q_INTERFACES(Qs::StrategyAbstract)

public:
    RecordStrategy();
    virtual void onTick(TickInfoPointer& info) override;
    virtual void onTrade(TradeInfoPointer& info) override;
    virtual void onOrder(OrderInfoPointer& info) override;

private:
    RecordStrategyPrivate* const d;
};
} // namespace Qs

#endif // OZ_RECORDSTRATEGY_H
