#ifndef CONCAVECONVEX_H
#define CONCAVECONVEX_H
#include <BaseAlgorithm.h>

namespace sqi{
namespace alg {

class ConcaveConvex : public BaseAlgorithm {
  
public:
  
  ConcaveConvex();
  ~ConcaveConvex();
  
  void cal_tri_error(meshdata::MeshDataPtr &mesh_data, lq::InspectFlag flag);
  void CalSingleProperty(meshdata::MeshDataPtr& mesh_data,
                         lq::InspectFlag flag, lq::WindowFlag win);
  
};

}

}


#endif // CONCAVECONVEX_H
