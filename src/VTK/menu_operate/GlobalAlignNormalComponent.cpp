#include <menu_operate/GlobalAlignNormalComponent.h>
#include <IntegrationReflection.h>
#include <LeastSquares.h>
#include <menu_operate/VtkLeastSquare.h>

namespace sqi{
namespace alg{

using namespace sqi::meshdata;

GlobalAlignNormalComponent::GlobalAlignNormalComponent() : BaseAlgorithm(),
  GlobalAlign()
{
  
}

GlobalAlignNormalComponent::~GlobalAlignNormalComponent()
{
  
}

void GlobalAlignNormalComponent::cal_tri_error(meshdata::MeshDataPtr &mesh_data,
                                               lq::InspectFlag flag)
{
  VtkLeastSquare<meshdata::Mesh, vcg::Matrix33f> vtk_svd;
  vcg::Matrix33f rotate_mat1;
  vtk_svd.GlobalLeastSquare(mesh_data->M(lq::kRight), mesh_data->M(lq::kLeft), rotate_mat1);
  IntegrationReflection::GenerateIntegralReflMapList(mesh_data);
  for(size_t i = 0; i < 2; ++i)
  {
    if(!mesh_data->IsHasFNormal(i))
    {
      vcg::tri::UpdateNormal<Mesh>::PerFace(mesh_data->M(i));
      vcg::tri::UpdateNormal<Mesh>::NormalizePerFace(mesh_data->M(i));
      mesh_data->IsHasFNormal(i) = true;
    }
  }
  
  meshdata::FaceHandleD global_normal_com = mesh_data->GetGlobalNormalCom();
  if(!mesh_data->IsHasGlobalMat())
  {
    GlobalAlign::ExecuteGlobalAlgin(mesh_data);
    std::cout << "global matrix by ICP:" << std::endl;
    for(size_t i = 0; i < 3; ++i)
    {
      for(size_t j = 0; j < 3; ++j)
        std::cout << mesh_data->GlobalAlignMat()[i][j] << " ";
      std::cout << std::endl;
    }
    meshdata::FaceHandleD global_integral = mesh_data->GetGlobalIntegral();
    for(size_t i = 0; i < mesh_data->FNum(lq::kRight); ++i)
    {
      AlignAlgorithm::CalIntegralRefl(i, mesh_data->GlobalAlignMat(),
                                      global_integral, mesh_data);
      AlignAlgorithm::CalNormalComponent(i, mesh_data->GlobalAlignMat(),
                                         global_normal_com, mesh_data);
      sqi::meshdata::FaceIterator fi = mesh_data->FBeginIter(lq::kRight) + i;
      fi->Q() = global_normal_com[*fi];
    }
    mesh_data->IsHasGlobalMat() = true;
  }
  else 
  {
    meshdata::FaceIterator fi;
    for(size_t i = 0; i < mesh_data->FNum(lq::kRight); ++i)
    {
      fi = mesh_data->FBeginIter(lq::kRight) + i;
      fi->Q() = global_normal_com[*fi];
    }
  }
  BaseAlgorithm::GenerateFinalScalarField(mesh_data, flag);
}

}
}
