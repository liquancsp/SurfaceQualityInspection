#ifndef DISTANCEDIFFERENT_H
#define DISTANCEDIFFERENT_H
#include <BaseAlgorithm.h>

namespace sqi
{
namespace alg
{

class DistanceDifferent : public BaseAlgorithm
{
  
public:
  DistanceDifferent();
  ~DistanceDifferent();
  void cal_tri_error(meshdata::MeshDataPtr &mesh_data, lq::InspectFlag flag);
  
};
}
}
#endif // DISTANCEDIFFERENT_H
