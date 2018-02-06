#ifndef OZ_SINASTRATEGY_H
#define OZ_SINASTRATEGY_H
#include "core/strategyabstract.h"

namespace Qs {

class SinaStrategy : public QObject, public Qs::StrategyAbstract
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID  "com.investredcat.QTrader.StrategyAbstract")
    Q_INTERFACES(Qs::StrategyAbstract)

public:
    explicit SinaStrategy();
    virtual void setEngine(EngineAbstract* engine);
    virtual void onTick(TickInfo* info);
    virtual void onTrade(TradeInfo* info);
    virtual void onOrder(OrderInfo* info);

private:
    class SinaStrategyPrivate* const d;
};
}

#endif // OZ_SINASTRATEGY_H
