#ifndef QS_SINASTRATEGY_H
#define QS_SINASTRATEGY_H
#include "core/strategyabstract.h"

namespace Qs {

class SinaStrategy : public Qs::StrategyAbstract {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.investredcat.QuantitativeSystem.StrategyAbstract" FILE "sinastrategy.json")
    Q_INTERFACES(Qs::StrategyAbstract)

public:
    explicit SinaStrategy();
    virtual void onTick(TickInfoPointer& info) override;
    virtual void onTrade(TradeInfoPointer& info) override;
    virtual void onOrder(OrderInfoPointer& info) override;
    virtual const QUuid& uuid() override;
    virtual void acceptGateway(const QUuid& gatewayId) override;
private:
    class SinaStrategyPrivate* const d;
};
} // namespace Qs

#endif // QS_SINASTRATEGY_H
