#include "gateway/sina/sinagateway.h"
#include "core/defaultengine.h"
//#include "core/recordstrategy.h"
#include "gateway/sina/sinadatatypes.h"
#include "core/strategyabstract.h"
#include <QPluginLoader>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    // foreach (QString fileName, QDir::current().entryList(QDir::Files))
    Qs::StrategyAbstract* strategy = nullptr;
    {
        QPluginLoader loader(QDir("./source/strategy/").absoluteFilePath("librecordstrategy.so"));
        QObject* plugin = loader.instance();
        if (plugin) {
            auto t = qobject_cast<Qs::StrategyAbstract*>(plugin);
            if (t) {
                strategy = t;
            }
        }
    }
    if (!strategy) {
        qCritical() << "Not strategy found";
        return 0;
    }
    Qs::GatewayAbstract* gateway = nullptr;
    {
        QPluginLoader loader(QDir("./source/gateway/").absoluteFilePath("libsinagateway.so"));
        QObject* plugin = loader.instance();
        if (plugin) {
            auto t = qobject_cast<Qs::GatewayAbstract*>(plugin);
            if (t) {
                gateway = t;
            }
        }
    }
    if (!gateway) {
        qCritical() << "Not gateway found";
        return 0;
    }
    Qs::DefaultEngine engine;
    auto gatewayId = engine.addGateway(gateway);

    engine.addStrategy(strategy);
    // TODO use signal slot connection to elimite this
    engine.connectServers();
    Qs::SubscribeRequestPointer r1(new Qs::SinaSubscribeRequest("sz002405"));
    Qs::SubscribeRequestPointer r2(new Qs::SinaSubscribeRequest("sz002268"));
    engine.Subscribe(r1, gatewayId);
    engine.Subscribe(r2, gatewayId);
    return app.exec();
}
