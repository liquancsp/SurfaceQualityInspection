#ifndef MIXREFLECTIONCONVEX_H
#define MIXREFLECTIONCONVEX_H

#include <ReflectionMap.h>

namespace sqi {
namespace alg {

class MixReflectionConvex : public ReflectionMap
{
  
public:
  
  MixReflectionConvex();
  ~MixReflectionConvex();
  
  virtual void cal_tri_error(meshdata::MeshDataPtr &mesh_data, lq::InspectFlag flag);
  
};

}

}

#endif // MIXREFLECTIONCONVEX_H
