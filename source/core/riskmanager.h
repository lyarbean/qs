#ifndef QS_RISKMANAGER_H
#define QS_RISKMANAGER_H
#include "datatypes.h"
class QUuid;
namespace Qs {
class RiskManager {
public:
    RiskManager();
    bool check(OrderRequestPointer& request, const QUuid& gateway);
};
} // namespace Qs
#endif // QS_RISKMANAGER_H
