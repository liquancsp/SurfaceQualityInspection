#ifndef PRINCIPALCURVATURE_H
#define PRINCIPALCURVATURE_H

#include <BaseAlgorithm.h>

namespace sqi
{
namespace alg
{

class PrincipalCurvature : public BaseAlgorithm
{
public:
  PrincipalCurvature();
  ~PrincipalCurvature();
  void cal_tri_error(sqi::meshdata::MeshDataPtr& mesh_data, lq::InspectFlag flag);
  void CalSingleProperty(meshdata::MeshDataPtr &mesh_data, lq::InspectFlag flag,
                         lq::WindowFlag win);
  

};

}
}

#endif // PRINCIPALCURVATURE_H
