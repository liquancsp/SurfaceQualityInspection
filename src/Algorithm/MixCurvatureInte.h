#ifndef MIXCURVATUREINTE_H
#define MIXCURVATUREINTE_H

#include <IntegrationReflection.h>
#include <CurvatureAlg.h>

namespace sqi {
namespace alg {

class MixCurvatureInte : public  IntegrationReflection
{
public:
  MixCurvatureInte();
  ~MixCurvatureInte();
  
  virtual void cal_tri_error(meshdata::MeshDataPtr &mesh_data, lq::InspectFlag flag);
};

}

}
#endif // MIXCURVATUREINTE_H
