#ifndef QS_DEFAULTENGINE_H
#define QS_DEFAULTENGINE_H
#include <QObject>
#include "engineabstract.h"

namespace Qs {
class DefaultEnginePrivate;
class DefaultEngine : public EngineAbstract {
    Q_OBJECT
public:
    explicit DefaultEngine();
    ~DefaultEngine();
    virtual void connectServers() override;
    virtual void connectServer(const QUuid&) override;
    virtual void addStrategy(StrategySharedPointer& strategy) override;
    virtual void addGateway(GatewaySharedPointer& gateway) override;
    virtual RiskManager* riskManager() const override;
    virtual PositionManager* positionManager() const override;

    // public slots:
    virtual void onTick(TickInfoPointer& info) override;
    virtual void onTrade(TradeInfoPointer& info) override;
    virtual void onOrder(OrderInfoPointer& info) override;
    virtual void onPoisition(PoisitionInfoPointer& info) override;
    virtual void onAccount(AccountInfoPointer& info) override;
    virtual void onContract(ContractInfoPointer& info) override;
    virtual void onLog(LogInfoPointer& info) override;
    virtual void sendOrder(OrderRequestPointer& request, const QUuid& gateway) override;
    virtual void cancelOrder(CancelOrderRequestPointer& request, const QUuid& gateway) override;
    virtual void Subscribe(SubscribeRequestPointer& request, const QUuid& gateway) override;
    void sendOrder(OrderRequestPointer& request);

private:
    DefaultEnginePrivate* const d;
};
} // namespace Qs

#endif // QS_DEFAULTENGINE_H
