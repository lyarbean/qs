#include "xtpdatatypes.h"
#include <QStringList>
#include <QDebug>
#include "XTP_API/bin/include/xtp_api_struct.h"

namespace Qs {
class XtpTickInfoPrivate {
public:
    QUuid gateway;
    struct XTPMarketDataStruct d;
};

XtpTickInfo::XtpTickInfo(const struct XTPMarketDataStruct& source, const QUuid& gateway)
  : TickInfo(gateway)
  , d(new XtpTickInfoPrivate) {
    d->d = source;
    d->gateway = gateway;
}
XtpTickInfo::~XtpTickInfo() {
    delete d;
}

QByteArray XtpTickInfo::ticker() const {
    return d->d.ticker;
}

QByteArray XtpTickInfo::tickerName() const {
    // TODO via static table
    return "";
}

QUuid XtpTickInfo::gateway() const {
    return d->gateway;
}
double XtpTickInfo::averagePrice() const {
    return d->d.avg_price;
}

double XtpTickInfo::lastPrice() const {
    return d->d.last_price;
}

double XtpTickInfo::preClosePrice() const {
    return d->d.pre_close_price;
}

double XtpTickInfo::openPrice() const {
    return d->d.open_price;
}

double XtpTickInfo::closePrice() const {
    return d->d.close_price;
}

double XtpTickInfo::highPrice() const {
    return d->d.high_price;
}

double XtpTickInfo::lowPrice() const {
    return d->d.low_price;
}

quint64 XtpTickInfo::volume() const {
    return d->d.qty;
}

double XtpTickInfo::turnover() const {
    return d->d.turnover;
}

double XtpTickInfo::bidPrice(QuoteOrderType which) const {
    // TODO constraint
    return d->d.bid[which];
}

quint64 XtpTickInfo::bidVolume(QuoteOrderType which) const {
    // TODO constraint
    return d->d.bid_qty[which];
}

double XtpTickInfo::askPrice(QuoteOrderType which) const {
    // TODO constraint
    return d->d.ask[which];
}

quint64 XtpTickInfo::askVolume(QuoteOrderType which) const {
    // TODO constraint
    return d->d.ask_qty[which];
}

// TODO
quint64 XtpTickInfo::datetime() const {
    return d->d.data_time;
} // YYYYMMDD

} // namespace Qs
