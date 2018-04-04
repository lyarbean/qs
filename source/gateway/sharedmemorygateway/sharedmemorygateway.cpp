#include "sharedmemorygateway.h"
#include "sharedmemorygateway_p.h"

using namespace Qs;


SharedMemoryGateway::SharedMemoryGateway()
 : d(new SharedMemoryGatewayPrivate(this))
{

}

SharedMemoryGateway::~SharedMemoryGateway()
{
    delete d;

}

void SharedMemoryGateway::connectToServer()
{

}

void SharedMemoryGateway::disconnectFromServer()
{

}

void SharedMemoryGateway::queryAccount()
{

}

void SharedMemoryGateway::queryPosition()
{

}

void SharedMemoryGateway::sendOrder(Qs::OrderRequestPointer& request)
{

}

void SharedMemoryGateway::cancelOrder(Qs::CancelOrderRequestPointer& request)
{

}

void SharedMemoryGateway::subscribe(Qs::SubscribeRequestPointer& request)
{

}

const QUuid& SharedMemoryGateway::uuid()
{
    return d->uuid;
}
