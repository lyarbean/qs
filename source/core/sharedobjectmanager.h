#ifndef SHAREDOBJECTMANAGER_H
#define SHAREDOBJECTMANAGER_H
#include <QtCore>
#include <QMutexLocker>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h>    /* For O_* constants */
#include <unistd.h>
#include <cstring>

namespace Qs {
// TODO MT-safe
// It may be safe for one gateway to set data but not safe to update pathToRef
class SharedObjectManager {
public:
    static SharedObjectManager& instance() {
        static SharedObjectManager m;
        return m;
    }
    template <typename T, int N = 100000>
    T* allocate(const QByteArray& path, int& index);

    template <typename T>
    T* get(const QByteArray& path, const int index);
    void close(const QByteArray& path);
    void closeAll();
private:
    SharedObjectManager() = default;
    SharedObjectManager(const SharedObjectManager&) = delete;
private:
    struct Ref {
        int fd = 0;
        int currentIndex = 0; // current unused index
        int allocated = 0;
        void* head = nullptr;
    };
    QMap<QByteArray, Ref> pathToRef;
    QMutex mutex;
};

template <typename T, int N>
T* SharedObjectManager::allocate(const QByteArray& path, int& index) {
    QMutexLocker locker(&mutex);
    Ref& ref = pathToRef[path];
    if (ref.currentIndex > ref.allocated) {
        // issue no resource
        return nullptr;
    }
    int& fd = ref.fd;
    if (!fd) {
        fd = shm_open(path.data(), O_RDWR | O_CREAT | O_APPEND, S_IRWXU);
        if (fd == -1) {
            fd = 0;
            return nullptr;
        }
        if (ftruncate(fd, sizeof(T) * N) == -1) {
            qCritical() << "ftruncate" << errno;
            return nullptr;
        }
        ref.allocated = N;
        ref.head = mmap(NULL, sizeof(T) * N, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    }
    if (N > ref.allocated) {
        if (ftruncate(fd, sizeof(T) * N) == -1) {
            qCritical() << "ftruncate" << errno;
            return nullptr;
        } else {
            ref.allocated = N;
            // remap
            ref.head = mmap(NULL, sizeof(T) * N, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
        }
    }
    index = ref.currentIndex;
    ref.currentIndex++;
    return reinterpret_cast<T*>(ref.head) + index;
}

template <typename T>
T* SharedObjectManager::get(const QByteArray& path, const int index) {
    QMutexLocker locker(&mutex);
    Ref& ref = pathToRef[path];
    if (ref.currentIndex <= index) {
        // issue no resource
        return nullptr;
    }
    return reinterpret_cast<T*>(ref.head) + index;
}

void SharedObjectManager::close(const QByteArray& path) {
    QMutexLocker locker(&mutex);
    Ref& ref = pathToRef[path];
    ::close(ref.fd);
    pathToRef.remove(path);
}

void SharedObjectManager::closeAll() {
    QMutexLocker locker(&mutex);
    for (auto& ref : pathToRef) {
        ::close(ref.fd);
    }
    pathToRef.clear();
}

}
#endif // SHAREDOBJECTMANAGER_H
