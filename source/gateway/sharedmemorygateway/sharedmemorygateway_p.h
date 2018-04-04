#ifndef SHAREDMEMORYGATEWAYPRIVATE_H
#define SHAREDMEMORYGATEWAYPRIVATE_H
#include <QThread>
#include <QMap>
#include <QUuid>
#include <sys/epoll.h>
#include "core/datatypes.h"
#include "sharedmemorygateway.h"
namespace Qs {

class SharedMemoryGatewayPrivate : public QThread
{
    Q_OBJECT
    enum {
        MaxEvents = 1024 * 16,
    };
public:
    SharedMemoryGatewayPrivate(SharedMemoryGateway* q);
    virtual ~SharedMemoryGatewayPrivate();
    void run() override;
    void stop();
    RequestStatus subscribe(const QUuid& gatewayId, const QByteArray& ticker);
    RequestStatus unSubscribe(const QUuid& gatewayId, const QByteArray& ticker);
private:
    friend class SharedMemoryGateway;
    SharedMemoryGateway* const q;
    int epollfd = -1;
    bool toExit = false;
    QUuid uuid;
    struct epoll_event events[MaxEvents];
    // TODO protection
    QMap<QByteArray, int> pathToFds;
    QMap<int, QByteArray> fdToPaths;
    QMap<QByteArray, void*> pathToMetaData;
    QMap<QByteArray, int> cursors; // cursors for tickers path
};
}

#endif // SHAREDMEMORYGATEWAYPRIVATE_H
