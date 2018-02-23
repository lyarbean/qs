#ifndef QS_SINA_DATATYPE_H
#define QS_SINA_DATATYPE_H
#include "core/datatypes.h"

namespace Qs {
class SinaTickInfoPrivate;
class SinaTickInfo : public TickInfo {
public:
    explicit SinaTickInfo(const QString& source, const QUuid& gateway);
    ~SinaTickInfo();
    virtual QByteArray ticker() const override;
    virtual QByteArray tickerName() const override;
    virtual QUuid gateway() const override;
    void setTicker(QByteArray&);
    virtual double averagePrice() const override;
    virtual double lastPrice() const override;
    virtual double preClosePrice() const override;
    virtual double openPrice() const override;
    virtual double closePrice() const override;
    virtual double highPrice() const override;
    virtual double lowPrice() const override;
    virtual quint64 volume() const override;
    virtual double turnover() const override;
    virtual double bidPrice(QuoteOrderType which) const override;
    virtual quint64 bidVolume(QuoteOrderType which) const override;
    virtual double askPrice(QuoteOrderType which) const override;
    virtual quint64 askVolume(QuoteOrderType which) const override;
    virtual quint64 datetime() const override; // YYYYmmddHHMMSSsss
    //     virtual qint32 date() const override;      // YYYYMMDD
    //     virtual qint32 day() const override;
    //     virtual qint32 hour() const override;
    //     virtual qint32 minute() const override;
    //     virtual qint32 second() const override;
    //     virtual qint32 millisecond() const override;
    //     virtual quint64 msecsSinceEpoch() const override;

private:
    SinaTickInfoPrivate* d;
};

} // namespace Qs
#endif // QS_SINA_DATATYPE_H
