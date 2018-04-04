#include "readwritelocker.h"
#include "readwritelocker_p.h"
#include <QDebug>

Qs::ReadWriteLockerPrivate::ReadWriteLockerPrivate()
{
}

Qs::ReadWriteLockerPrivate::~ReadWriteLockerPrivate()
{
    sem_close(toReadSemaphore);
    sem_close(toWriteSemaphore);
    sem_close(lock);
    sem_close(activeReaders);
    sem_close(activeWriters);
    sem_close(waitingReaders);
    sem_close(waitingWriters);
    auto readSemaphoreName = name + "_read";
    auto writeSemaphoreName = name + "_write";
    auto arSemaphoreName = name + "_ar";
    auto awSemaphoreName = name + "_aw";
    auto wrSemaphoreName = name + "_wr";
    auto wwSemaphoreName = name + "_ww";
    auto lockSemaphoreName = name + "_lock";
    sem_unlink(readSemaphoreName);
    sem_unlink(writeSemaphoreName);
    sem_unlink(arSemaphoreName);
    sem_unlink(awSemaphoreName);
    sem_unlink(wrSemaphoreName);
    sem_unlink(wwSemaphoreName);
    sem_unlink(lockSemaphoreName);
}

void Qs::ReadWriteLockerPrivate::open(const QByteArray& name) {
    auto readSemaphoreName = name + "_read";
    auto writeSemaphoreName = name + "_write";
    auto arSemaphoreName = name + "_ar";
    auto awSemaphoreName = name + "_aw";
    auto wrSemaphoreName = name + "_wr";
    auto wwSemaphoreName = name + "_ww";
    auto lockSemaphoreName = name + "_lock";
    this->name = name;
    // TODO check failure
    toReadSemaphore = sem_open(readSemaphoreName.constData(), O_CREAT | O_CLOEXEC | O_RDWR, S_IRUSR | S_IWUSR, 0);
    toWriteSemaphore = sem_open(writeSemaphoreName.constData(), O_CREAT | O_CLOEXEC | O_RDWR, S_IRUSR | S_IWUSR, 0);
    lock = sem_open(lockSemaphoreName.constData(), O_CREAT | O_CLOEXEC | O_RDWR, S_IRUSR | S_IWUSR, 1); // Lock is open
    activeReaders = sem_open(arSemaphoreName.constData(), O_CREAT | O_CLOEXEC | O_RDWR, S_IRUSR | S_IWUSR, 0);
    activeWriters = sem_open(awSemaphoreName.constData(), O_CREAT | O_CLOEXEC | O_RDWR, S_IRUSR | S_IWUSR, 0);
    waitingReaders = sem_open(wrSemaphoreName.constData(), O_CREAT | O_CLOEXEC | O_RDWR, S_IRUSR | S_IWUSR, 0);
    waitingWriters = sem_open(wwSemaphoreName.constData(), O_CREAT | O_CLOEXEC | O_RDWR, S_IRUSR | S_IWUSR, 0);
    if (lock == SEM_FAILED) {
        qCritical() << (int) errno;
    }
    initialized = true;
}


Qs::ReadWriteLocker::ReadWriteLocker(const QByteArray& name) : d (new ReadWriteLockerPrivate)
{
    d->open(name);
}


Qs::ReadWriteLocker::~ReadWriteLocker()
{
    delete d;
}

// TODO Shall we check return value for sem_*?
inline void throwOnFailure(int returnCode) {
    if (returnCode == -1) {
        throw std::logic_error(" __PRETTY_FUNCTION__ : __LINE__ ");
    }
}

void Qs::ReadWriteLocker::lockForRead()
{
    if (!d->initialized) {
        return;
    }
    sem_wait(d->lock);
    int aw = 0;
    int ww = 0;
    sem_getvalue(d->activeWriters, &aw);
    sem_getvalue(d->waitingWriters, &ww);
    int writers = aw + ww;
    if (writers) {
        sem_post(d->waitingReaders);
        sem_post(d->lock);
        sem_wait(d->toReadSemaphore);
    } else {
        sem_post(d->activeReaders);
        sem_post(d->lock);
    }
}

