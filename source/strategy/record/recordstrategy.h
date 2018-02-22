#ifndef QS_RECORDSTRATEGY_H
#define QS_RECORDSTRATEGY_H
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
    virtual const QUuid& uuid() override;
    virtual void addGateway(const QUuid& gatewayId) override;

private:
    RecordStrategyPrivate* const d;
};
} // namespace Qs

#endif // QS_RECORDSTRATEGY_H
