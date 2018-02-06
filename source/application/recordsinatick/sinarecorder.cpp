#include "gateway/sina/sinagateway.h"
#include "core/defaultengine.h"
//#include "core/recordstrategy.h"
#include "gateway/sina/sinadatatypes.h"
#include "core/strategyabstract.h"
#include <QPluginLoader>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
int main (int argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    Qs::StrategyAbstract* strategy = nullptr;
    foreach (QString fileName, QDir::current().entryList(QDir::Files)) {
          QPluginLoader loader(QDir::current().absoluteFilePath(fileName));
          QObject *plugin = loader.instance();
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
    Qs::DefaultEngine engine;
    auto gatewayId = engine.addGateway(new Qs::SinaGateway);

    engine.addStrategy(strategy);
    // TODO use signal slot connection to elimite this
    strategy->setEngine(&engine);
    engine.connectServers();
    Qs::SinaSubscribeRequest r1("sz002405");
    Qs::SinaSubscribeRequest r2("sz002268");
    engine.Subscribe(&r1, gatewayId);
    engine.Subscribe(&r2, gatewayId);
    return app.exec();
}
