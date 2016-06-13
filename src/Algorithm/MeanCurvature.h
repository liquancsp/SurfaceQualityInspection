#ifndef MEANCURVATURE_H
#define MEANCURVATURE_H
#include <iostream>
#include <BaseAlgorithm.h>
namespace sqi
{
namespace alg
{

class MeanCurvature : public BaseAlgorithm
{
public:
  
  MeanCurvature();
  ~MeanCurvature();
  void cal_tri_error(sqi::meshdata::MeshDataPtr& mesh_data, 
                     lq::InspectFlag flag);
 
  void CalSingleProperty(meshdata::MeshDataPtr &mesh_data, lq::InspectFlag flag, 
                         lq::WindowFlag win);
};

}
}


#endif // MEANCURVATURE_H
