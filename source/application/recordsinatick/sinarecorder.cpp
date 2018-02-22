#include <QPluginLoader>
#include <QCoreApplication>
#include <QDir>
#include <QThread>
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
    // Move to a separte thread
    QThread strategyThread;
    strategy->moveToThread(&strategyThread);
    strategyThread.start();

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

    // Move to a separte thread
    QThread gatewayThread;
    gateway->moveToThread(&gatewayThread);
    gatewayThread.start();

    // [3] Engine
    Qs::DefaultEngine engine;
    // [4] attach gateway and strategy
    Qs::GatewaySharedPointer gatewaySharedPointer(gateway);
    engine.addGateway(gatewaySharedPointer);
    Qs::StrategySharedPointer strategySharedPointer(strategy);
    engine.addStrategy(strategySharedPointer);

    // [5] Start
    engine.connectServers();

    // [6] Subscribe two tickers
    Qs::SubscribeRequestPointer r1(new Qs::SinaSubscribeRequest("sz002405"));
    Qs::SubscribeRequestPointer r2(new Qs::SinaSubscribeRequest("sz002268"));
    engine.Subscribe(r1, gateway->uuid());
    engine.Subscribe(r2, gateway->uuid());

    // [7] start the main event loop
    return app.exec();
}
