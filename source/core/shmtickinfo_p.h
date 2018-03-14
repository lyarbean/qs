#ifndef SHMTICKINFOPRIVATE_H
#define SHMTICKINFOPRIVATE_H
#include <QtCore>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h>    /* For O_* constants */
#include <unistd.h>
#include <cstring>
namespace Qs {

class TickInfo;
// TODO Definition of internal structure for tick info
struct QsTick {
    char ticker[16] = {0};
    char gateway[32] = {0}; // {...}
    quint64 datetime = 0;
    double bid1Price = 0.0;
};


class ShmTickInfoPrivate {
public:
    ShmTickInfoPrivate(ShmTickInfo* q);
    virtual ~ShmTickInfoPrivate();
    void set(const TickInfo& t);
    QsTick* tick();
private:
    class ShmTickInfo* const q;
    QByteArray path;
    int index = -1;
};
}

#endif // SHMTICKINFOPRIVATE_H
