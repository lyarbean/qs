#include "sinagateway_p.h"
#include "sinadatatypes.h"
#include <cstring>
#include <QCoreApplication>
#include <QTextCodec>
#include <QDebug>
#include "core/ipc/shmtickinfo.h"
namespace Qs {
static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    SinaGatewayPrivate* p = (SinaGatewayPrivate*)userp;
    struct MemoryStruct* mem = &p->received; //(struct MemoryStruct*)userp;

    mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL) {
        // std::cerr << "not enough memory (realloc returned NULL)" << std::endl;
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    return realsize;
}
static bool is_CURL_GLOBAL_ALL_initialized = false;
SinaGatewayPrivate::SinaGatewayPrivate(SinaGateway* q)
  : q(q) {

}
SinaGatewayPrivate::~SinaGatewayPrivate() {
    curl_easy_cleanup(curlHandler);
}

const QString prefixUri = "http://hq.sinajs.cn/list=";

void SinaGatewayPrivate::fetch() {

    if (!is_CURL_GLOBAL_ALL_initialized) {
        is_CURL_GLOBAL_ALL_initialized = true;
    }
    if(!curlHandler) {
        curlHandler = curl_easy_init();
    }
    if (urls.isEmpty()) {
        return;
    }
    // TODO Reuse allocated memory
    received.memory = (char*)malloc(1);
    received.size = 0;
    QString urlsString = urls.join(',');
    urlsString.prepend(prefixUri);
    //     qCritical() << __PRETTY_FUNCTION__ << urlsString;
    curl_easy_setopt(curlHandler, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curlHandler, CURLOPT_WRITEDATA, (void*)this);
    curl_easy_setopt(curlHandler, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(curlHandler, CURLOPT_URL, (void*)urlsString.toLocal8Bit().constData());
    auto res = curl_easy_perform(curlHandler);
    if (res == CURLE_OK) {
        QByteArray buffer(received.memory, received.size);
        QString converted = QTextCodec::codecForName("GBK")->toUnicode(buffer);
                qCritical() << converted;
        auto data = converted.split('\n');
        for (auto& datum : data) {
            if (datum.isEmpty()) {
                return;
            }
            // FIXME Use shared pointer
            TickInfoPointer tick = QSharedPointer<SinaTickInfo>::create(datum, q->uuid());
            emit q->hasTick(tick);
            ShmTickInfo::create(*tick); // Write shared memory
            // TODO add helper to signal changes
        }
    } else {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }
    received.size = 0;
    free(received.memory);
}

SinaGateway::SinaGateway()
  : d(new SinaGatewayPrivate(this)) {
}

SinaGateway::~SinaGateway() {
    delete d;
}

void SinaGateway::connectToServer() {
    // do nothing
    d->timer = new QTimer;
    connect(d->timer, &QTimer::timeout, [this]() {
        QMetaObject::invokeMethod(this, "pull", Qt::QueuedConnection);
    });
    d->timer->start(3000);
}

void SinaGateway::disconnectFromServer() {
}

void SinaGateway::subscribe(SubscribeRequestPointer& request) {
    d->urls.append(request->tickerList());
}

void SinaGateway::cancelOrder(CancelOrderRequestPointer& request) {
}

void SinaGateway::queryAccount() {
}

void SinaGateway::queryPosition() {
}

void SinaGateway::sendOrder(OrderRequestPointer& request) {
}

const QUuid& SinaGateway::uuid() {
    if (d->uuid.isNull()) {
        d->uuid = QUuid::createUuid();
    }
    return d->uuid;
}

void Qs::SinaGateway::pull() {
    d->fetch();
}
} // namespace Qs
