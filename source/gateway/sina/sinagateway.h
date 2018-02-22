#ifndef QS_SINAGATEWAY_H
#define QS_SINAGATEWAY_H
#include "core/gatewayabstract.h"
namespace Qs {

// class CancelOrderRequest;
// class OrderRequest;
// class SubscribeRequest;

class SinaGateway : public GatewayAbstract {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.investredcat.QuantitativeSystem.GatewayAbstract" FILE "sinagateway.json")
    Q_INTERFACES(Qs::GatewayAbstract)
public:
    SinaGateway();
    ~SinaGateway();
    virtual void connectServer() override;
    virtual void closeServer() override;
    virtual void queryAccount() override;
    virtual void queryPosition() override;
    virtual void sendOrder(OrderRequestPointer& request) override;
    virtual void cancelOrder(CancelOrderRequestPointer& request) override;
    virtual void subscribe(SubscribeRequestPointer& request) override;
    virtual const QUuid& uuid() override;
protected slots:
    void pull();

private:
    class SinaGatewayPrivate* const d;
};
} // namespace Qs

#endif // QS_SINAGATEWAY_H
