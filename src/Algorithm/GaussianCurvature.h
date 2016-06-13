#ifndef GAUSSIANCURVATURE_H
#define GAUSSIANCURVATURE_H

#include <BaseAlgorithm.h>

namespace sqi
{
namespace alg
{

class GaussianCurvature : public BaseAlgorithm
{
  
public:
  
  GaussianCurvature();
  ~GaussianCurvature();
  void cal_tri_error(sqi::meshdata::MeshDataPtr& mesh_data, 
                     lq::InspectFlag flag);
  void CalSingleProperty(meshdata::MeshDataPtr &mesh_data, lq::InspectFlag flag, 
                         lq::WindowFlag win);
};

}
}
#endif // GAUSSIANCURVATURE_H
