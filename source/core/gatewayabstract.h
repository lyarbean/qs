#ifndef QS_GATEWAYABSTRACT_H
#define QS_GATEWAYABSTRACT_H
#include <QObject>
#include <QtPlugin>
#include "datatypes.h"
namespace Qs {
// class CancelOrderRequest;
// class OrderRequest;
// class SubscribeRequest;
//
// class AccountInfoPointer;
// class ContractInfoPointer;
// class LogInfoPointer;
// class OrderInfoPointer;
// class PoisitionInfoPointer;
// class TickInfoPointer;
// class TradeInfoPointer;

class GatewayAbstract : public QObject {
    Q_OBJECT
public:
    virtual void connectServer() = 0;
    virtual void closeServer() = 0;
    virtual void queryAccount() = 0;
    virtual void queryPosition() = 0;
    virtual void sendOrder(OrderRequestPointer& request) = 0;
    virtual void cancelOrder(CancelOrderRequestPointer& request) = 0;
    virtual void subscribe(SubscribeRequestPointer& request) = 0;
    // Generate on call
    virtual const QUuid& uuid() = 0;
signals:
    void hasTick(TickInfoPointer& info);
    void hasTrade(TradeInfoPointer& info);
    void hasOrder(OrderInfoPointer& info);
    void hasPoisition(PoisitionInfoPointer& info);
    void hasAccount(AccountInfoPointer& info);
    void hasContract(ContractInfoPointer& info);
    void hasLog(LogInfoPointer& info);
};
} // namespace Qs
Q_DECLARE_INTERFACE(Qs::GatewayAbstract, "com.investredcat.QuantitativeSystem.GatewayAbstract")

#endif // QS_GATEWAYABSTRACT_H
