#ifndef QS_RECORDTICKINFO_H
#define QS_RECORDTICKINFO_H
#include <QDataStream>
#include "core/datatypes.h"
namespace Qs {
struct RecordTickInfo {
    quint32 magic; // 0xABCDEF00
    QString ticker;
    QString tickerName;
    double averagePrice;
    double lastPrice;
    double preClosePrice;
    double openPrice;
    double closePrice;
    double highPrice;
    double lowPrice;
    quint64 volume;
    double turnover;
    double bidPrice[TickInfo::TenthOrder + 1];
    quint64 bidVolume[TickInfo::TenthOrder + 1];
    double askPrice[TickInfo::TenthOrder + 1];
    quint64 askVolume[TickInfo::TenthOrder + 1];
    quint64 datetime; // YYYYmmddHHMMSSsss
};

inline QDataStream& operator<<(QDataStream& stream, const RecordTickInfo& info) {
    stream << info.magic << info.ticker << info.tickerName << info.averagePrice << info.lastPrice << info.preClosePrice
           << info.openPrice << info.closePrice << info.highPrice << info.lowPrice << info.volume << info.turnover;
    for (int i = TickInfo::FirstOrder; i <= TickInfo::TenthOrder; ++i) {
        stream << info.bidPrice[i];
    }
    for (int i = TickInfo::FirstOrder; i <= TickInfo::TenthOrder; ++i) {
        stream << info.bidVolume[i];
    }
    for (int i = TickInfo::FirstOrder; i <= TickInfo::TenthOrder; ++i) {
        stream << info.askPrice[i];
    }
    for (int i = TickInfo::FirstOrder; i <= TickInfo::TenthOrder; ++i) {
        stream << info.askVolume[i];
    }
    stream << info.datetime; // YYYYmmddHHMMSSsss
}

inline QDataStream& operator>>(QDataStream& stream, RecordTickInfo& info) {
    stream >> info.magic >> info.ticker >> info.tickerName >> info.averagePrice >> info.lastPrice >>
        info.preClosePrice >> info.openPrice >> info.closePrice >> info.highPrice >> info.lowPrice >> info.volume >>
        info.turnover;
    for (int i = TickInfo::FirstOrder; i <= TickInfo::TenthOrder; ++i) {
        stream >> info.bidPrice[i];
    }
    for (int i = TickInfo::FirstOrder; i <= TickInfo::TenthOrder; ++i) {
        stream >> info.bidVolume[i];
    }
    for (int i = TickInfo::FirstOrder; i <= TickInfo::TenthOrder; ++i) {
        stream >> info.askPrice[i];
    }
    for (int i = TickInfo::FirstOrder; i <= TickInfo::TenthOrder; ++i) {
        stream >> info.askVolume[i];
    }
    stream >> info.datetime; // YYYYmmddHHMMSSsss
}
} // namespace Qs

#endif // QS_RECORDTICKINFO_H
