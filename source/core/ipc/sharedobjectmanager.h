#ifndef SHAREDOBJECTMANAGER_H
#define SHAREDOBJECTMANAGER_H
#include <QtCore>
#include <QMutexLocker>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h>    /* For O_* constants */
#include <unistd.h>
#include <cstring>
#include "core/datatypes.h"

namespace Qs {
// TODO MT-safe
// It may be safe for one gateway to set data but not safe to update pathToRef
class SharedObjectManager {
public:
    static SharedObjectManager& instance() {
        static SharedObjectManager m;
        return m;
    }
    // allocates N units and returns last index via \a index
    template <typename T, size_t N = 100000>
    bool allocate(const QByteArray& path, T*& t, int& index);

    template <typename T>
    T* get(const QByteArray& path, const int index);

    void close(const QByteArray& path);

    void closeAll();
private:
    SharedObjectManager() = default;
    SharedObjectManager(const SharedObjectManager&) = delete;
private:
    struct Ref {
        int currentIndex = 0; // current index zero-based
        size_t allocated = 0;
        void* head = nullptr;
    };
    QMap<QByteArray, Ref> pathToRef;
    QMutex mutex;
};

template <typename T, size_t N>
bool SharedObjectManager::allocate(const QByteArray& path, T*& t, int& index) {
    QMutexLocker locker(&mutex);
    Ref& ref = pathToRef[path];
    if (ref.currentIndex > ref.allocated) {
        // issue no resource
        return false;
    }

    if (N > ref.allocated) {
        int fd = shm_open(path.data(), O_RDWR | O_CREAT | O_APPEND, S_IRWXU);
        if (fd == -1) {
            return false;
        }
        if (ftruncate(fd, sizeof(T) * N) == -1) {
            qCritical() << "ftruncate" << errno;
            ::close(fd);
            return false;
        }
        ref.head = mmap(NULL, sizeof(T) * N, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
        if (ref.head == MAP_FAILED) {
            ::close(fd);
            return false;
        }
        // Do nothing though failed on  madvise or mlock
        if (madvise(ref.head, sizeof(T) * N, MADV_RANDOM) == -1) {
            qCritical() << "madvise!";
            munmap(ref.head, ref.allocated);
            ::close(fd);
            return false;
        }
        if (mlock(ref.head, sizeof(T) * N) == -1) {
            qCritical() << "mlock!";
            munmap(ref.head, ref.allocated);
            ::close(fd);
            return false;
        }
        ref.allocated = N;
        ::close(fd);
    }
    index = ref.currentIndex;
    ref.currentIndex++;
    t = reinterpret_cast<T*>(ref.head) + index;
    return true;
}

template <typename T>
T* SharedObjectManager::get(const QByteArray& path, const int index) {
    QMutexLocker locker(&mutex);
    Ref& ref = pathToRef[path];
    if (ref.currentIndex <= index) {
        // issue no resource
        return nullptr;
    }
    if (ref.head == MAP_FAILED) {
        return nullptr;
    }
    return reinterpret_cast<T*>(ref.head) + index;
}

enum InfoKind {
    TickInfoKind = 0,
    OrderInfoKind,
    TradeInfoKind,
    PositionInfoKind,
    AccountInfoKind,
    ErrorInfoKind,
    RequestInfoKind,
};

inline QByteArray infoKindNameOf(InfoKind infoKind) {
    switch (infoKind) {
        case TickInfoKind:
            return "TickInfoKind";
        case OrderInfoKind:
            return "OrderInfoKind";
        case TradeInfoKind:
            return "TradeInfoKind";
        case PositionInfoKind:
            return "PositionInfoKind";
        case AccountInfoKind:
            return "AccountInfoKind";
        case ErrorInfoKind:
            return "ErrorInfoKind";
        case RequestInfoKind:
            return "RequestInfoKind";
        default:
            return "UnknownInfoKind";
    }
}

/**
* @brief Infopath for IPC
* 
* @param gateway p_gateway: the uuid of gateway
* @param infoKind p_infoKind: the kind the path interests
* @param ticker p_ticker: the ticker the path associated
* @param date p_date:
* @return QByteArray {ef002017-6a4b-4d92-91a4-279796af9674}_0_sz002268_20180319

*/

inline QByteArray infoPathOf(const QByteArray& gateway, InfoKind infoKind, const QByteArray& ticker) {
    return QByteArray("/")
        .append(infoKindNameOf(infoKind)).append('_')
        .append(gateway).append('_')
        .append(ticker);
}


// TODO plugins infos, e.g.
// bi-direction mapping
// plugin-instance :==> plugin-file
// plugin-file :==> plugin-instance

// TODO ways to know the combination of gateways and strategies

enum RequestKind {
    DummyRequestKind = 0,
    OrderRequestKind,
    CancelOrderRequestKind,
    SubscribeRequestKind,
    QueryAccountRequestKind,
};

struct RequestInfo {
    RequestKind kind;
    union {
        OrderRequest o;
        CancelOrderRequest c;
        SubscribeRequest s;
        // QueryAccountRequest a;
        // QueryPositionRequest p;
    } d;
};

}
#endif // SHAREDOBJECTMANAGER_H
