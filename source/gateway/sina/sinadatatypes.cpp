#include "sinadatatypes.h"
#include <QStringList>
#include <QDebug>
namespace Qs {
class SinaTickInfoPrivate {
public:
    void parse(const QString& source);
    QByteArray ticker;
    QByteArray tickerName;
    double averagePrice;
    double lastPrice;
    double preClosePrice;
    double openPrice;
    double closePrice;
    double highPrice;
    double lowPrice;
    quint64 volume;
    double turnover;
    double bidPrice[TenthOrder + 1] = {0.0};
    quint64 bidVolume[TenthOrder + 1] = {0};
    double askPrice[TenthOrder + 1] = {0.0};
    quint64 askVolume[TenthOrder + 1] = {0};
    quint64 datetime; // YYYYmmddHHMMSSsss
    QUuid gateway;
    //     qint32 date; // YYYYMMDD
    //     qint32 day;
    //     qint32 hour;
    //     qint32 minute;
    //     qint32 second;
    //     qint32 millisecond;
    //     quint64 msecsSinceEpoch;
};

void SinaTickInfoPrivate::parse(const QString& source) {
    QStringList items = source.split(',');
    if (items.count() != 33) { // TODO
        // qCritical() << items.count();
        return;
    }
    // 0
    ticker = items.value(0).mid(11, 8).toUtf8();
    tickerName = items.value(0).mid(21).toUtf8();
    // 1
    openPrice = items.value(1).toDouble();
    // 2
    closePrice = items.value(2).toDouble();
    // 3
    lastPrice = items.value(3).toDouble();
    // 4
    highPrice = items.value(4).toDouble();
    // 5
    lowPrice = items.value(5).toDouble();
    // 6
    // bidPrice1
    // 7
    // askPrice1
    // 8
    volume = items.value(8).toULongLong();
    // 9
    turnover = items.value(9).toDouble();
    // 10
    for (int i = 0; i < 5; ++i) {
        bidVolume[i] = items.value(10 + i * 2).toULongLong();
        bidPrice[i] = items.value(10 + i * 2 + 1).toDouble();
    }
    for (int i = 0; i < 5; ++i) {
        askVolume[i] = items.value(20 + i * 2).toULongLong();
        askPrice[i] = items.value(20 + i * 2 + 1).toDouble();
    }
    // 30
    auto day = items.value(30);
    auto time = items.value(30);
    day.replace('-', "");
    time.replace(':', "");
    day += time + "000";
    datetime = day.toULongLong();
}

SinaTickInfo::SinaTickInfo(const QString& source, const QUuid& gateway)
  : TickInfo(gateway), d(new SinaTickInfoPrivate) {
    d->parse(source);
    d->gateway = gateway;
}
SinaTickInfo::~SinaTickInfo() {
    delete d;
}

QByteArray SinaTickInfo::ticker() const {
    return d->ticker;
}

QByteArray SinaTickInfo::tickerName() const {
    return d->tickerName;
}

QUuid SinaTickInfo::gateway() const {
    return d->gateway;
}
double SinaTickInfo::averagePrice() const {
    return d->averagePrice;
}

double SinaTickInfo::lastPrice() const {
    return d->lastPrice;
}

double SinaTickInfo::preClosePrice() const {
    return d->preClosePrice;
}

double SinaTickInfo::openPrice() const {
    return d->openPrice;
}

double SinaTickInfo::closePrice() const {
    return d->closePrice;
}

double SinaTickInfo::highPrice() const {
    return d->highPrice;
}

double SinaTickInfo::lowPrice() const {
    return d->lowPrice;
}

quint64 SinaTickInfo::volume() const {
    return d->volume;
}

double SinaTickInfo::turnover() const {
    return d->turnover;
}

double SinaTickInfo::bidPrice(QuoteOrderType which) const {
    return d->bidPrice[which];
}

quint64 SinaTickInfo::bidVolume(QuoteOrderType which) const {
    return d->bidVolume[which];
}

double SinaTickInfo::askPrice(QuoteOrderType which) const {
    return d->askPrice[which];
}

quint64 SinaTickInfo::askVolume(QuoteOrderType which) const {
    return d->askVolume[which];
}

// TODO
quint64 SinaTickInfo::datetime() const {
    return d->datetime;
} // YYYYMMDD

} // namespace Qs
