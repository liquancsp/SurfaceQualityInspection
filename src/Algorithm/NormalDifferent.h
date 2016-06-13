#ifndef NORMALDIFFERENT_H
#define NORMALDIFFERENT_H

#include <BaseAlgorithm.h>
namespace sqi
{
namespace alg
{

class NormalDifferent : public BaseAlgorithm
{
  
public:
  NormalDifferent();
  ~NormalDifferent();
  void cal_tri_error(meshdata::MeshDataPtr &mesh_data, lq::InspectFlag flag);
  
};

}
}

#endif // NORMALDIFFERENT_H
