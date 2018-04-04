#include <QPluginLoader>
#include <QCoreApplication>
#include <QDir>
#include <QThread>
#include <QDebug>

// Import interfaces
#include "core/defaultengine.h"
#include "core/strategyabstract.h"
#include "core/gatewayabstract.h"
#include "utils/eventdispatcher_epoll.h"

int main(int argc, char* argv[]) {
    QCoreApplication::setEventDispatcher(new EventDispatcherEPoll);
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
    // Moved to a separate thread
    QThread strategyThread;
    strategyThread.setEventDispatcher(new EventDispatcherEPoll);

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

    // Moved to a separate thread
    QThread gatewayThread;
    gatewayThread.setEventDispatcher(new EventDispatcherEPoll);

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
    Qs::SubscribeRequestPointer r(new Qs::SubscribeRequest(gateway->uuid(), {"sz002405", "sz002268"}));
    engine.Subscribe(r, gateway->uuid());

    // [7] start the main event loop
    return app.exec();
}
