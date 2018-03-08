#ifndef QS_XTP_DATATYPE_H
#define QS_XTP_DATATYPE_H
#include "core/datatypes.h"

struct XTPMarketDataStruct;
namespace Qs {
class XtpTickInfoPrivate;
class XtpTickInfo : public TickInfo {
public:
    explicit XtpTickInfo(const struct XTPMarketDataStruct& source, const QUuid& gateway);
    ~XtpTickInfo();
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
    // TODO more overrides

private:
    XtpTickInfoPrivate* d;
};

} // namespace Qs
#endif // QS_XTP_DATATYPE_H
