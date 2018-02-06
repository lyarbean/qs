#ifndef OZ_GATEWAYABSTRACT_H
#define OZ_GATEWAYABSTRACT_H
#include <QObject>
#include <QtPlugin>
namespace Qs {
class CancelOrderRequest;
class OrderRequest;
class SubscribeRequest;

class AccountInfo;
class ContractInfo;
class LogInfo;
class OrderInfo;
class PoisitionInfo;
class TickInfo;
class TradeInfo;

class GatewayAbstract : public QObject {
    Q_OBJECT
public:
    virtual void connectServer() = 0;
    virtual void closeServer() = 0;
    virtual void queryAccount() = 0;
    virtual void queryPosition() = 0;
    virtual void sendOrder(OrderRequest* request) = 0;
    virtual void cancelOrder(CancelOrderRequest* request) = 0;
    virtual void Subscribe(SubscribeRequest* request) = 0;
signals:
    void hasTick(TickInfo* info);
    void hasTrade(TradeInfo* info);
    void hasOrder(OrderInfo* info);
    void hasPoisition(PoisitionInfo* info);
    void hasAccount(AccountInfo* info);
    void hasContract(ContractInfo* info);
    void hasLog(LogInfo* info);
};
} // namespace Qs
Q_DECLARE_INTERFACE (Qs::GatewayAbstract, "com.investredcat.QTrader.GatewayAbstract")

#endif // OZ_GATEWAYABSTRACT_H
