#ifndef QS_XTPGATEWAY_H
#define QS_XTPGATEWAY_H
#include "core/gatewayabstract.h"
namespace Qs {
class XtpGatewayPrivate;
class XtpGateway : public GatewayAbstract {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.investredcat.QuantitativeSystem.GatewayAbstract" FILE "xtpgateway.json")
    Q_INTERFACES(Qs::GatewayAbstract)
public:
    XtpGateway();
    ~XtpGateway();
    // Use queued connection to invoke these commands
public slots:
    virtual void connectToServer() override;
    virtual void disconnectFromServer() override;
    virtual void queryAccount() override;
    virtual void queryPosition() override;
    virtual void sendOrder(OrderRequestPointer& request) override;
    virtual void cancelOrder(CancelOrderRequestPointer& request) override;
    virtual void subscribe(SubscribeRequestPointer& request) override;
    virtual const QUuid& uuid() override;

private:
    XtpGatewayPrivate* const d;
};
} // namespace Qs

#endif // QS_XTPGATEWAY_H
