#ifndef OZ_RISKMANAGER_H
#define OZ_RISKMANAGER_H

class QUuid;
namespace Qs {
class OrderRequest;
class RiskManager
{
public:
    RiskManager();
    bool check(OrderRequest* request, const QUuid& gateway);
};

}
#endif // OZ_RISKMANAGER_H
