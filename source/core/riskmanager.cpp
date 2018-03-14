#include "riskmanager.h"

Qs::RiskManager::RiskManager() {
}

bool Qs::RiskManager::check(Qs::OrderRequestPointer& request, const QUuid& gateway) {
    if (request->side == BuySide) {
        // ..
    } else if (request->side == SellSide) {

    }
    return true;
}
