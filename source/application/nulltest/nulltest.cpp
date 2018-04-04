#include "core/defaultengine.h"
#include "nulltest.h"
#include "utils/eventdispatcher_epoll.h"
using namespace Qs;


/**
 * compiled with -O3
 *
 * EPOLL_DISPATCHER == 1
 Tick Received 60489683
 Order Received 60489683

 real    1m40.395s
 user    3m4.421s
 sys     0m14.791s
 *
 * EPOLL_DISPATCHER == 0
 Tick Received 43999848
 Order Received 43999848

 real    1m39.812s
 user    2m32.422s
 sys     0m42.808s
 *
 */



#define EPOLL_DISPATCHER 0
int main(int argc, char* argv[]) {
#if EPOLL_DISPATCHER
    QCoreApplication::setEventDispatcher(new EventDispatcherEPoll);
#endif
    QCoreApplication app(argc, argv);
    auto engine = new DefaultEngine;
    // [1] Gateway
    auto gateway = GatewaySharedPointer(new NullGateway);
    QThread gatewayThread;
#if EPOLL_DISPATCHER
    gatewayThread.setEventDispatcher(new EventDispatcherEPoll);
#endif
    gateway.data()->moveToThread(&gatewayThread);
    gatewayThread.start();
    engine->addGateway(gateway);

    // [2] Strategy
    auto strategy = StrategySharedPointer(new NullStrategy);
    QThread strategyThread;
#if EPOLL_DISPATCHER
    strategyThread.setEventDispatcher(new EventDispatcherEPoll);
#endif
    strategy.data()->moveToThread(&strategyThread);
    strategyThread.start();

    engine->addStrategy(strategy);
    strategy->acceptGateway(gateway->uuid());

    QObject::connect(strategy.data(), &StrategyAbstract::order, engine, &EngineAbstract::sendOrder);
    engine->connectServers();
    QTimer::singleShot(elapsed, &app, [&]() {
        // Print the results
        static_cast<NullGateway*>(gateway.data())->stop();
        dynamic_cast<NullStrategy*>(strategy.data())->count();
        dynamic_cast<NullGateway*>(gateway.data())->count();
        gatewayThread.quit();
        strategyThread.quit();
        gatewayThread.wait();
        strategyThread.wait();
        delete engine;
        app.quit();
    });
    QMetaObject::invokeMethod(gateway.data(), "run");
    return app.exec();
}
