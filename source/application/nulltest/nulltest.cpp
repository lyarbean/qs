#include "core/defaultengine.h"

#include "nulltest.h"
using namespace Qs;



int main (int argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    auto engine = new DefaultEngine();
    auto gateway = new NullGateway();
    auto gatewayId = engine->addGateway(gateway);
    auto strategy = new NullStrategy;
    engine->addStrategy(strategy);
    // TODO use signal slot connection to elimite this
    strategy->setEngine(engine);
    engine->connectServers();
    QMetaObject::invokeMethod(gateway, "run");
    QTimer::singleShot(elapsed, engine, [&](){
        // Print the results
        strategy->count();
        gateway->count();
        engine->deleteLater();
        app.quit();
    });
    return app.exec();
}
