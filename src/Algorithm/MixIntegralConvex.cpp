#include <MixIntegralConvex.h>

namespace sqi {
namespace alg {

MixIntegralConvex::MixIntegralConvex() : IntegrationReflection()
{
  
}

MixIntegralConvex::~MixIntegralConvex()
{
  
}

void MixIntegralConvex::cal_tri_error(meshdata::MeshDataPtr &mesh_data, 
                                      lq::InspectFlag flag)
{
  IntegrationReflection::cal_tri_error(mesh_data, flag);
  BaseAlgorithm::MixAlgWithConvex(mesh_data, lq::kRight);
}

}

}
 