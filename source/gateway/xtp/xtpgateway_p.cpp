#include "xtpgateway_p.h"

namespace Qs {

// TODO use cmake configfile or external db
XtpQuoteSpi::XtpQuoteSpi(XtpGatewayPrivate* q)
  : q(q) {
    api = XTP::API::QuoteApi::CreateQuoteApi(1, "/tmp", XTP_LOG_LEVEL_WARNING);
    api->RegisterSpi(this);
}

XtpQuoteSpi::~XtpQuoteSpi() {
    if (api) {
        api->Release();
    }
}

void XtpQuoteSpi::start() {
    stop();
    login();
}

void XtpQuoteSpi::stop() {
    api->Logout();
}

int XtpQuoteSpi::login() {
    /**
    auto& ip = Config::quoteServerIp;
    auto& port = Config::quoteServerPort;
    auto& name = Config::quoteUsername;
    auto& password = Config::quotePassword;
    int loginResult = api->Login(ip, port, name, password, XTP_PROTOCOL_TCP);
    if (loginResult == 0) {
        //         if (!m_writer) {
        //             std::string day = m_quoteApi->GetTradingDay();
        //             std::string fileName = Config::filepath;
        //             std::string fileNameSZ = fileName + "/SZ" + day;
        //             std::string fileNameSH = fileName + "/SH" + day;
        //             m_writer = new Writer(fileNameSZ, fileNameSH);
        //         }
        //char* tickers[] = {"002268","002405"};
        // m_quoteApi->SubscribeMarketData(tickers, 2, XTP_EXCHANGE_SZ);
        // NOTE QueryAllTickers
//         m_quoteApi->QueryAllTickers(XTP_EXCHANGE_SZ);
//         m_quoteApi->QueryAllTickers(XTP_EXCHANGE_SH);

    } else {
        XTPRI* error_info = api->GetApiLastError();
        qCritical() << "XTPQuoteSpi::Login, Login to server error, " << error_info->error_id << " : "
                    << error_info->error_msg;
    }
    return loginResult;
    **/
    return 0;
}

void XtpQuoteSpi::OnDisconnected(int reason) {
}
void XtpQuoteSpi::OnError(XTPRI* error_info) {
}
void XtpQuoteSpi::OnSubMarketData(XTPST* ticker, XTPRI* error_info, bool is_last) {
}
void XtpQuoteSpi::OnUnSubMarketData(XTPST* ticker, XTPRI* error_info, bool is_last) {
}
void XtpQuoteSpi::OnDepthMarketData(XTPMD* market_data, int64_t bid1_qty[], int32_t bid1_count, int32_t max_bid1_count,
                                    int64_t ask1_qty[], int32_t ask1_count, int32_t max_ask1_count) {
}
void XtpQuoteSpi::OnQueryAllTickers(XTPQSI* ticker_info, XTPRI* error_info, bool is_last) {
}

XtpTraderSpi::XtpTraderSpi(XtpGatewayPrivate* q)
  : q(q) {
}

XtpTraderSpi::~XtpTraderSpi() {
    if (api) {
        api->Logout(session);
        api->Release();
    }
}

void XtpTraderSpi::start() {
    stop();
    login();
}

void XtpTraderSpi::stop() {
    api->Logout(session);
}

int XtpTraderSpi::login() {
}

void XtpTraderSpi::OnDisconnected(uint64_t session_id, int reason) {
}

void XtpTraderSpi::OnError(XTPRI* error_info) {
}

void XtpTraderSpi::OnOrderEvent(XTPOrderInfo* order_info, XTPRI* error_info, uint64_t session_id) {
}

void XtpTraderSpi::OnTradeEvent(XTPTradeReport* trade_info, uint64_t session_id) {
}

void XtpTraderSpi::OnCancelOrderError(XTPOrderCancelInfo* cancel_info, XTPRI* error_info, uint64_t session_id) {
}

void XtpTraderSpi::OnQueryOrder(XTPQueryOrderRsp* order_info, XTPRI* error_info, int request_id, bool is_last,
                                uint64_t session_id) {
}

void XtpTraderSpi::OnQueryTrade(XTPQueryTradeRsp* trade_info, XTPRI* error_info, int request_id, bool is_last,
                                uint64_t session_id) {
}

void XtpTraderSpi::OnQueryPosition(XTPQueryStkPositionRsp* position, XTPRI* error_info, int request_id, bool is_last,
                                   uint64_t session_id) {
}

void XtpTraderSpi::OnQueryAsset(XTPQueryAssetRsp* asset, XTPRI* error_info, int request_id, bool is_last,
                                uint64_t session_id) {
}

XtpGatewayPrivate::XtpGatewayPrivate(XtpGateway* q)
  : q(q)
  , quote(new XtpQuoteSpi(this))
  , trader(new XtpTraderSpi(this)) {
}

XtpGatewayPrivate::~XtpGatewayPrivate() {
    if (quote) {
        delete quote;
        quote = nullptr;
    }
    if (trader) {
        delete trader;
        trader = nullptr;
    }
}

} // namespace Qs
