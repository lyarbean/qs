#ifndef QS_DATATYPE_H
#define QS_DATATYPE_H
#include <QByteArray>
#include <QUuid>
#include <QSharedPointer>

namespace Qs {

enum QuoteOrderType {
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
    // TODO More ?
};

enum RequestStatus {
    InitialRequest = 0,
    NoSuchGateway,
    GatewayIsGone,
    AcceptedOnRiskControl,
    RejectedOnRiskControl,
    InvalidTicker,
    InvalidSide,
    Inserted,
    FailedToInsert,
    Done,
};  

enum TradeSideType {
    BuySide = 0,
    SellSide,
    BuyOpenSide,
    SellOpenSide,
    BuyCloseSide,
    SellCloseSide,
    PurchaseSide,
    RedemptionSide,
    SplitSide,
    MergeSide,
};

enum TradePriceKind {
    LimitPriceKind = 0,
    BestOrCancelPriceKind,
    Best5OrLimitPriceKind,
    Best5ORCancelPriceKind,
    AllOrCancelPriceKind,
    ForwardBestPriceKind,
    ReverseBestLimitPriceKind,
    InvalidPriceKind,
};

// TODO
enum TradeBusinessType {
    CashBusiness = 0,
    IposBusiness,
};

enum TradeType {
    CommonTrade = 0,
    OptionsExecutionTrade,
    OtcTrade,
    EtpDerivedTrade,
    CombinationDerivedTrade,
    EtfPurchase,
    EtfRedem
};

enum OrderTradeStatusType {
    InitialOrder = 0,
    AllTradedOrder,
    PartialTradedQueueing,
    PartialTradedNotQueueing,
    NotTradedQueueing,
    CanceledOrder,
    RejectedOrder,
};

enum OrderSubmitStatusType {
    OrderInsertSubmitted = 0,
    OrderInsertAccepted,
    OrderInsertRejected,
    OrderCancelSubmitted,
    OrderCancelAccepted,
    OrderCancelRejected,
};

enum OrderKind {
    NormalOrder = 0,
    DerivedFromQuoteOrder,
    DerivedFromCombinationOrder,
    CombinationOrder,
    ConditionalOrder,
    SwapOrder,
};

/**
 * @brief TickInfo represents market/option/etf tick data.
 *
 */
class TickInfo {
public:
    // A gateway is enforced
    explicit TickInfo(const QUuid& gateway) {
    }
    virtual ~TickInfo() {
    }
    virtual QByteArray ticker() const = 0;
    virtual QByteArray tickerName() const = 0;
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
    virtual double bidPrice(QuoteOrderType which) const {
        return 0;
    }
    virtual quint64 bidVolume(QuoteOrderType which) const {
        return 0;
    }
    virtual double askPrice(QuoteOrderType which) const {
        return 0;
    }
    virtual quint64 askVolume(QuoteOrderType which) const {
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
        return false;
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
    // A gateway is enforced
    explicit TradeInfo(const QUuid& gateway) {
    }
    virtual ~TradeInfo(){};
    virtual QUuid gateway() const = 0;
    virtual QByteArray ticker() const = 0;
    virtual double price() const = 0;
    virtual quint64 tradedVolume() const = 0;
    virtual double tradedAmount() const = 0; // price * tradedVolume
    virtual quint64 tradedTime() const = 0;
    virtual quint64 reportIndex() const = 0;
    virtual TradeType tradeType() const = 0;
    virtual TradeSideType tradeSideType() const = 0;
    virtual TradeBusinessType tradeBusinessType() const = 0;
};

typedef QSharedPointer<TradeInfo> TradeInfoPointer;

class OrderInfo {
public:
    virtual ~OrderInfo(){};
    virtual QByteArray ticker() const = 0;
    virtual double price() const = 0;
    virtual quint64 volume() const = 0;
    // YYYYMMDDHHMMSSzzz
    virtual quint64 insertTime() const = 0;
    virtual quint64 updateTime() const = 0;
    virtual quint64 cancelTime() const = 0;
    virtual double tradedAmount() const = 0;
    virtual TradeSideType tradeSideType() const = 0;
    virtual TradeBusinessType tradeBusinessType() const = 0;
    virtual TradePriceKind kind() const = 0;
    virtual OrderTradeStatusType tradeStatus() const = 0;
    virtual OrderSubmitStatusType submitStatus() const = 0;
};

typedef QSharedPointer<OrderInfo> OrderInfoPointer;

// Gateway should keep a table mapping from internal id to a uuid.
class OrderCancelInfo {
public:
    const QUuid& id() const {
        return uuid;
    }

private:
    QUuid uuid;
};

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

class OrderRequest {
public:
    OrderRequest(const QUuid& gateway) {
        status = InitialRequest;
        id = QUuid::createUuid();
        gatewayId = gatewayId;
    }
    RequestStatus status;
    QUuid id;
    QUuid gatewayId; // to which gateway
    QByteArray ticker;
    double price;
    quint32 volume;
    TradeSideType side;
    TradePriceKind kind;
    TradeBusinessType bussiness;
};

typedef QSharedPointer<OrderRequest> OrderRequestPointer;

class CancelOrderRequest {
public:
    explicit CancelOrderRequest(const QUuid& gateway, const QUuid& order) {
        status = InitialRequest;
        id = QUuid::createUuid();
        orderId = order;
        gatewayId = gateway;
    }
    RequestStatus status;
    QUuid id;
    QUuid orderId;
    QUuid gatewayId;
};

typedef QSharedPointer<CancelOrderRequest> CancelOrderRequestPointer;

class SubscribeRequest {
public:
    explicit SubscribeRequest(const QUuid& gateway, const QByteArrayList& tickers) {
        status = InitialRequest;
        id = QUuid::createUuid();
        gatewayId = gatewayId;
        this->tickers = tickers;
    }
    int count() const {
        return tickers.count();
    }
    QByteArrayList tickerList() const {
        return tickers;
    }
    // separator = [',', '\n',...]
    QByteArray tickersAsByteArray(const QByteArray& separator) const {
        return tickers.join(separator);
    }

private:
    RequestStatus status;
    QUuid id;
    QUuid gatewayId;
    QByteArrayList tickers;
};

typedef QSharedPointer<SubscribeRequest> SubscribeRequestPointer;
} // namespace Qs

#endif // QS_DATATYPE_H
