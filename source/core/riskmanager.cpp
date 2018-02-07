#include "riskmanager.h"

Qs::RiskManager::RiskManager() {
}

bool Qs::RiskManager::check(Qs::OrderRequestPointer& request, const QUuid& gateway) {
    return true;
}
