#ifndef QS_DATATYPE_H
#define QS_DATATYPE_H
#include <QString>
#include <QUuid>
#include <QSharedPointer>

namespace Qs {
/**
 * @brief TickInfo represents market or option tick data.
 *
 */
class TickInfo {
public:
    enum OrderSequenceType {
        InvalidOrder = -1,
        FirstOrder = 0,
        SecondOrder,
        ThirdOrder,
        FourthOrder,
        FifthOrder,
        SixthOrder,
        SeventhOrder,
        EighthOrder,
        NinethOrder,
        TenthOrder
    };
    virtual ~TickInfo() {
    }
    virtual QString ticker() const = 0;
    virtual QString tickerName() const = 0;
    virtual QUuid gateway() const = 0;
    // Stock
    virtual double lastPrice() const {
        return 0;
    }
    virtual double preClosePrice() const {
        return 0;
    }
    virtual double openPrice() const {
        return 0;
    }
    virtual double closePrice() const {
        return 0;
    }
    virtual double highPrice() const {
        return 0;
    }
    virtual double lowPrice() const {
        return 0;
    }
    // Option
    virtual double preOpenInterest() const {
        return 0;
    }
    virtual double openInterest() const {
        return 0;
    }
    virtual double preSettlementPrice() const {
        return 0;
    }
    virtual double settlementPrice() const {
        return 0;
    }

    // Common
    virtual double upperLimitPrice() const {
        return 0;
    }
    virtual double lowerLimitPrice() const {
        return 0;
    }
    virtual double previousDelta() const {
        return 0;
    }
    virtual double currentDelta() const {
        return 0;
    }
    virtual quint64 volume() const {
        return 0;
    }
    virtual double turnover() const {
        return 0;
    }
    virtual double averagePrice() const {
        return 0;
    }
    virtual quint64 datetime() const {
        return 0;
    } // YYYYmmddHHMMSSsss

    // orders
    virtual double bidPrice(OrderSequenceType which) const {
        return 0;
    }
    virtual quint64 bidVolume(OrderSequenceType which) const {
        return 0;
    }
    virtual double askPrice(OrderSequenceType which) const {
        return 0;
    }
    virtual quint64 askVolume(OrderSequenceType which) const {
        return 0;
    }

    // ETF
    // NOTE see XTP
    virtual qint64 tradeCount() const {
        return 0;
    }
    // tickerStatus
    virtual quint64 totalBidVolume() const {
        return 0;
    }
    virtual quint64 totalAskVolume() const {
        return 0;
    }
    virtual double maBidPrice() const {
        return 0;
    }
    virtual double maAskPrice() const {
        return 0;
    }
    virtual double maBondBidPrice() const {
        return 0;
    }
    virtual double maBondAskPrice() const {
        return 0;
    }
    virtual double yieldToMaturity() const {
        return 0;
    }
    virtual double iopv() const {
        return 0;
    }
    virtual quint64 etfBuyCount() const {
        return 0;
    }
    virtual quint64 etfSellCount() const {
        return 0;
    }
    virtual double etfBuyVolume() const {
        return 0;
    }
    virtual double etfBuyMoney() const {
        return 0;
    }
    virtual double etfSellVolume() const {
        return 0;
    }
    virtual double etfSellMoney() const {
        return 0;
    }
    virtual double totalWarrantExecVolume() {
        return 0;
    }
    virtual double warrantLowerPrice() {
        return 0;
    }
    virtual double warrantUpperPrice() {
        return 0;
    }
    virtual quint64 cancelBuyCount() const {
        return 0;
    }
    virtual quint64 cancelSellCount() const {
        return 0;
    }
    virtual double cancelBuyVolume() const {
        return 0;
    }
    virtual double cancelSellVolume() const {
        return 0;
    }
    virtual quint64 totalBuyCount() const {
        return 0;
    }
    virtual quint64 totalSellCount() const {
        return 0;
    }
    virtual quint32 durationAfterBuy() const {
        return 0;
    }
    virtual quint32 durationAfterSell() const {
        return 0;
    }
    virtual quint32 countOfBidOrders() const {
        return 0;
    }
    virtual quint32 countOfAskOrders() const {
        return 0;
    }
    virtual quint32 execTime() const {
        return 0;
    } // UA3113
    virtual bool isMarketClosed() const {
        return 0;
    } // UA103/UA104
    virtual double totalPosition() const {
        return 0;
    }
    virtual double peRatio1() const {
        return 0;
    }
    virtual double peRatio2() const {
        return 0;
    }
};

typedef QSharedPointer<TickInfo> TickInfoPointer;

class TradeInfo {
public:
    virtual ~TradeInfo(){};
    virtual QString ticker() const = 0;
};

typedef QSharedPointer<TradeInfo> TradeInfoPointer;

class OrderInfo {
public:
    virtual ~OrderInfo(){};
    virtual QString ticker() const = 0;
};

typedef QSharedPointer<OrderInfo> OrderInfoPointer;

class AccountInfo {
public:
    virtual ~AccountInfo(){};
};

typedef QSharedPointer<AccountInfo> AccountInfoPointer;

class PoisitionInfo {
public:
    virtual ~PoisitionInfo(){};
};

typedef QSharedPointer<PoisitionInfo> PoisitionInfoPointer;

class LogInfo {
public:
    virtual ~LogInfo(){};
};

typedef QSharedPointer<LogInfo> LogInfoPointer;

class ContractInfo {
public:
    virtual ~ContractInfo(){};
};

typedef QSharedPointer<ContractInfo> ContractInfoPointer;

// TODO Requests are issue by engines, we may fix their structures
class OrderRequest {
public:
    virtual ~OrderRequest(){};
};

typedef QSharedPointer<OrderRequest> OrderRequestPointer;

class CancelOrderRequest {
public:
    virtual ~CancelOrderRequest(){};
};

typedef QSharedPointer<CancelOrderRequest> CancelOrderRequestPointer;

class SubscribeRequest {
public:
    virtual ~SubscribeRequest(){};
    virtual QString ticker() const = 0;
};

typedef QSharedPointer<SubscribeRequest> SubscribeRequestPointer;
} // namespace Qs

#endif // QS_DATATYPE_H