void Qs::ReadWriteLocker::unlockForRead()
{
    if (!d->initialized) {
        return;
    }
    if (sem_wait(d->lock) == -1) {
        return;
    }
    sem_wait(d->activeReaders);
    int ar = 0;
    int ww = 0;
    sem_getvalue(d->activeReaders, &ar);
    sem_getvalue(d->waitingWriters, &ww);
    if (ar < 0 || ww < 0) {
        throw std::logic_error("unlockForRead: ar < 0 || ww < 0");
    }
    if (ar == 0 && ww > 0) {
        sem_post(d->toWriteSemaphore);
        sem_post(d->activeWriters);
        sem_wait(d->waitingWriters);
    }
    sem_post(d->lock);
}

void Qs::ReadWriteLocker::lockForWrite()
{
    if (!d->initialized) {
        return;
    }
    if (sem_wait(d->lock) == -1) {
        // TODO
        return;
    }
    int aw = 0;
    int ww = 0;
    int ar = 0;
    sem_getvalue(d->activeWriters, &aw);
    sem_getvalue(d->waitingWriters, &ww);
    sem_getvalue(d->activeReaders, &ar);
    if (ar < 0 || aw < 0 || ww < 0) {
        throw std::logic_error("unlockForRead: ar < 0 || ar < 0 || ww < 0");
    }
    int readers = aw + ww + ar;
    if (readers) {
        sem_post(d->waitingWriters);
        sem_post(d->lock);
        sem_wait(d->toWriteSemaphore);
    } else {
        sem_post(d->activeWriters);
        sem_post(d->lock);
    }
}

void Qs::ReadWriteLocker::unlockForWrite()
{
    if (!d->initialized) {
        return;
    }
    if (sem_wait(d->lock) == -1) {
        // TODO
        return;
    }
    sem_wait(d->activeWriters);

    int aw = 0;
    int ww = 0;
    int wr = 0;
    sem_getvalue(d->activeWriters, &aw);
    sem_getvalue(d->waitingWriters, &ww);
    sem_getvalue(d->waitingReaders, &wr);
    if (aw < 0 || wr < 0 || ww < 0) {
        throw std::logic_error("unlockForRead: ar < 0 || wr < 0 || ww < 0");
    }
    if (ww > 0) {
        sem_post(d->toWriteSemaphore);
        sem_post(d->activeWriters);
        sem_wait(d->waitingWriters);
    } else {
        while (wr > 0) {
            sem_post(d->toReadSemaphore);
            sem_post(d->activeReaders);
            sem_wait(d->waitingReaders);
            sem_getvalue(d->waitingReaders, &wr);
        }
    }
    sem_post(d->lock);
}

// --------------------------------------------

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <linux/futex.h>
#include <sys/time.h>

// Code from mampage futex(2)
static int futex(int *uaddr, int futex_op, int val,
        const struct timespec *timeout, int *uaddr2, int val3)
{
    return syscall(SYS_futex, uaddr, futex_op, val,
                    timeout, uaddr, val3);
}

static bool fwait(int *futexp)
{
    int s;
    while (true) {
        /* Is the futex available? */

        if (__sync_bool_compare_and_swap(futexp, 1, 0))
            break;      /* Yes */

        /* Futex is not available; wait */

        s = futex(futexp, FUTEX_WAIT, 0, NULL, NULL, 0);
        if (s == -1 && errno != EAGAIN) {
            return false;
        }
    }
    return true;
}

static bool fpost(int *futexp)
{
    int s;
    if (__sync_bool_compare_and_swap(futexp, 0, 1)) {

        s = futex(futexp, FUTEX_WAKE, 1, NULL, NULL, 0);
        if (s == -1) {
            return false;
        }
    }
    return true;
}


Qs::FutexReadWriteLockerPrivate::FutexReadWriteLockerPrivate()
{
}

Qs::FutexReadWriteLockerPrivate::~FutexReadWriteLockerPrivate()
{

}

