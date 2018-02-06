#ifndef OZ_RECORDSTRATEGY_H
#define OZ_RECORDSTRATEGY_H
#include "core/strategyabstract.h"
namespace Qs {
class RecordStrategyPrivate;
class RecordStrategy : public QObject, public StrategyAbstract {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID  "com.investredcat.QTrader.StrategyAbstract")
    Q_INTERFACES(Qs::StrategyAbstract)

public:
    RecordStrategy();
    virtual void setEngine(EngineAbstract* engine) override;
    virtual void onTick(TickInfo* info) override;
    virtual void onTrade(TradeInfo* info) override;
    virtual void onOrder(OrderInfo* info) override;

private:
    RecordStrategyPrivate* const d;
};
} // namespace Qs

#endif // OZ_RECORDSTRATEGY_H
