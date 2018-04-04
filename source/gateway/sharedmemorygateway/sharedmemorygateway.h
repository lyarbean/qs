#ifndef QS_SHAREDMEMORYGATEWAY_H
#define QS_SHAREDMEMORYGATEWAY_H

#include "core/datatypes.h"
#include "core/gatewayabstract.h"

namespace Qs {
class SharedMemoryGatewayPrivate;
class SharedMemoryGateway : public GatewayAbstract
{
public:
    SharedMemoryGateway();
    SharedMemoryGateway(const SharedMemoryGateway& other) = delete;
    ~SharedMemoryGateway();
    virtual void connectToServer();
    virtual void disconnectFromServer();
    virtual void queryAccount();
    virtual void queryPosition();
    virtual void sendOrder(OrderRequestPointer& request);
    virtual void cancelOrder(CancelOrderRequestPointer& request);
    virtual void subscribe(SubscribeRequestPointer& request);
    virtual const QUuid& uuid();

private:
    SharedMemoryGatewayPrivate* const d;
};
}

#endif // QS_SHAREDMEMORYGATEWAY_H
