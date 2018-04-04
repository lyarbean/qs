#ifndef QS_SHMTICKINFO_H
#define QS_SHMTICKINFO_H

#include "core/datatypes.h"

// /gateway/ticker/hour/minute/{tick}*

namespace Qs {
class ShmTickInfoPrivate;
class ShmTickInfo : public TickInfo {
public:
    virtual ~ShmTickInfo();
    // ShmTickInfo(const QUuid& gateway);
    static ShmTickInfo create(const TickInfo&);
    static ShmTickInfo* create(const QUuid& gateway, const QByteArray& path, int index);
    virtual QByteArray ticker() const override;
    virtual QByteArray tickerName() const override;
    virtual QUuid gateway() const override;
    virtual double lastPrice() const override;
    virtual double preClosePrice() const override;
    virtual double openPrice() const override;
    virtual double closePrice() const override;
    virtual double highPrice() const override;
    virtual double lowPrice() const override;
    virtual double preOpenInterest() const override;
    virtual double openInterest() const override;
    virtual double preSettlementPrice() const override;
    virtual double settlementPrice() const override;
    virtual double upperLimitPrice() const override;
    virtual double lowerLimitPrice() const override;
    virtual double previousDelta() const override;
    virtual double currentDelta() const override;
    virtual quint64 volume() const override;
    virtual double turnover() const override;
    virtual double averagePrice() const override;
    virtual quint64 datetime() const override;
    virtual double bidPrice(Qs::QuoteOrderType which) const override;
    virtual quint64 bidVolume(Qs::QuoteOrderType which) const override;
    virtual double askPrice(Qs::QuoteOrderType which) const override;
    virtual quint64 askVolume(Qs::QuoteOrderType which) const override;
    virtual qint64 tradeCount() const override;
    virtual quint64 totalBidVolume() const override;
    virtual quint64 totalAskVolume() const override;
    virtual double maBidPrice() const override;
    virtual double maAskPrice() const override;
    virtual double maBondBidPrice() const override;
    virtual double maBondAskPrice() const override;
    virtual double yieldToMaturity() const override;
    virtual double iopv() const override;
    virtual quint64 etfBuyCount() const override;
    virtual quint64 etfSellCount() const override;
    virtual double etfBuyVolume() const override;
    virtual double etfBuyMoney() const override;
    virtual double etfSellVolume() const override;
    virtual double etfSellMoney() const override;
    virtual double totalWarrantExecVolume();
    virtual double warrantLowerPrice();
    virtual double warrantUpperPrice();
    virtual quint64 cancelBuyCount() const override;
    virtual quint64 cancelSellCount() const override;
    virtual double cancelBuyVolume() const override;
    virtual double cancelSellVolume() const override;
    virtual quint64 totalBuyCount() const override;
    virtual quint64 totalSellCount() const override;
    virtual quint32 durationAfterBuy() const override;
    virtual quint32 durationAfterSell() const override;
    virtual quint32 countOfBidOrders() const override;
    virtual quint32 countOfAskOrders() const override;
    virtual quint32 execTime() const override;
    virtual bool isMarketClosed() const override;
    virtual double totalPosition() const override;
    virtual double peRatio1() const override;
    virtual double peRatio2() const override;

private:
    ShmTickInfo() = delete;
    explicit ShmTickInfo(const QUuid& gateway);
    ShmTickInfo(const ShmTickInfo&) = default;

private:
    ShmTickInfoPrivate* const d;
};
}

#endif // QS_SHMTICKINFO_H
