#ifndef OZ_DATATYPE_H
#define OZ_DATATYPE_H
#include <QString>
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
    virtual double averagePrice() const = 0;
    virtual double lastPrice() const = 0;
    virtual double preClosePrice() const = 0;
    virtual double openPrice() const = 0;
    virtual double closePrice() const = 0;
    virtual double highPrice() const = 0;
    virtual double lowPrice() const = 0;
    virtual quint64 volume() const = 0;
    virtual double turnover() const = 0;
    virtual double bidPrice(OrderSequenceType which) const = 0;
    virtual quint64 bidVolume(OrderSequenceType which) const = 0;
    virtual double askPrice(OrderSequenceType which) const = 0;
    virtual quint64 askVolume(OrderSequenceType which) const = 0;
    virtual quint64 datetime() const = 0;
    //     virtual qint32 date() const = 0; // YYYYMMDD
    //     virtual qint32 day() const = 0;
    //     virtual qint32 hour() const = 0;
    //     virtual qint32 minute() const = 0;
    //     virtual qint32 second() const = 0;
    //     virtual qint32 millisecond() const = 0;
    //     virtual quint64 msecsSinceEpoch() const = 0;
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

#endif // OZ_DATATYPE_H
