#ifndef SINASTRATEGYPRIVATE_H
#define SINASTRATEGYPRIVATE_H
#include <QList>
#include <QMap>
#include <QVector>
#include "core/datatypes.h"
namespace Qs {
const double OneTick = 0.01;
struct SubTickInfo {
    double turnover;
    quint64 volume;
    double askPrice;
    double bidPrice;
    quint64 lastAskVolume;
    quint64 lastBidVolume;
};

struct SinaStrategyPart {
    QVector<SubTickInfo> subTickInfos;
    quint64 bigVolume = 2000;
    double boughtPrice = 23.6;
    double average = 0;
    double averageBSDiff = 0;
    double p = 0;
    double p2 = 0;
    double THBSDiff = 0.6;
    double THCancelBSDiff = 0;
    double THLongOpen = 0.9;
    double StopLoss = 0.003;
    double StopLoss2 = 0.01;
    double THLongPC = -0.2;
    double THLongPCCancel = 0.2;
    double THLongCancel = 0.2;
    double THQPC = -0.6;
    double THOpenExpProfit = 0.05;
    double THCancelExpProfit = 0.04;
    double THDownside = -0.01;
    double THTurnoverRatio = 1.5;
    qint32 TurnoverRatioN = 100;
    qint32 TurnoverRatioM = 20;
    qint32 DownsideN = 200;
    TickInfo::OrderSequenceType indexBid;
    bool hasP3 = false;
};

class SinaStrategyPrivate {
public:
    SinaStrategyPrivate(SinaStrategy* q);
    virtual ~SinaStrategyPrivate();
    double downside(quint32 n, const QString& ticker) const;
    double averageBidAskDiff(quint32 count, const QString& ticker) const;
    double turnoverRatio(quint32 n, quint32 m, const QString& ticker) const;
    void getVolumes(quint64& askVolume, quint64& bidVolume, const QString& ticker) const;

    void prepareToExit(const SinaStrategyPart& part, TickInfoPointer& info);
    void cancelBidOrders(const SinaStrategyPart& part, TickInfoPointer& info);
    void cancelAskOrders(const SinaStrategyPart& part, TickInfoPointer& info);
    void applyAskOrders(const SinaStrategyPart& part, TickInfoPointer& info);
    void applyBidOrders(const SinaStrategyPart& part, TickInfoPointer& info);

    class SinaStrategy* const q;
    QMap<QString, SinaStrategyPart> parts;

    // TODO group by ticker
    // struct Parameter { ... };
};
} // namespace Qs

#endif // SINASTRATEGYPRIVATE_H
