#ifndef RECORDSTRATEGYPRIVATE_H
#define RECORDSTRATEGYPRIVATE_H
#include "recordtickinfo.h"
#include <QMap>
#include <QFile>
#include <QUuid>
namespace Qs {
class EngineAbstract;
class RecordStrategy;

class RecordStrategyPrivate {
public:
    RecordStrategyPrivate(RecordStrategy* q);
    virtual ~RecordStrategyPrivate();
    RecordStrategy* const q;
    EngineAbstract* engine;
    QMap<QString, double> bidPrice;
    QMap<QString, double> askPrice;
    QMap<QString, RecordTickInfo> ticks;
    QUuid uuid;
    QFile file;
    QDataStream stream;
};
} // namespace Qs

#endif // RECORDSTRATEGYPRIVATE_H
