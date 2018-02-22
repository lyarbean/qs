#ifndef QS_ENGINEABSTRACT_H
#define QS_ENGINEABSTRACT_H
#include <QUuid>
#include <QtPlugin>
#include "datatypes.h"
namespace Qs {

class GatewayAbstract;
class StrategyAbstract;
class RiskManager;
class PositionManager;

// class AccountInfoPointer;
// class ContractInfoPointer;
// class LogInfoPointer;
// class OrderInfoPointer;
// class PoisitionInfoPointer;
// class TickInfoPointer;
// class TradeInfoPointer;
//
// class CancelOrderRequest;
// class OrderRequest;
// class SubscribeRequest;
typedef QSharedPointer<StrategyAbstract> StrategySharedPointer;
typedef QWeakPointer<StrategyAbstract> StrategyWeakPointer;
typedef QSharedPointer<GatewayAbstract> GatewaySharedPointer;
typedef QWeakPointer<GatewayAbstract> GatewayWeakPointer;

class EngineAbstract : public QObject {
    Q_OBJECT
public:
    virtual void connectServers() = 0;
    virtual void connectServer(const QUuid&) = 0;
    virtual void addStrategy(StrategySharedPointer& strategy) = 0;
    virtual void addGateway(GatewaySharedPointer& gateway) = 0;
    virtual RiskManager* riskManager() const = 0;
    virtual PositionManager* positionManager() const = 0;
public slots:
    virtual void onTick(TickInfoPointer& info) = 0;
    virtual void onTrade(TradeInfoPointer& info) = 0;
    virtual void onOrder(OrderInfoPointer& info) = 0;
    virtual void onPoisition(PoisitionInfoPointer& info) = 0;
    virtual void onAccount(AccountInfoPointer& info) = 0;
    virtual void onContract(ContractInfoPointer& info) = 0;
    virtual void onLog(LogInfoPointer& info) = 0;
    virtual void sendOrder(OrderRequestPointer& request, const QUuid& gateway) = 0;
    virtual void cancelOrder(CancelOrderRequestPointer& request, const QUuid& gateway) = 0;
    virtual void Subscribe(SubscribeRequestPointer& request, const QUuid& gateway) = 0;
};
} // namespace Qs

Q_DECLARE_INTERFACE(Qs::EngineAbstract, "com.investredcat.QuantitativeSystem.EngineAbstract")
#endif // QS_ENGINEABSTRACT_H
