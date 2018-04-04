#ifndef READWRITELOCKERPRIVATE_H
#define READWRITELOCKERPRIVATE_H
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>

namespace Qs {

class ReadWriteLockerPrivate
{
public:
    ReadWriteLockerPrivate();
    virtual ~ReadWriteLockerPrivate();
    void open(const QByteArray& name);
    sem_t *toReadSemaphore = nullptr;
    sem_t *toWriteSemaphore = nullptr;
    sem_t *lock = nullptr;
    sem_t *activeReaders = nullptr;
    sem_t *activeWriters = nullptr;
    sem_t *waitingReaders = nullptr;
    sem_t *waitingWriters = nullptr;
    QByteArray name;
    bool initialized = false;
};

class FutexReadWriteLockerPrivate
{
public:
    FutexReadWriteLockerPrivate();
    virtual ~FutexReadWriteLockerPrivate();
    void open(const QByteArray& name);
    QByteArray path;
    bool initialized = false;
    struct _D {
        int lock = 1;
        int toRead = 0;
        int toWrite = 0;
        int activeReaders = 0;
        int activeWriters = 0;
        int waitingReaders = 0;
        int waitingWriters = 0;
    } *d = nullptr;
};
}

#endif // READWRITELOCKERPRIVATE_H
