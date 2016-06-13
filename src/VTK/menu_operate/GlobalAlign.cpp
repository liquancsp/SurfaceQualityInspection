#include <menu_operate/GlobalAlign.h>
#include <vtkMatrix4x4.h>

#include <vtkIterativeClosestPointTransform.h>
#include <vtkPolyData.h>
#include <vtkVertexGlyphFilter.h>

namespace sqi{
namespace alg{

GlobalAlign::GlobalAlign() : AlignAlgorithm()
{
  
}

GlobalAlign::~GlobalAlign()
{
  
}

void GlobalAlign::ExecuteGlobalAlgin(meshdata::MeshDataPtr &mesh_data)
{
  vtkSmartPointer<vtkPoints> points_left = 
      vtkSmartPointer<vtkPoints>::New();
  vtkSmartPointer<vtkPoints> points_right = 
      vtkSmartPointer<vtkPoints>::New();
  sqi::meshdata::VertexIterator vi1, vi2;
  for(size_t i = 0; i < mesh_data->VNum(lq::kRight); ++i)
  {
    vi1 = mesh_data->VBeginIter(lq::kLeft) + i;
    vi2 = mesh_data->VBeginIter(lq::kRight) + i;
    points_left->InsertNextPoint(vi1->P()[0], vi1->P()[1], vi1->P()[2]);
    points_right->InsertNextPoint(vi2->P()[0], vi2->P()[1], vi2->P()[2]);
  }
  vtkSmartPointer<vtkLandmarkTransform> land_mark_trans = 
      vtkSmartPointer<vtkLandmarkTransform>::New();
 
  AlignAlgorithm::GenerateRigidTransMatrix4x4(points_right, points_left, land_mark_trans);
  vtkSmartPointer<vtkMatrix4x4> vtk_mat = 
      vtkSmartPointer<vtkMatrix4x4>::New();
  vtk_mat = land_mark_trans->GetMatrix();
  AlignAlgorithm::TransMatrix4x4FromVtkToVcg(vtk_mat, mesh_data->GlobalAlignMat());
  
}

void GlobalAlign::ExecuteGlobalAlginIcp(meshdata::MeshDataPtr &mesh_data)
{
  vtkSmartPointer<vtkPoints> points_left = 
      vtkSmartPointer<vtkPoints>::New();
  vtkSmartPointer<vtkPoints> points_right = 
      vtkSmartPointer<vtkPoints>::New();
  sqi::meshdata::VertexIterator vi1, vi2;
  for(size_t i = 0; i < mesh_data->VNum(lq::kRight); ++i)
  {
    vi1 = mesh_data->VBeginIter(lq::kLeft) + i;
    vi2 = mesh_data->VBeginIter(lq::kRight) + i;
    points_left->InsertNextPoint(vi1->P()[0], vi1->P()[1], vi1->P()[2]);
    points_right->InsertNextPoint(vi2->P()[0], vi2->P()[1], vi2->P()[2]);
  }
  vtkSmartPointer<vtkIterativeClosestPointTransform> icp = 
      vtkSmartPointer<vtkIterativeClosestPointTransform>::New();
  
  vtkSmartPointer<vtkPolyData> poly1 = 
      vtkSmartPointer<vtkPolyData>::New();
  vtkSmartPointer<vtkPolyData> poly2 =
      vtkSmartPointer<vtkPolyData>::New();
  SetIcpPolyData(poly1, points_left);
  SetIcpPolyData(poly2, points_right);
  icp->SetSource(poly2);
  icp->SetTarget(poly1);
  icp->GetLandmarkTransform()->SetModeToRigidBody();
  icp->StartByMatchingCentroidsOn();
  icp->Modified();
  icp->Update();
  vtkSmartPointer<vtkMatrix4x4> vtk_mat = 
      vtkSmartPointer<vtkMatrix4x4>::New();
  vtk_mat = icp->GetMatrix();
  AlignAlgorithm::TransMatrix4x4FromVtkToVcg(vtk_mat, mesh_data->GlobalAlignMat());
}

void GlobalAlign::SetIcpPolyData(vtkSmartPointer<vtkPolyData> &poly_data,
                                 const vtkSmartPointer<vtkPoints>& p)
{
  vtkSmartPointer<vtkPolyData> tmp = 
      vtkSmartPointer<vtkPolyData>::New();
  tmp->SetPoints(p);
  vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter = 
      vtkSmartPointer<vtkVertexGlyphFilter>::New();
#if VTK_MAJOR_VERSION <= 5
  vertexFilter->SetInputConnection(tmp->GetProducerPort());
#else
  vertexFilter->SetInputData(tmp);
#endif
  vertexFilter->Update();
  
  poly_data->ShallowCopy(vertexFilter->GetOutput());
}

}
}
