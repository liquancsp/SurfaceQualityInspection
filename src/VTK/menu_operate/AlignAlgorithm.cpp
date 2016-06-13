#include <menu_operate/AlignAlgorithm.h>
#include <vtkMatrix4x4.h>
#include <vcg/complex/algorithms/geodesic.h>


namespace sqi{
namespace alg{

using namespace sqi::meshdata;

AlignAlgorithm::AlignAlgorithm()
{
  
}

AlignAlgorithm::~AlignAlgorithm()
{
  
}

void AlignAlgorithm::GenerateRigidTransMatrix4x4(const vtkSmartPointer<vtkPoints> &points1, 
                                                 const vtkSmartPointer<vtkPoints> &points2, 
                                                 vtkSmartPointer<vtkLandmarkTransform> &land_mark_transform)
{
  land_mark_transform->SetSourceLandmarks(points1);
  land_mark_transform->SetTargetLandmarks(points2);
  land_mark_transform->SetModeToRigidBody();
  land_mark_transform->Update();
}

void AlignAlgorithm::TransMatrix4x4FromVtkToVcg(const vtkSmartPointer<vtkMatrix4x4> &vtk_mat, 
                                                vcg::Matrix44f &vcg_mat)
{
  float tmp[16];
  for(size_t i = 0; i < 4; ++i)
  {
    for(size_t j = 0; j < 4; ++j)
      tmp[i * 4 + j] = vtk_mat->GetElement(i, j);
  }
  vcg::Matrix44f tmp_mat(tmp);
  vcg_mat = tmp_mat;
}

void AlignAlgorithm::CalIntegralRefl(const size_t idx, const vcg::Matrix44f& mat,
                                     meshdata::FaceHandleD& handle,
                                     meshdata::MeshDataPtr &mesh_data)
{
  sqi::meshdata::FaceIterator fi1 = mesh_data->FBeginIter(lq::kLeft) + idx;
  sqi::meshdata::FaceIterator fi2 = mesh_data->FBeginIter(lq::kRight) + idx;
  vcg::Point4f normal1(fi1->cN()[0], fi1->cN()[1], fi1->cN()[2], 0.0f);
  vcg::Point4f normal2(fi2->cN()[0], fi2->cN()[1], fi2->cN()[2], 0.0f);
//  normal2 = mesh_data->GlobalAlignMat() * normal2;
  normal2 = mat * normal2;
  float angle = vcg::Angle<float>(normal1, normal2);
  angle = (angle * 180.0f) / lq::PI;
  int diff_idx = static_cast<int>(angle * 100);
  if(diff_idx > mesh_data->IntegralReflList().size() - 1)
    diff_idx = mesh_data->IntegralReflList().size() - 1;
  handle[*fi2] = mesh_data->IntegralReflList()[diff_idx];
}

void AlignAlgorithm::CalIntegralReflVertex(const size_t idx, const vcg::Matrix44f &mat, 
                                           meshdata::MeshDataPtr &mesh_data, 
                                           std::vector<double>& vert_error)
{
  VertexIterator vi1 = mesh_data->VBeginIter(lq::kLeft) + idx;
  VertexIterator vi2 = mesh_data->VBeginIter(lq::kRight) + idx;
  vcg::Point4f normal1(vi1->cN()[0], vi1->cN()[1], vi1->cN()[2], 0.0);
  vcg::Point4f normal2(vi2->cN()[0], vi2->cN()[1], vi2->cN()[2], 0.0);
  normal2 = mat * normal2;
  float angle = vcg::Angle<float>(normal1, normal2);
  angle = (angle * 180.0f) / lq::PI;
  int diff_idx = static_cast<int>(angle * 100);
  if(diff_idx > mesh_data->IntegralReflList().size() - 1)
    diff_idx = mesh_data->IntegralReflList().size() - 1;
  vert_error[idx] = mesh_data->IntegralReflList()[diff_idx];
}

void AlignAlgorithm::CalNormalComponent(const size_t idx, const vcg::Matrix44f& mat,
                                        meshdata::FaceHandleD &handle,
                                        meshdata::MeshDataPtr &mesh_data)
{
  sqi::meshdata::FaceIterator fi1 = mesh_data->FBeginIter(lq::kLeft) + idx;
  sqi::meshdata::FaceIterator fi2 = mesh_data->FBeginIter(lq::kRight) + idx;
  vcg::Point3f bary_center1 = vcg::Barycenter(*fi1);
  vcg::Point3f bary_center2 = vcg::Barycenter(*fi2);
  vcg::Point4f tmp_center(bary_center2[0], bary_center2[1],
      bary_center2[2], 1.0f);
  tmp_center = mat * tmp_center;
  vcg::Point3f rotate_center(tmp_center[0], tmp_center[1], tmp_center[2]);
  rotate_center -= bary_center1;
  handle[*fi2] = rotate_center * fi1->cN() / fi1->cN().Norm();
}

void AlignAlgorithm::CalNormalComponentVertex(const size_t idx, const vcg::Matrix44f &mat, 
                                              MeshDataPtr &mesh_data, 
                                              std::vector<double> &vert_error)
{
  VertexIterator vi1 = mesh_data->VBeginIter(lq::kLeft) + idx;
  VertexIterator vi2 = mesh_data->VBeginIter(lq::kRight) + idx;
  vcg::Point4f tmp(vi2->cP()[0], vi2->cP()[1], vi2->cP()[2], 1.0f);
  tmp = mat * tmp;
  vcg::Point3f rotate_vert(tmp[0], tmp[1], tmp[2]);
  rotate_vert -= vi1->cP();
  vert_error[idx] = rotate_vert * vi1->cN() / vi1->cN().Norm();
}


}
}
