#ifndef QS_SINAGATEWAY_H
#define QS_SINAGATEWAY_H
#include "core/gatewayabstract.h"
namespace Qs {
class SinaGatewayPrivate;
class SinaGateway : public GatewayAbstract {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.investredcat.QuantitativeSystem.GatewayAbstract" FILE "sinagateway.json")
    Q_INTERFACES(Qs::GatewayAbstract)
public:
    SinaGateway();
    ~SinaGateway();
    virtual const QUuid& uuid() override;
public slots:
    virtual void connectToServer() override;
    virtual void disconnectFromServer() override;
    virtual void queryAccount() override;
    virtual void queryPosition() override;
    virtual void sendOrder(OrderRequestPointer& request) override;
    virtual void cancelOrder(CancelOrderRequestPointer& request) override;
    virtual void subscribe(SubscribeRequestPointer& request) override;

protected slots:
    void pull();

private:
    SinaGatewayPrivate * const d;
};
} // namespace Qs

#endif // QS_SINAGATEWAY_H
