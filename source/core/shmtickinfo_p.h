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
// TODO distinguish Stock Option and ETF and use union to merge them

struct QsTick {
    char ticker[16] = {0};
    char gateway[40] = {0}; // {00000000-0000-0000-0000-000000000000}
    quint64 datetime = 0;
    double upperLimitPrice = 0.0;
    double lowerLimitPrice = 0.0;
    quint64 volume = 0;
    double turnover = 0.0;
    double averagePrice = 0.0;
    union D {
        double dummy[10] = {0.0};
        struct Stock {
            double lastPrice;
            double preClosePrice;
            double openPrice;
            double  closePrice;
            double  highPrice;
            double  lowPrice;
        } stock;
        struct Option {

        } option;
        struct Futures {
            double preOpenInterest;
            double openInterest;
            double preSettlementPrice;
            double settlementPrice;
            double previousDelta;
            double currentDelta;
        };
        struct Etf {
        } etf;
    } d;
    // Order book
    double bidPrice[10] = {0.0};
    quint64 bidVolume[10] = {0};
    double askPrice[10] = {0.0};
    quint64 askVolume[10] = {0};
};

class ShmTickInfo;
class ShmTickInfoPrivate {
public:
    ShmTickInfoPrivate(ShmTickInfo* q);
    virtual ~ShmTickInfoPrivate();
    void set(const TickInfo& t);
    QsTick* tick();
private:
    ShmTickInfo* const q;
    QByteArray path;
    int index = -1;
};
}

#endif // SHMTICKINFOPRIVATE_H
