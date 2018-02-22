#include "core/defaultengine.h"
#include "nulltest.h"
using namespace Qs;

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    auto engine = new DefaultEngine;
    // [1] Gateway
    auto gateway = GatewaySharedPointer(new NullGateway);
    QThread gatewayThread;
    gateway.data()->moveToThread(&gatewayThread);
//     QObject::connect(&gatewayThread, &QThread::finished, gateway.data(), &QObject::deleteLater);
    gatewayThread.start();
    engine->addGateway(gateway);

    // [2] Strategy
    auto strategy = StrategySharedPointer(new NullStrategy);
    QThread strategyThread;
    strategy.data()->moveToThread(&strategyThread);
//     QObject::connect(&strategyThread, &QThread::finished, strategy.data(), &QObject::deleteLater);
    strategyThread.start();

    engine->addStrategy(strategy);
    strategy->addGateway(gateway->uuid());
    // FIXME Use member functions
    QObject::connect(strategy.data(), &StrategyAbstract::order, engine, &EngineAbstract::sendOrder);
    engine->connectServers();
    QMetaObject::invokeMethod(gateway.data(), "run");
    QTimer::singleShot(elapsed, &app, [&]() {
        // Print the results
        dynamic_cast<NullStrategy*>(strategy.data())->count();
        dynamic_cast<NullGateway*>(gateway.data())->count();
        delete engine;
        gatewayThread.quit();
        strategyThread.quit();
        gatewayThread.wait();
        strategyThread.wait();
        app.quit();
    });
    return app.exec();
}
