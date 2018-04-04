#include "sharedmemorygateway_p.h"
#include "core/ipc/sharedobjectmanager.h"
#include "core/ipc/shmtickinfo.h"
#include "core/ipc/readwritelocker.h"
#include "core/ipc/ipcdatatypes.h"
#include <QDebug>

namespace Qs {
SharedMemoryGatewayPrivate::SharedMemoryGatewayPrivate(SharedMemoryGateway* q) : q(q) {
    uuid = QUuid::createUuid();
}

SharedMemoryGatewayPrivate::~SharedMemoryGatewayPrivate()
{
    stop();
}

void SharedMemoryGatewayPrivate::run() {
    epollfd = epoll_create1(0);
    if (epollfd == -1) {
        qCritical() << __PRETTY_FUNCTION__ << "Fail on epoll_create1";
    }
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = 0;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, ev.data.fd , &ev) == -1) {
        qCritical() << __PRETTY_FUNCTION__ << "Fail on epoll_wait" << errno;
        return;
    }
    const int waitTimeout = 1000; // 1 second
    while (true) {
        if (epollfd == -1) {
            return;
        }
        int count =  epoll_wait(epollfd, events, MaxEvents, waitTimeout);
        if (count == 0) {
            // TIMEOUT
            if (toExit) {
                // close all fds
                stop();
                return;
            }
            continue;
        }
        if (count == -1) {
            qCritical() << __PRETTY_FUNCTION__ << "Fail on epoll_wait" << errno;
            return;
        }
        for (int i = 0; i < count; ++i) {
            int fd = events[i].data.fd;
            auto t = fdToPaths.find(fd);
            // We get TickInfoKind
            if (t != fdToPaths.end()) {
                /**
                 * Access metadata and fetch last index from path_meta
                **/
                QByteArray& path = t.value();
                int cursor = cursors[path];
                int newCursor = 0;
                Qs::FutexReadWriteLocker locker(path);
                locker.lockForRead();
                MetaData* m = (MetaData*)pathToMetaData[path];
                newCursor = m->cursor;
                locker.unlockForRead();
                cursors[path] = newCursor;
                for (int c = cursor; c < newCursor; ++c) {
                    // TODO uuid? Use ours or underlying
                    auto shmtick = ShmTickInfo::create(uuid, path, c);
                    if (shmtick) {
                        TickInfoPointer tickInfo(shmtick);
                        emit q->hasTick(tickInfo);
                    }
                }
            }
        }
    }
}

void SharedMemoryGatewayPrivate::stop() {
    // close all fds
    for (auto k : fdToPaths.keys()) {
        ::close(k);
    }
    fdToPaths.clear();
    pathToFds.clear();
    if (epollfd != -1) {
        ::close(epollfd);
        epollfd = -1;
    }
}

// Write subscription to SharedMemoryObject A, and get create SharedMemoryObject B for ticker,
// Also associate fd with that ticker
RequestStatus SharedMemoryGatewayPrivate::subscribe(const QUuid& gatewayId, const QByteArray& ticker) {
    QByteArray path = infoPathOf(gatewayId.toByteArray(), TickInfoKind, ticker);
    if (pathToFds.find(path) != pathToFds.end()) {
        return Inserted;
    }
    int fd = shm_open(path.data(), O_RDONLY | O_CREAT, S_IRWXU);
    if (fd == -1) {
        return FailedToInsert;
    }
    {
        bool needInitialize = false;
        int metaFd = shm_open((path + "_meta").data(), O_RDWR | O_CREAT | O_EXCL, S_IRWXU);
        if (metaFd == -1) { // Doesn't exist
            metaFd =  shm_open((path + "_meta").data(), O_RDWR | O_CREAT, S_IRWXU);
            if (metaFd == -1) {
                ::close (fd);
                return FailedToInsert;
            }
            needInitialize = true;
        }
        if (ftruncate(metaFd, sizeof(MetaData)) == -1) {
            ::close (fd);
            ::close(metaFd);
            return FailedToInsert;
        }
        MetaData* m = (MetaData*) mmap(NULL, sizeof(MetaData), PROT_WRITE | PROT_READ, MAP_SHARED, metaFd, 0);
        if (m == MAP_FAILED) {
            ::close(fd);
            ::close(metaFd);
            return FailedToInsert;
        }
        if (madvise(m, sizeof(MetaData), MADV_RANDOM) == -1) {
            munmap(m, sizeof(MetaData));
            ::close(fd);
            ::close(metaFd);
            return FailedToInsert;
        }
        if (mlock(m, sizeof(MetaData)) == -1) {
            munmap(m, sizeof(MetaData));
            ::close(fd);
            ::close(metaFd);
            return FailedToInsert;
        }
        if (needInitialize) {
            memset(m, '\0', sizeof(MetaData));
        }
        ::close(metaFd);
    }
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = fd;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        qCritical() << __PRETTY_FUNCTION__ << "Fail on EPOLL_CTL_ADD" << errno;
        close(fd);
    }
    fdToPaths[fd] = path;
    pathToFds[path] = fd;
    cursors[path] = 0;
    return Inserted;
}

RequestStatus SharedMemoryGatewayPrivate::unSubscribe(const QUuid& gatewayId, const QByteArray& ticker) {
    QByteArray path = infoPathOf(gatewayId.toByteArray(), TickInfoKind, ticker);
    auto p = pathToFds.find(path);
    if (p != pathToFds.end()) {
        auto fd = p.value();
        pathToFds.erase(p);
        fdToPaths.remove(fd);
        // TODO do check?
        munlock(pathToMetaData[path], sizeof(MetaData));
        munmap(pathToMetaData[path], sizeof(MetaData));
        pathToMetaData.remove(path);
        if (epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL) == -1) {
            qCritical() << __PRETTY_FUNCTION__ << "Fail on EPOLL_CTL_DEL" << errno;
        }
        close(fd);
    }
    return Done;
}

}
