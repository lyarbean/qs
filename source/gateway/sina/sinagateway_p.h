#ifndef SINAGATEWAYPRIVATE_H
#define SINAGATEWAYPRIVATE_H
#include "sinagateway.h"
#include <curl/curl.h>
#include <QThread>
#include <QTimer>
struct MemoryStruct {
    char* memory;
    size_t size;
};

namespace Qs {

class SinaGatewayPrivate
{
public:
    SinaGatewayPrivate(SinaGateway* q);
    virtual ~SinaGatewayPrivate();
    void fetch();
    class SinaGateway* const q;
    MemoryStruct received;
    CURL* curlHandler;
    QThread thread;
    QTimer* timer;
    QStringList urls;
};
}

#endif // SINAGATEWAYPRIVATE_H
