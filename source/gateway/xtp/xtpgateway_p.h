#ifndef XTPGATEWAYPRIVATE_H
#define XTPGATEWAYPRIVATE_H
// #include "xtpgateway.h"

#include "XTP_API/bin/include/xtp_quote_api.h"
#include "XTP_API/bin/include/xtp_trader_api.h"

#include <QThread>
#include <QTimer>
#include <QUuid>

namespace Qs {
class XtpGateway;
class XtpGatewayPrivate;
class XtpQuoteSpi : public XTP::API::QuoteSpi {
public:
    explicit XtpQuoteSpi(XtpGatewayPrivate* q);
    virtual ~XtpQuoteSpi();
    void start();
    void stop();
    int login();
    // QuoteSpi
    void OnDisconnected(int reason) override;
    void OnError(XTPRI* error_info) override;
    void OnSubMarketData(XTPST* ticker, XTPRI* error_info, bool is_last) override;
    void OnUnSubMarketData(XTPST* ticker, XTPRI* error_info, bool is_last) override;
    void OnDepthMarketData(XTPMD* market_data, int64_t bid1_qty[], int32_t bid1_count, int32_t max_bid1_count,
                           int64_t ask1_qty[], int32_t ask1_count, int32_t max_ask1_count) override;
    void OnQueryAllTickers(XTPQSI* ticker_info, XTPRI* error_info, bool is_last) override;
    XtpGatewayPrivate* const q;
    XTP::API::QuoteApi* api = nullptr;
};

class XtpTraderSpi : public XTP::API::TraderSpi {
public:
    explicit XtpTraderSpi(XtpGatewayPrivate* q);
    virtual ~XtpTraderSpi();
    void start();
    void stop();
    int login();
    // TraderSpi
    void OnDisconnected(uint64_t session_id, int reason) override;
    void OnError(XTPRI* error_info) override;
    void OnOrderEvent(XTPOrderInfo* order_info, XTPRI* error_info, uint64_t session_id) override;
    void OnTradeEvent(XTPTradeReport* trade_info, uint64_t session_id) override;
    void OnCancelOrderError(XTPOrderCancelInfo* cancel_info, XTPRI* error_info, uint64_t session_id) override;
    void OnQueryOrder(XTPQueryOrderRsp* order_info, XTPRI* error_info, int request_id, bool is_last,
                      uint64_t session_id) override;
    void OnQueryTrade(XTPQueryTradeRsp* trade_info, XTPRI* error_info, int request_id, bool is_last,
                      uint64_t session_id) override;
    void OnQueryPosition(XTPQueryStkPositionRsp* position, XTPRI* error_info, int request_id, bool is_last,
                         uint64_t session_id) override;
    void OnQueryAsset(XTPQueryAssetRsp* asset, XTPRI* error_info, int request_id, bool is_last,
                      uint64_t session_id) override;
    XtpGatewayPrivate* const q;
    XTP::API::TraderApi* api = nullptr;
    int session = 0;
};

class XtpGatewayPrivate {
public:
    explicit XtpGatewayPrivate(XtpGateway* q);
    virtual ~XtpGatewayPrivate();
    XtpGateway* const q;
    QUuid uuid;
    XtpQuoteSpi* quote;
    XtpTraderSpi* trader;
};
} // namespace Qs

#endif // XTPGATEWAYPRIVATE_H
