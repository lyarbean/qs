#ifndef DEFAULTENGINEPRIVATE_H
#define DEFAULTENGINEPRIVATE_H
#include <QMap>
#include <QThread>
namespace Qs {
class StrategyAbstract;
class RiskManager;
class PositionManager;
class DefaultEnginePrivate {
public:
    DefaultEnginePrivate(DefaultEngine* q);
    virtual ~DefaultEnginePrivate();

private:
    friend class DefaultEngine;
    DefaultEngine* const q;
    QThread thread;
    RiskManager* riskManager;
    PositionManager* positionManager;
    QMap<QUuid, StrategyAbstract*> strategies;
    QMap<QUuid, GatewayAbstract*> gateways;
};
} // namespace Qs

#endif // DEFAULTENGINEPRIVATE_H
