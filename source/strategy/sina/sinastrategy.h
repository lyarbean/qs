#ifndef OZ_SINASTRATEGY_H
#define OZ_SINASTRATEGY_H
#include "core/strategyabstract.h"

namespace Qs {

class SinaStrategy : public Qs::StrategyAbstract {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.investredcat.QuantitativeSystem.StrategyAbstract")
    Q_INTERFACES(Qs::StrategyAbstract)

public:
    explicit SinaStrategy();
    virtual void onTick(TickInfoPointer& info);
    virtual void onTrade(TradeInfoPointer& info);
    virtual void onOrder(OrderInfoPointer& info);

private:
    class SinaStrategyPrivate* const d;
};
} // namespace Qs

#endif // OZ_SINASTRATEGY_H
