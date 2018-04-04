#ifndef QS_READWRITELOCKER_H
#define QS_READWRITELOCKER_H
#include <QByteArray>
namespace Qs {

class ReadWriteLocker
{
public:
    explicit ReadWriteLocker(const QByteArray& name);
    virtual ~ReadWriteLocker();
    void lockForRead();
    void unlockForRead();
    void lockForWrite();
    void unlockForWrite();
//     bool tryLockForRead();
//     bool tryLockForRead(int timeout);
//     bool tryLockForWrite();
//     bool tryLockForWrite(int timeout);
//     void unlock();
private:
    class ReadWriteLockerPrivate* const d;
};

class FutexReadWriteLocker
{
public:
    explicit FutexReadWriteLocker(const QByteArray& name);
    virtual ~FutexReadWriteLocker();
    void lockForRead();
    void unlockForRead();
    void lockForWrite();
    void unlockForWrite();
//     bool tryLockForRead();
//     bool tryLockForRead(int timeout);
//     bool tryLockForWrite();
//     bool tryLockForWrite(int timeout);
//     void unlock();
private:
    class FutexReadWriteLockerPrivate* const d;
};
}

#endif // QS_READWRITELOCKER_H
