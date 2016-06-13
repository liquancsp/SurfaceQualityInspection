#include <MixCurvatureInte.h>

namespace sqi {
namespace alg {

MixCurvatureInte::MixCurvatureInte() : IntegrationReflection()
{
  
}

MixCurvatureInte::~MixCurvatureInte()
{
  
}

void MixCurvatureInte::cal_tri_error(meshdata::MeshDataPtr &mesh_data, lq::InspectFlag flag)
{
  IntegrationReflection::cal_tri_error(mesh_data, flag);
  if(flag == lq::kMixK1Inte || flag == lq::kMixK2Inte)
  {
    CurvatureAlg<meshdata::Mesh>::PrincipalCurvatureAndDirection(mesh_data->M(lq::kLeft)); 
    CurvatureAlg<meshdata::Mesh>::PrincipalCurvatureAndDirection(mesh_data->M(lq::kRight)); 
  }
  BaseAlgorithm::MixCurvatureWithInte(mesh_data, flag, lq::kRight);
}

}

}