void Qs::FutexReadWriteLockerPrivate::open(const QByteArray& name) {
    if (initialized) {
        return;
    }
    path = name + "_futex";
    bool needInitialize = false;
    // Check if it exists
    int fd = shm_open(path.data(), O_RDWR | O_CREAT | O_EXCL, S_IRWXU);
    if (fd == -1) {
        int fd = shm_open(path.data(), O_RDWR | O_CREAT, S_IRWXU);
    } else {
        needInitialize = true;
    }
    if (fd == -1) {
        return;
    }
    if (ftruncate(fd, sizeof(_D)) == -1) {
        ::close(fd);
        return;
    }
    d = (_D*) mmap(NULL, sizeof(_D), PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    if (d == MAP_FAILED) {
        ::close(fd);
        return;
    }
    // Do nothing though failed on  madvise or mlock
    if (madvise(d, sizeof(_D), MADV_RANDOM) == -1) {
        munmap(d, sizeof(_D));
        ::close(fd);
        return;
    }
    if (mlock(d, sizeof(_D)) == -1) {
        munmap(d, sizeof(_D));
        ::close(fd);
        return;
    }
    // TODO if no one access
    if (needInitialize) {
        d->lock = 1;
        d->toRead = 0;
        d->toWrite = 0;
        d->activeReaders = 0;
        d->activeWriters = 0;
        d->waitingReaders = 0;
        d->waitingWriters = 0;
    }
    initialized = true;
    ::close(fd);
}


Qs::FutexReadWriteLocker::FutexReadWriteLocker(const QByteArray& name) : d (new FutexReadWriteLockerPrivate)
{
    d->open(name);
}


Qs::FutexReadWriteLocker::~FutexReadWriteLocker()
{
    delete d;
}


void Qs::FutexReadWriteLocker::lockForRead()
{
    if (!d->initialized) {
        return;
    }
    fwait(&d->d->lock);
    int aw = d->d->activeWriters;
    int ww = d->d->waitingWriters;

    int writers = aw + ww;
    if (writers) {
        fpost(&d->d->waitingReaders);
        fpost(&d->d->lock);
        fwait(&d->d->toRead);
    } else {
        fpost(&d->d->activeReaders);
        fpost(&d->d->lock);
    }
}

void Qs::FutexReadWriteLocker::unlockForRead()
{
    if (!d->initialized) {
        return;
    }
    if (fwait(&d->d->lock) == -1) {
        return;
    }
    fwait(&d->d->activeReaders);
    int ar = d->d->activeReaders;
    int ww = d->d->waitingWriters;
    if (ar < 0 || ww < 0) {
        throw std::logic_error("unlockForRead: ar < 0 || ww < 0");
    }
    if (ar == 0 && ww > 0) {
        fpost(&d->d->toWrite);
        fpost(&d->d->activeWriters);
        fwait(&d->d->waitingWriters);
    }
    fpost(&d->d->lock);
}


void Qs::FutexReadWriteLocker::lockForWrite()
{
    if (!d->initialized) {
        return;
    }
    if (fwait(&d->d->lock) == -1) {
        // TODO
        return;
    }
    int aw = d->d->activeWriters;
    int ww = d->d->waitingWriters;
    int ar = d->d->activeReaders;
    if (ar < 0 || aw < 0 || ww < 0) {
        throw std::logic_error("unlockForRead: ar < 0 || ar < 0 || ww < 0");
    }
    int readers = aw + ww + ar;
    if (readers) {
        fpost(&d->d->waitingWriters);
        fpost(&d->d->lock);
        fwait(&d->d->toWrite);
    } else {
        fpost(&d->d->activeWriters);
        fpost(&d->d->lock);
    }
}

void Qs::FutexReadWriteLocker::unlockForWrite()
{
    if (!d->initialized) {
        return;
    }
    if (fwait(&d->d->lock) == -1) {
        // TODO
        return;
    }
    fwait(&d->d->activeWriters);

    int aw = d->d->activeWriters;
    int ww = d->d->waitingWriters;
    volatile int wr = d->d->waitingReaders;

    if (aw < 0 || wr < 0 || ww < 0) {
        throw std::logic_error("unlockForRead: ar < 0 || wr < 0 || ww < 0");
    }
    if (ww > 0) {
        fpost(&d->d->toWrite);
        fpost(&d->d->activeWriters);
        fwait(&d->d->waitingWriters);
    } else {
        while (wr > 0) {
            fpost(&d->d->toRead);
            fpost(&d->d->activeReaders);
            fwait(&d->d->waitingReaders);
            wr = d->d->waitingReaders;
        }
    }
    fpost(&d->d->lock);
}
