#ifndef OZ_RISKMANAGER_H
#define OZ_RISKMANAGER_H
#include "datatypes.h"
class QUuid;
namespace Qs {
class RiskManager {
public:
    RiskManager();
    bool check(OrderRequestPointer& request, const QUuid& gateway);
};
} // namespace Qs
#endif // OZ_RISKMANAGER_H
