#include <menu_operate/LocalAlign.h>
#include <Topology/Geodesic.h>
#include <BaseAlgorithm.h>
#include <menu_operate/VtkLeastSquare.h>

#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkLandmarkTransform.h>
#include <vtkMatrix4x4.h>
#include <vtkIdList.h>



namespace sqi{
namespace alg{

using namespace sqi::meshdata;
using namespace lq;
using namespace std;

vtkSmartPointer<vtk_model_data> LocalAlign::data_set_ = NULL;

LocalAlign::LocalAlign() : AlignAlgorithm()
{
  
}

LocalAlign::~LocalAlign()
{
  
}

void LocalAlign::ExecuteComputeErrorUseFace(MeshDataPtr &mesh_data, lq::InspectFlag flag,
                                            const bool is_computed)
{
  FaceHandleD normal_handle = mesh_data->GetLocalNormalCom();
  FaceHandleD integral_handle = mesh_data->GetLocalIntegral();
  io::ConsoleMessage<std::string>::MessageText<int>("Thread number in integration reflection :",
                                                    io::kInfo, thread_num(mesh_data->FNum(0), MIN_ITERATOR_NUM));
  
  if(!is_computed)
  {
#pragma omp parallel for num_threads(thread_num(mesh_data->FNum(0), MIN_ITERATOR_NUM))
    for(int i = 0; i < mesh_data->FNum(0); ++i)
    {
      vector<int> neigh_list;
      vcg::Matrix44f vcg_mat;
      LocalAlign::ExecuteLocalAlignVertex(i, neigh_list, vcg_mat, mesh_data);
     
      if(neigh_list.size() <= 1)
      {
        //cout << "neigh_list :" << neigh_list.size() << endl;
        normal_handle[i] = 0.0f;
        integral_handle[i] = 0.0f;
        continue;
      }
      //      LocalAlign::CalIntegralRefl(vcg_mat, neigh_list, i,
      //                                  integral_handle, mesh_data);
      AlignAlgorithm::CalIntegralRefl(i, vcg_mat, integral_handle, mesh_data);
      AlignAlgorithm::CalNormalComponent(i, vcg_mat, normal_handle, mesh_data);
      //FaceIterator fi = mesh_data->FBeginIter(lq::kRight) + i;
    }
  }
  FaceHandleD handle;
  if(flag == lq::kLocalAlignIntegralRefl)
    handle = integral_handle;
  else if(flag == lq::kLocalAlignNormalDivision)
    handle = normal_handle;
  for(FaceIterator fi = mesh_data->FBeginIter(lq::kRight); 
      fi != mesh_data->FEndIter(lq::kRight); ++fi)
  {
    fi->Q() = handle[*fi];
  }
}

void LocalAlign::ExecuteComputeErrorUseVertex(MeshDataPtr &mesh_data)
{
  FaceHandleD normal_handle = mesh_data->GetLocalNormalCom();
  FaceHandleD integral_handle = mesh_data->GetLocalIntegral();
  vector<double> vert_error_inte, vert_error_normal;
  vert_error_inte.resize(mesh_data->VNum(lq::kLeft));
  vert_error_normal.resize(mesh_data->VNum(lq::kLeft));
#pragma omp parallel for num_threads(thread_num(mesh_data->FNum(0), MIN_ITERATOR_NUM))
  for(int i = 0; i < mesh_data->VNum(lq::kLeft); ++i)
  {
    vector<int> neigh_list;
    vcg::Matrix44f vcg_mat;
    int ret = LocalAlign::ExecuteLocalAlignVertex(i, neigh_list, vcg_mat, mesh_data);
    if(ret == 1)
    {
      vert_error_inte[i] = 0.0f;
      vert_error_normal[i] = 0.0f;
      continue;
    }
    AlignAlgorithm::CalIntegralReflVertex(i, vcg_mat, mesh_data, vert_error_inte);
    AlignAlgorithm::CalNormalComponentVertex(i, vcg_mat, mesh_data, vert_error_normal);
    
  }
  BaseAlgorithm::GenerateTriError(mesh_data, vert_error_inte, lq::kRight);
  ofstream fout("local_align.txt");
  for(int i = 0; i < mesh_data->VNum(0); ++i)
  {
    meshdata::VertexIterator vi2 = mesh_data->VBeginIter(1) + i,
        vi1 = mesh_data->VBeginIter(0) + i;
    double diff = vi2->Kh() - vi1->Kh();
    double tmp ;
    if(is_less(diff, sqi::meshdata::EPS_E6))
      tmp = -1.0f;
    else if(is_zero(diff, sqi::meshdata::EPS_E6))
      tmp = 0.0f;
    else
      tmp = 1.0f;
    fout << vert_error_inte[i] * tmp << endl;
  }
  fout.close();
  LocalAlign::SaveResult(vert_error_inte, mesh_data, lq::kRight, integral_handle);
  LocalAlign::SaveResult(vert_error_normal, mesh_data, lq::kRight, normal_handle);
}

void LocalAlign::ExecuteLocalAlignFace(const size_t idx, std::vector<int> &neigh_list,
                                       vcg::Matrix44f& mat,meshdata::MeshDataPtr &mesh)
{
  FaceIterator fi1, fi2;
 
  Geodesic<Mesh> dijsktra;
  FaceType* fptr_left = &mesh->M(lq::kLeft).face[idx];
  dijsktra.FaceDijsktraCompute(mesh->M(lq::kLeft), fptr_left,
                               mesh->LocalRadius(), neigh_list);
  vtkSmartPointer<vtkPoints> points_left = vtkSmartPointer<vtkPoints>::New();
  vtkSmartPointer<vtkPoints> points_right = vtkSmartPointer<vtkPoints>::New();
  for(size_t i = 0; i < neigh_list.size(); ++i)
  { 
    fi1 = mesh->FBeginIter(lq::kLeft) + neigh_list[i];
    fi2 = mesh->FBeginIter(lq::kRight) + neigh_list[i];
    for(size_t j = 0; j < 3; ++j)
    {
      points_left->InsertNextPoint(fi1->cP(j)[0], fi1->cP(j)[1], fi1->cP(j)[2]);
      points_right->InsertNextPoint(fi2->cP(j)[0], fi2->cP(j)[1], fi2->cP(j)[2]);      
    }
  }
  vtkSmartPointer<vtkLandmarkTransform> land_mark_trans = 
      vtkSmartPointer<vtkLandmarkTransform>::New();
  
  AlignAlgorithm::GenerateRigidTransMatrix4x4(points_right, points_left, 
                                              land_mark_trans);
  vtkSmartPointer<vtkMatrix4x4> vtk_mat = vtkSmartPointer<vtkMatrix4x4>::New();
  vtk_mat = land_mark_trans->GetMatrix();
  AlignAlgorithm::TransMatrix4x4FromVtkToVcg(vtk_mat, mat);
}

int LocalAlign::ExecuteLocalAlignVertex(const size_t idx, std::vector<int> &neigh_list, 
                                         vcg::Matrix44f &mat, MeshDataPtr &mesh)
{
  SearchNeighWithKDTree(idx, neigh_list, mesh);
  ComputeTransferMatrixICP(neigh_list, mat, mesh);
//  ComputeTransferMatrixLS(neigh_list, mat, mesh);
  return 0;
}

void LocalAlign::SearchNeighWithKDTree(const size_t idx, std::vector<int> &neigh_list, 
                                       MeshDataPtr &mesh)
{
//  //FaceType* fptr_right = &mesh->M(lq::kRight).face[idx];
//  VertexType* vptr_right = &mesh->M(lq::kRight).vert[idx];
//  vtkSmartPointer<vtkIdList> idx_list = 
//      vtkSmartPointer<vtkIdList>::New();
//  double ref_point[3] = {vptr_right->cP()[0], vptr_right->cP()[1],
//                         vptr_right->cP()[2]};
////  double ref_point[3] = {fptr_right->cP(0)[0], fptr_right->cP(0)[1],
////                         fptr_right->cP(0)[2]};
//  LocalAlign::kd_tree_->FindPointsWithinRadius(mesh->LocalRadius(), ref_point, idx_list);
//  if(idx == 0)
//  {
//    cout << "num : " << idx_list->GetNumberOfIds() << endl;
//  }
//  for(size_t i = 0; i < idx_list->GetNumberOfIds(); ++i)
//  {
//    neigh_list.push_back(idx_list->GetId(i));
//  }
  //set<int> tmp;
//  FaceIterator fptr_right = mesh->FBeginIter(lq::kRight) + idx;
//  vcg::Point3f bary_center = vcg::Barycenter(*fptr_right);
//  vtkSmartPointer<vtkIdList> idx_list = 
//      vtkSmartPointer<vtkIdList>::New();
//  double ref_point[3] = {bary_center[0], bary_center[1],
//                         bary_center[2]};
//  LocalAlign::kd_tree_->FindPointsWithinRadius(mesh->LocalRadius(), ref_point, idx_list);
//  if(idx == 0)
//  {
//    cout << "num : " << idx_list->GetNumberOfIds() << endl;
//  }
//  for(size_t i = 0; i < idx_list->GetNumberOfIds(); ++i)
//  {
//    neigh_list.push_back(idx_list->GetId(i));
//  }
  //std::set<int> tmp;
  FaceIterator fi = mesh->FBeginIter(lq::kRight) + idx;
  for(size_t i = 0; i < 3; ++i)
  {
    vtkSmartPointer<vtkIdList> idx_list = 
        vtkSmartPointer<vtkIdList>::New();
    double ref_point[3] = {fi->cP(i)[0], fi->cP(i)[1], 
                          fi->cP(i)[2]};
    data_set_->KdTree()->FindPointsWithinRadius(mesh->LocalRadius(), ref_point, idx_list);
    for(size_t i = 0; i < idx_list->GetNumberOfIds(); ++i)
    {
      //tmp.insert(idx_list->GetId(i));
      neigh_list.push_back(idx_list->GetId(i));
    }
  }
//  std::set<int>::iterator iter;
  
//  for(iter = tmp.begin(); iter != tmp.end(); ++iter)
//    neigh_list.push_back(*iter);
}

void LocalAlign::CalIntegralRefl(const vcg::Matrix44f &mat, const std::vector<int> &neigh_list, 
                                 const size_t index, meshdata::FaceHandleD& handle,
                                 meshdata::MeshDataPtr &mesh_data)
{
  double diff = 0.0f;
  double sum_area = 0.0f;
  for(size_t i = 0; i < neigh_list.size(); ++i)
  {
    Mesh::FaceIterator tmp_fi1 = mesh_data->FBeginIter(kLeft);
    Mesh::FaceIterator tmp_fi2 = mesh_data->FBeginIter(kRight);
    
    tmp_fi1 += neigh_list[i];
    tmp_fi2 += neigh_list[i];
    
    vcg::Point4f normal1(tmp_fi1->cN()[0], tmp_fi1->cN()[1], tmp_fi1->cN()[2], 0.0f);
    vcg::Point4f normal2(tmp_fi2->cN()[0], tmp_fi2->cN()[1], tmp_fi2->cN()[2], 0.0f);
    normal2 = mat * normal2;
    float angle = vcg::Angle<float>(normal1, normal2);
    angle = (angle * 180.0f) / lq::PI;
    int diff_idx = static_cast<int>(angle * 100);
    if(diff_idx > mesh_data->IntegralReflList().size() - 1)
      diff_idx = mesh_data->IntegralReflList().size() - 1;
    float tri_area = vcg::DoubleArea<Mesh::FaceType>((*tmp_fi2));
    sum_area += tri_area;
    diff += mesh_data->IntegralReflList()[diff_idx] * tri_area;
  }
  FaceIterator fi2 = mesh_data->FBeginIter(kRight) + index;
 // FaceHandleD handle = mesh_data->GetLocalIntegral();
  if(!is_zero(sum_area))
  {
    handle[*fi2] = diff / sum_area;
  }
}

//void LocalAlign::CalIntegralReflNonMutlScalar(const vcg::Matrix44d &mat, 
//                                              const size_t index, FaceHandleD &handle,
//                                              MeshDataPtr &mesh_data)
//{
//  sqi::meshdata::FaceIterator fi1 = mesh_data->FBeginIter(lq::kLeft);
//  sqi::meshdata::FaceIterator fi2 = mesh_data->FBeginIter(lq::kRight);
//  fi1 += index;
//  fi2 += index;
//  vcg::Point4d normal1(fi1->cN()[0], fi1->cN()[1], fi1->cN()[2], 0.0f);
//  vcg::Point4d normal2(fi2->cN()[0], fi2->cN()[1], fi2->cN()[2], 0.0f);
//  normal2 = mat * normal2;
//  double angle = vcg::Angle<double>(normal1, normal2);
//  angle = (angle * 180.0f) / lq::PI;
//  int diff_idx = static_cast<int>(angle * 100);
//  if(diff_idx > mesh_data->IntegralReflList().size() - 1)
//    diff_idx = mesh_data->IntegralReflList().size() - 1;
//  handle[*fi2] = mesh_data->IntegralReflList()[diff_idx];
//}

void LocalAlign::CalNormalComponent(const vcg::Matrix44f &mat, const size_t idx,
                                    meshdata::MeshDataPtr &mesh_data)
{
  
}


void LocalAlign::SaveResult(const std::vector<double> &vert_error, 
                            const meshdata::MeshDataPtr& mesh_data,
                            lq::WindowFlag win,
                            meshdata::FaceHandleD &handle)
{
  for(meshdata::Mesh::FaceIterator fi = mesh_data->M(win).face.begin();
      fi != mesh_data->M(win).face.end(); ++fi)
  {
    double tmp_error = 0.0f;
    for(size_t i = 0; i < 3; ++i)
    {
      tmp_error += vert_error[mesh_data->GetIndexVertex((*fi).V(i), win)];
    }
    handle[*fi] = tmp_error;
  }
}  

void LocalAlign::ComputeTransferMatrixICP(std::vector<int> &neigh_list, vcg::Matrix44f &mat, 
                                          MeshDataPtr &mesh)
{
  if(neigh_list.size() <= 1)
    return ;
  vtkSmartPointer<vtkPoints> points_left = vtkSmartPointer<vtkPoints>::New();
  vtkSmartPointer<vtkPoints> points_right = vtkSmartPointer<vtkPoints>::New();
  VertexIterator vi1, vi2;
  for(size_t i = 0; i < neigh_list.size(); ++i)
  {
    vi1 = mesh->VBeginIter(lq::kLeft) + neigh_list[i];
    vi2 = mesh->VBeginIter(lq::kRight) + neigh_list[i];
    points_left->InsertNextPoint(vi1->cP()[0], vi1->cP()[1], vi1->cP()[2]);
    points_right->InsertNextPoint(vi2->cP()[0], vi2->cP()[1], vi2->cP()[2]);
  }
  vtkSmartPointer<vtkLandmarkTransform> land_mark_trans = 
      vtkSmartPointer<vtkLandmarkTransform>::New();
  AlignAlgorithm::GenerateRigidTransMatrix4x4(points_right, points_left, 
                                              land_mark_trans);
  vtkSmartPointer<vtkMatrix4x4> vtk_mat = vtkSmartPointer<vtkMatrix4x4>::New();
  vtk_mat = land_mark_trans->GetMatrix();
  AlignAlgorithm::TransMatrix4x4FromVtkToVcg(vtk_mat, mat);
}

void LocalAlign::ComputeTransferMatrixLS(std::vector<int> &neigh_list, vcg::Matrix44f &mat,
                                         MeshDataPtr &mesh)
{
  if(neigh_list.size() <= 1)
    return;
  VtkLeastSquare<meshdata::Mesh, vcg::Matrix33f> vtk_svd;
  vtk_svd.LocalLeastSquare(mesh->M(lq::kRight), mesh->M(lq::kLeft),
                           neigh_list);
  mat = vtk_svd.GetTransferMatrix();
}

}
}
