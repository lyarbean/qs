#include "recordstrategy.h"
#include "recordstrategy_p.h"
#include "core/datatypes.h"
#include <QDebug>

using namespace Qs;

RecordStrategyPrivate::RecordStrategyPrivate(RecordStrategy* q)
  : q(q) {
    file.setFileName("/tmp/sina.data");
    if (file.open(QIODevice::WriteOnly | QIODevice::Unbuffered)) {
        stream.setDevice(&file);
        stream.setVersion(QDataStream::Qt_5_5);
    }
}
RecordStrategyPrivate::~RecordStrategyPrivate() {
}

RecordStrategy::RecordStrategy()
  : d(new RecordStrategyPrivate(this)) {
}

void RecordStrategy::onTick(TickInfoPointer& info) {
    //     auto ask = d->askPrice[info->ticker()];
    //     auto bid = d->bidPrice[info->ticker()];
    //     if (ask != info->askPrice(FirstOrder) || bid != info->bidPrice(FirstOrder)) {
    //         qCritical() previous->ticker() previous->tickerName() previous->bidPrice(FirstOrder)
    //         previous->askPrice(FirstOrder);
    //         d->askPrice[info->ticker()] = info->askPrice(FirstOrder);
    //         d->bidPrice[info->ticker()] = info->bidPrice(FirstOrder);
    //     }
    if (!d->stream.device()) {
        return;
    }
    RecordTickInfo& previous = d->ticks[info->ticker()];
    // TODO skip if no change
    previous.magic = 0xABCDEF00;
    previous.ticker = info->ticker();
    previous.tickerName = info->tickerName();
    previous.averagePrice = info->averagePrice();
    previous.lastPrice = info->lastPrice();
    previous.preClosePrice = info->preClosePrice();
    previous.openPrice = info->openPrice();
    previous.closePrice = info->closePrice();
    previous.highPrice = info->highPrice();
    previous.lowPrice = info->lowPrice();
    previous.volume = info->volume();
    previous.turnover = info->turnover();
    for (int i = FirstOrder; i <= TenthOrder; ++i) {
        previous.bidPrice[i] = info->bidPrice(static_cast<QuoteOrderType>(i));
    }
    for (int i = FirstOrder; i <= TenthOrder; ++i) {
        previous.bidVolume[i] = info->bidVolume(static_cast<QuoteOrderType>(i));
    }
    for (int i = FirstOrder; i <= TenthOrder; ++i) {
        previous.askPrice[i] = info->askPrice(static_cast<QuoteOrderType>(i));
    }
    for (int i = FirstOrder; i <= TenthOrder; ++i) {
        previous.askVolume[i] = info->askVolume(static_cast<QuoteOrderType>(i));
    }
    previous.datetime = info->datetime(); // YYYYmmddHHMMSSsss
    d->stream << previous;
}

void RecordStrategy::onTrade(TradeInfoPointer& info) {
}
void RecordStrategy::onOrder(OrderInfoPointer& info) {
}

const QUuid& RecordStrategy::uuid() {
    if (d->uuid.isNull()) {
        d->uuid = QUuid::createUuid();
    }
    return d->uuid;
}

void RecordStrategy::acceptGateway(const QUuid& gatewayId) {
}
