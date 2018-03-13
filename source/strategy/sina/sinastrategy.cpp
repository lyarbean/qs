#include "sinastrategy.h"
#include "sinastrategy_p.h"
#include "core/datatypes.h"
#include <cmath>
#include <QDebug>
#include "utils/utils.h"
namespace Qs {
static double getP(Qs::TickInfoPointer& tick) {
    if (tick->bidVolume(FirstOrder) == 0 || tick->askVolume(FirstOrder) == 0) {
        throw std::logic_error("Zero Volume");
    }

    double buy = 0.0;
    double sell = 0.0;
    // CALCULATE_CLOSE:
    double close = tick->averagePrice(); // (ask(0).price + bid(0).price) / 2;
                                         // CALCULATE_BUY:
    double buyWeight = 0.0;
    for (int i = 0; i < 5; ++i) {
        buyWeight += 1.0 / (tick->bidPrice(static_cast<QuoteOrderType>(i)) - close);
    }
    for (int i = 0; i < 5; ++i) {
        buy += (tick->bidVolume(static_cast<QuoteOrderType>(i)) /
                (tick->bidPrice(static_cast<QuoteOrderType>(i)) - close));
    }
    buy /= buyWeight;

    // CALCULATE_SELL:
    double sellWeight = 0.0;
    for (int i = 0; i < 5; ++i) {
        buyWeight += 1.0 / (tick->askPrice(static_cast<QuoteOrderType>(i)) - close);
    }
    for (int i = 0; i < 5; ++i) {
        buy += (tick->askVolume(static_cast<QuoteOrderType>(i)) /
                (tick->askPrice(static_cast<QuoteOrderType>(i)) - close));
    }
    sell /= sellWeight;
    return log(buy) - log(sell);
}

SinaStrategyPrivate::SinaStrategyPrivate(SinaStrategy* q)
  : q(q) {
}
SinaStrategyPrivate::~SinaStrategyPrivate() {
}

double SinaStrategyPrivate::downside(quint32 n, const QString& ticker) const {
    auto& subTickInfo = parts[ticker].subTickInfos;
    auto s = subTickInfo.size();
    if (s <= n) {
        throw std::logic_error("No enough ticks");
    }
    return (subTickInfo[s - 1].bidPrice - subTickInfo[s - 1 - n].bidPrice) / subTickInfo[s - 1 - n].bidPrice;
}

double SinaStrategyPrivate::averageBidAskDiff(quint32 count, const QString& ticker) const {
    if (!count) {
        throw std::logic_error("averageBidAskDiff: count is 0");
    }
    auto& subTickInfo = parts[ticker].subTickInfos;
    auto s = subTickInfo.size();
    if (count >= s) {
        qCritical() << count << s;
        throw std::logic_error("averageBidAskDiff: No adequate ticks");
    }
    double bid = 0.0, ask = 0.0;
    /**
        auto iter = subTickInfo.rbegin();
        for (size_t p = 0; p < count; ++p) {
            bid += (*iter).lastBidVolume;
            ask += (*iter).lastAskVolume;
            ++iter;
        }
    **/
    int c = subTickInfo.count();
    for (size_t p = 0; p < count; ++p) {
        bid += subTickInfo[c - 1 - p].lastBidVolume;
        ask += subTickInfo[c - 1 - p].lastAskVolume;
    }
    if (equalTo(bid + ask, 0.0)) {
        return 0.0;
        // throw std::logic_error("averageBidAskDiff: bid + ask == 0");
    }
    return (bid - ask) / (bid + ask);
}

double SinaStrategyPrivate::turnoverRatio(quint32 n, quint32 m, const QString& ticker) const {
    if (!(n && m)) {
        return 0.0;
    }
    auto& subTickInfos = parts[ticker].subTickInfos;
    auto s = subTickInfos.size();
    if (n >= s || m >= s) {
        throw std::logic_error("No adequate ticks");
    }
    double turnoverN = 0.0;
    double turnoverM = 0.0;
    double t = static_cast<double>(n) / static_cast<double>(m);

    auto& subTickInfo = subTickInfos.last();
    auto& subTickInfoM = subTickInfos[s - m - 1];
    auto& subTickInfoN = subTickInfos[s - n - 1];

    turnoverN = subTickInfo.turnover - subTickInfoN.turnover;
    turnoverM = subTickInfo.turnover - subTickInfoM.turnover;
    if (lessThanOrEqualTo(turnoverN, 1E-6)) {
        throw std::logic_error("No trade happens " + std::to_string(turnoverN));
    }
    return turnoverM * t / turnoverN;
}

void SinaStrategyPrivate::getVolumes(quint64& askVolume, quint64& bidVolume, const QString& ticker) const {
    askVolume = 0;
    bidVolume = 0;
    auto& subTickInfo = parts[ticker].subTickInfos;
    auto s = subTickInfo.size();
    if (s < 2) {
        return;
    }
    auto& currentTick = subTickInfo.at(s - 1);
    auto& previousTick = subTickInfo.at(s - 2);
    double turnover = currentTick.turnover - previousTick.turnover;
    double volume = currentTick.volume - previousTick.volume;

    if (lessThanOrEqualTo(volume, 1.0)) {
        return;
    }

    double average = turnover / volume;
    double topAskPrice = previousTick.askPrice;
    double topBidPrice = previousTick.bidPrice;

    if (greaterThanOrEqualTo(average, topAskPrice)) {
        askVolume = 0;
        bidVolume = static_cast<quint64>(volume);
    } else if (lessThanOrEqualTo(average, topBidPrice)) {
        bidVolume = 0;
        askVolume = static_cast<quint64>(volume);
    } else {
        askVolume = static_cast<quint64>((topAskPrice * volume - turnover) / (topAskPrice - topBidPrice));
        bidVolume = static_cast<quint64>(volume) - bidVolume;
    }
}

void SinaStrategyPrivate::prepareToExit(const SinaStrategyPart& part, TickInfoPointer& info) {
}

void SinaStrategyPrivate::cancelBidOrders(const SinaStrategyPart& part, TickInfoPointer& info) {
    if (part.indexBid != InvalidOrder) {
        double p1 = info->bidPrice(part.indexBid);
        bool a1 = lessThan(part.p, part.THLongCancel);
        bool a2 = lessThan(part.averageBSDiff, part.THCancelBSDiff);
        bool a3 = lessThan((part.p2 - p1 - OneTick * 2) / part.average, part.THCancelExpProfit);
        bool toCancel = a1 || a2 || a3;
        if (toCancel) {
            qCritical() << "Warning: cancelBidOrders " << info->tickerName() << a1 << a2 << a3;
        }
    }
}

void SinaStrategyPrivate::cancelAskOrders(const SinaStrategyPart& part, TickInfoPointer& info) {
}

void SinaStrategyPrivate::applyAskOrders(const SinaStrategyPart& part, TickInfoPointer& info) {
}

void SinaStrategyPrivate::applyBidOrders(const SinaStrategyPart& part, TickInfoPointer& info) {
    // TODO if not isBuyHoldAvailable return
    auto ticker = info->ticker();
    if (part.indexBid != InvalidOrder
        // && period_ == InTrade
        && lessThan(downside(part.DownsideN, ticker), part.THDownside) &&
        greaterThan(turnoverRatio(part.TurnoverRatioN, part.TurnoverRatioM, ticker), part.THTurnoverRatio) &&
        greaterThan((info->bidPrice(FirstOrder) - info->openPrice()) / info->openPrice(), -0.08)) {
        double p1 = info->bidPrice(part.indexBid);
        if (lessThan(part.THLongOpen, part.p) && greaterThanOrEqualTo(part.averageBSDiff, part.THBSDiff) &&
            greaterThanOrEqualTo((part.p2 - p1 - OneTick * 2) / part.average, part.THOpenExpProfit)) {
            // send bid order with price p1 + OneTick;
            qCritical() << "Warning: To Bid " << p1 + OneTick;
        }
    }
}

SinaStrategy::SinaStrategy()
  : d(new SinaStrategyPrivate(this)) {
}

void SinaStrategy::onTick(TickInfoPointer& info) {
    // //     qCritical() << "..";
    QString ticker = info->ticker();
    //     if (ticker == "sz002405") {
    //         auto price = info->bidPrice(FirstOrder);
    //         if (price < 26.398 * 0.92) {
    //             qCritical() << "Warning: " << info->tickerName() << "Low price: " << price;
    //         } else if (price > 26.398 * 1.06) {
    //             qCritical() << "Warning: " << info->tickerName() << "High price: " << price;
    //         }
    //     }
    //     if (ticker == "sz002268") { // 23.362
    //         auto price = info->bidPrice(FirstOrder);
    //         if (price < 23.362 * 0.92) {
    //             qCritical() << "Warning: " << info->tickerName() << "Low price: " << price;
    //         } else if (price > 23.362 * 1.06) {
    //             qCritical() << "Warning: " << info->tickerName() << "High price: " << price;
    //         }
    //     }
    auto& part = d->parts[ticker];
    QVector<SubTickInfo>& subTickInfos = part.subTickInfos;
    subTickInfos.append({info->turnover(), info->volume(), info->askPrice(FirstOrder), info->bidPrice(FirstOrder)});
    quint64 bid, ask;
    d->getVolumes(ask, bid, ticker);
    auto& currentTick = subTickInfos.last();
    currentTick.lastAskVolume = ask;
    currentTick.lastBidVolume = bid;

    if (subTickInfos.count() < part.TurnoverRatioN + 3) {
        return;
    }

    part.indexBid = InvalidOrder;
    QuoteOrderType indexAsk = FifthOrder; // Ask5
    part.hasP3 = false;
    for (size_t i = 0; i < 5; ++i) {
        if (info->bidVolume(FirstOrder) >= part.bigVolume) {
            part.indexBid = static_cast<QuoteOrderType>(i);
            break;
        }
    }
    for (size_t i = 0; i < 5; ++i) {
        if (info->askVolume(FirstOrder) >= part.bigVolume) {
            indexAsk = static_cast<QuoteOrderType>(i);
            ;
            part.hasP3 = true;
            break;
        }
    }
    const qint32 NBSDiff = 10;
    part.p = getP(info);
    part.averageBSDiff = d->averageBidAskDiff(NBSDiff, ticker);
    part.average = info->averagePrice();
    if (part.hasP3) {
        part.p2 = info->bidPrice(indexAsk); // book_->askQuotes()[indexAsk].price;
    } else {
        part.p2 = info->bidPrice(indexAsk) * 1.01;
        // p2_ = book_->bidQuotes()(0).price * 1.01; // TODO parameterize
    }

    // TODO subroutine
    d->prepareToExit(part, info);
    d->cancelBidOrders(part, info);
    d->cancelAskOrders(part, info);
    d->applyAskOrders(part, info);
    d->applyBidOrders(part, info);
}
void SinaStrategy::onTrade(TradeInfoPointer& info) {
}
void SinaStrategy::onOrder(OrderInfoPointer& info) {
}

const QUuid& SinaStrategy::uuid() {
    if (d->uuid.isNull()) {
        d->uuid = QUuid::createUuid();
    }
    return d->uuid;
}

void SinaStrategy::acceptGateway(const QUuid& gatewayId) {
    d->gateway = gatewayId;
}

} // namespace Qs
