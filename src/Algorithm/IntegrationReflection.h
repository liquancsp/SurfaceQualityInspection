#ifndef INTEGRATIONREFLECTION_H
#define INTEGRATIONREFLECTION_H
#include <BaseAlgorithm.h>
namespace sqi
{
namespace alg
{

class IntegrationReflection : public BaseAlgorithm
{
  
public:
  
  IntegrationReflection();
  ~IntegrationReflection();
  
  void cal_tri_error(meshdata::MeshDataPtr &mesh_data, lq::InspectFlag flag);
  
  static void GenerateIntegralReflMapList(meshdata::MeshDataPtr& mesh_data);
};

}
}

#endif // INTEGRATIONREFLECTION_H
