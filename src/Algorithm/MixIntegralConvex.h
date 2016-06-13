#ifndef MIXINTEGRALCONVEX_H
#define MIXINTEGRALCONVEX_H
#include <IntegrationReflection.h>

namespace sqi {
namespace alg {

class MixIntegralConvex : public  IntegrationReflection
{
public :
  
  MixIntegralConvex();
  ~MixIntegralConvex();
  
  virtual void cal_tri_error(meshdata::MeshDataPtr &mesh_data, 
                             lq::InspectFlag flag);
  
  
};

}

}

#endif // MIXINTEGRALCONVEX_H
