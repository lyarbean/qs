#include "xtpgateway.h"
#include "xtpgateway_p.h"
#include "xtpdatatypes.h"

#include <cstring>
#include <QCoreApplication>
#include <QTextCodec>
#include <QDebug>
namespace Qs {

XtpGateway::XtpGateway()
  : d(new XtpGatewayPrivate(this)) {
}

XtpGateway::~XtpGateway() {
    delete d;
}

void XtpGateway::connectToServer() {
    // Quote
    d->quote->start();
    d->trader->start();
}

void XtpGateway::disconnectFromServer() {
    d->quote->stop();
    d->trader->stop();
}

// ticker start with sh or sz

static XTP_EXCHANGE_TYPE exchangeOf(const QByteArray& ticker) {
    QByteArray prefix = ticker.left(2);
    if (prefix == "sh") {
        return XTP_EXCHANGE_SH;
    } else if (prefix == "sz") {
        return XTP_EXCHANGE_SH;
    }
    return XTP_EXCHANGE_UNKNOWN;
}

static QByteArray codeOf(const QByteArray& ticker) {
    return ticker.mid(2);
}

void XtpGateway::subscribe(SubscribeRequestPointer& request) {
    QByteArrayList tickerList = request->tickerList();
    QMap<XTP_EXCHANGE_TYPE, QByteArrayList> codes;
    for (auto& t : tickerList) {
        XTP_EXCHANGE_TYPE e = exchangeOf(t);
        codes[e].append(codeOf(t));
    }
    for (auto p = codes.begin(); p != codes.end(); ++p) {
        if (p.key() == XTP_EXCHANGE_UNKNOWN) {
            // TODO issue warning
            continue;
        }
        d->quote->api->SubscribeMarketData((char**)p.value().join('\0').constData(), p.value().count(), p.key());
    }
}

// using a map to store CancelOrderRequestPointer
static uint64_t xtpIdOfCancelOrder(CancelOrderRequestPointer& request) {

    return 0;
}

/*
static uint64_t xtpIdOfOrder(OrderRequestPointer& request) {

    return 0;
}*/

void XtpGateway::cancelOrder(CancelOrderRequestPointer& request) {
    d->trader->api->CancelOrder(xtpIdOfCancelOrder(request), d->trader->session);
}

void XtpGateway::queryAccount() {
    d->trader->api->QueryAsset(d->trader->session, 0);
}

void XtpGateway::queryPosition() {
    d->trader->api->QueryPosition("", d->trader->session, 0);
}

void XtpGateway::sendOrder(OrderRequestPointer& request) {
    // TODO fill info
    XTPOrderInsertInfo info;
    memset(info.ticker, '\0', XTP_TICKER_LEN);
    strcpy(info.ticker, codeOf(request->ticker).constData());
    XTP_EXCHANGE_TYPE e = exchangeOf(request->ticker);
    if (e == XTP_EXCHANGE_SZ) {
        info.market = XTP_MKT_SZ_A;
    } else if (e == XTP_EXCHANGE_SH) {
        info.market = XTP_MKT_SH_A;
    } else {
        qCritical() << __PRETTY_FUNCTION__ << "Unknown market for" << request->ticker;
        request->status = InvalidTicker;
        return;
    }
    info.price = request->price;
    info.quantity = request->volume;
    if (request->side == BuySide) {
        info.side = XTP_SIDE_BUY;
    } else if (request->side == SellSide) {
        info.side = XTP_SIDE_SELL;
    } else {
        qCritical() << __PRETTY_FUNCTION__ << "Unknown side for" << request->ticker;
        request->status = InvalidSide;
        return;
    }
    info.price_type = XTP_PRICE_LIMIT;           // request.kind
    info.business_type = XTP_BUSINESS_TYPE_CASH; // request.bussiness
    // TODO add to map: xtpId -> OrderRequestPointer
    uint64_t xtpId = d->trader->api->InsertOrder(&info, d->trader->session);
    request->status = Inserted;
}

const QUuid& XtpGateway::uuid() {
    if (d->uuid.isNull()) {
        d->uuid = QUuid::createUuid();
    }
    return d->uuid;
}

} // namespace Qs
