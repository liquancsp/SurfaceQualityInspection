#include <menu_operate/GlobalAlignIntegralMap.h>
#include <IntegrationReflection.h>
#include <vtk_data/vtk_model_data.h>
#include <vcg/space/deprecated_point4.h>
#include <CurvatureAlg.h>

//#include <vtkLandmarkTransform.h>

namespace sqi{
namespace alg{
using namespace sqi::meshdata;

GlobalAlignIntegralMap::GlobalAlignIntegralMap() : BaseAlgorithm(),
  GlobalAlign()
{
  
}

GlobalAlignIntegralMap::~GlobalAlignIntegralMap()
{
  
}

void GlobalAlignIntegralMap::cal_tri_error(meshdata::MeshDataPtr &mesh_data, 
                                           lq::InspectFlag flag)
{
  IntegrationReflection::GenerateIntegralReflMapList(mesh_data);
  CurvatureAlg<Mesh>::UpdateConcavAndConvex(mesh_data->M(lq::kLeft),
                                            mesh_data->M(lq::kRight));
  
  meshdata::FaceHandleD global_integral = mesh_data->GetGlobalIntegral();
  
  if(!mesh_data->IsHasGlobalMat())
  {
    GlobalAlign::ExecuteGlobalAlgin(mesh_data);
    mesh_data->GlobalAlignMat().print();
    meshdata::FaceHandleD global_normal_com = mesh_data->GetGlobalNormalCom();
    for(size_t i = 0; i < mesh_data->FNum(lq::kRight); ++i)
    {
      AlignAlgorithm::CalIntegralRefl(i, mesh_data->GlobalAlignMat(),
                                      global_integral, mesh_data);
      AlignAlgorithm::CalNormalComponent(i, mesh_data->GlobalAlignMat(), 
                                         global_normal_com, mesh_data);
      sqi::meshdata::FaceIterator fi = mesh_data->FBeginIter(lq::kRight) + i;
      fi->Q() = global_integral[*fi];
    }
    mesh_data->IsHasGlobalMat() = true;
  }
  else 
  {
    meshdata::FaceIterator fi;
    for(size_t i = 0; i < mesh_data->FNum(lq::kRight); ++i)
    {
      fi = mesh_data->FBeginIter(lq::kRight) + i;
      fi->Q() = global_integral[*fi];
    }
  }
  BaseAlgorithm::MixAlgWithConvex(mesh_data, lq::kRight);
  BaseAlgorithm::GenerateFinalScalarField(mesh_data, flag);
  
  
//  for(size_t i = 0; i < mesh_data->FNum(lq::kRight); ++i)
//  {
//    AlignAlgorithm::CalIntegralRefl(i, mesh_data); 
////    sqi::meshdata::FaceIterator fi1 = mesh_data->FBeginIter(lq::kLeft);
////    sqi::meshdata::FaceIterator fi2 = mesh_data->FBeginIter(lq::kRight);
////    fi1 += i;
////    fi2 += i;
////    vcg::Point4d normal1(fi1->cN()[0], fi1->cN()[1], fi1->cN()[2], 0.0f);
////    vcg::Point4d normal2(fi2->cN()[0], fi2->cN()[1], fi2->cN()[2], 0.0f);
//////    normal2 = vcg_mat * normal2;
////    normal2 = mesh_data->GlobalAlignMat() * normal2;
////    double angle = vcg::Angle<double>(normal1, normal2);
////    angle = (angle * 180.0f) / lq::PI;
////    int diff_idx = static_cast<int>(angle * 100);
////    if(diff_idx > mesh_data->IntegralReflList().size() - 1)
////      diff_idx = mesh_data->IntegralReflList().size() - 1;
////    fi2->Q() = mesh_data->IntegralReflList()[diff_idx];
//  }
//  BaseAlgorithm::GenerateFinalScalarField(mesh_data, flag);
}

//void GlobalAlignIntegralMap::ExecuteGlobalAlgin(meshdata::MeshDataPtr &mesh_data)
//{
//  vtkSmartPointer<vtkPoints> points_left = 
//      vtkSmartPointer<vtkPoints>::New();
//  vtkSmartPointer<vtkPoints> points_right = 
//      vtkSmartPointer<vtkPoints>::New();
//  sqi::meshdata::VertexIterator vi1, vi2;
//  for(size_t i = 0; i < mesh_data->VNum(lq::kRight); ++i)
//  {
//    vi1 = mesh_data->VBeginIter(lq::kLeft) + i;
//    vi2 = mesh_data->VBeginIter(lq::kRight) + i;
//    points_left->InsertNextPoint(vi1->P()[0], vi1->P()[1], vi1->P()[2]);
//    points_right->InsertNextPoint(vi2->P()[0], vi2->P()[1], vi2->P()[2]);
//  }
//  vtkSmartPointer<vtkLandmarkTransform> land_mark_trans = 
//      vtkSmartPointer<vtkLandmarkTransform>::New();
//  GenerateRigidTransMatrix4x4(points_right, points_left, land_mark_trans);
//  vtkSmartPointer<vtkMatrix4x4> vtk_mat = 
//      vtkSmartPointer<vtkMatrix4x4>::New();
//  vtk_mat = land_mark_trans->GetMatrix();
//  TransMatrix4x4FromVtkToVcg(vtk_mat, mesh_data->GlobalAlignMat());
//}

//void GlobalAlignIntegralMap::GenerateRigidTransMatrix4x4(const vtkSmartPointer<vtkPoints> &points1, 
//                                                         const vtkSmartPointer<vtkPoints> &points2,
//                                                         vtkSmartPointer<vtkLandmarkTransform> &land_mark_transform)
//{
//  land_mark_transform->SetSourceLandmarks(points1);
//  land_mark_transform->SetTargetLandmarks(points2);
//  land_mark_transform->SetModeToRigidBody();
//  land_mark_transform->Update();
//}

//void GlobalAlignIntegralMap::TransMatrix4x4FromVtkToVcg(const vtkSmartPointer<vtkMatrix4x4> &vtk_mat, 
//                                                        vcg::Matrix44d &vcg_mat)
//{
//  double tmp[16];
//  for(size_t i = 0; i < 4; ++i)
//  {
//    for(size_t j = 0; j < 4; ++j)
//      tmp[i * 4 + j] = vtk_mat->GetElement(i, j);
//  }
//  vcg::Matrix44d tmp_mat(tmp);
//  vcg_mat = tmp_mat;
//}

}

}
