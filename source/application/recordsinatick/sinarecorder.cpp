#include <QPluginLoader>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>

// Import interfaces
#include "core/defaultengine.h"
#include "core/strategyabstract.h"
#include "core/gatewayabstract.h"

// Import here for SinaSubscribeRequest derived from SubscribeRequest
#include "gateway/sina/sinadatatypes.h"


int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    // Load plugins
    // [1] Strategy
    Qs::StrategyAbstract* strategy = nullptr;
    {
        QPluginLoader loader(QDir("./source/strategy/").absoluteFilePath("librecordstrategy.so"));
        QObject* plugin = loader.instance();
        if (plugin) {
            qCritical() << loader.metaData().value("MetaData").toObject().value("Build").toString();
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
    // [2] Gateway
    Qs::GatewayAbstract* gateway = nullptr;
    {
        QPluginLoader loader(QDir("./source/gateway/").absoluteFilePath("libsinagateway.so"));
        QObject* plugin = loader.instance();
        if (plugin) {
            qCritical() << loader.metaData().value("MetaData").toObject().value("Build").toString();
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

    // [3] Engine
    Qs::DefaultEngine engine;
    // [4] attach gateway and strategy
    auto gatewayId = engine.addGateway(gateway);
    engine.addStrategy(strategy);

    // [5] As it's named
    engine.connectServers();

    // [6] Subscribe two tickers
    Qs::SubscribeRequestPointer r1(new Qs::SinaSubscribeRequest("sz002405"));
    Qs::SubscribeRequestPointer r2(new Qs::SinaSubscribeRequest("sz002268"));
    engine.Subscribe(r1, gatewayId);
    engine.Subscribe(r2, gatewayId);

    // [7] start the main event loop
    return app.exec();
}
