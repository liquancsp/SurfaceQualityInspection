
#include <menu_operate/LocalRegisterIntegralMap.h>
#include <IntegrationReflection.h>
#include <Topology/Geodesic.h>
#include <CurvatureAlg.h>

#include <vcg/math/matrix44.h>
#include <vcg/space/deprecated_point4.h>

#include <vtkTransformPolyDataFilter.h>
#include <vtkCellArray.h>
#include <vtkIdList.h>
#include <vtkTriangle.h>
#include <vtkMatrix4x4.h>

#include <ctime>
#include <iomanip>
#include <sstream>


namespace sqi{
namespace alg{

using namespace sqi::meshdata;
using namespace sqi::alg;
using namespace std;
using namespace lq;

LocalRegisterIntegralMap::LocalRegisterIntegralMap() : BaseAlgorithm(), LocalAlign()
{
  
}

LocalRegisterIntegralMap::~LocalRegisterIntegralMap()
{}

void LocalRegisterIntegralMap::cal_tri_error(meshdata::MeshDataPtr &mesh_data, lq::InspectFlag flag)
{
  IntegrationReflection::GenerateIntegralReflMapList(mesh_data);
  std::cout << "pre radius : " << mesh_data->PreLocalRadius() << " radius : " <<
               mesh_data->LocalRadius() << std::endl;
  clock_t start, end;
  start = clock();
  
  stringstream s1 ;
  s1 << mesh_data->PreLocalRadius();
  stringstream s2;
  s2 << mesh_data->LocalRadius();
  bool is_computed = (strcmp(s1.str().c_str(), s2.str().c_str()) == 0);
  LocalAlign::ExecuteComputeErrorUseFace(mesh_data, flag, is_computed);
  CurvatureAlg<Mesh>::UpdateConcavAndConvex(mesh_data->M(lq::kLeft),
                                            mesh_data->M(lq::kRight));
  BaseAlgorithm::MixAlgWithConvex(mesh_data, lq::kRight);  
  BaseAlgorithm::GenerateFinalScalarField(mesh_data, flag);
  end = clock();
  io::ConsoleMessage<string>::SendMessage("", io::kEndAlg, lq::kNull, mesh_data->MaxE(),
                                          difftime(end, start)/CLOCKS_PER_SEC);
}

void LocalRegisterIntegralMap::DoTransformation(vtkSmartPointer<vtkPolyData> &poly, 
                                                vtkSmartPointer<vtkLandmarkTransform> land_mark_transform)
{
  vtkSmartPointer<vtkTransformPolyDataFilter> transform_filter = 
      vtkSmartPointer<vtkTransformPolyDataFilter>::New();
#if VTK_MAJOR_VERSION <= 5
  transform_filter->SetInput(poly);
#else
  transform_filter->SetInputData(poly);
#endif
  transform_filter->SetTransform(land_mark_transform);
  transform_filter->Update();
  poly->ShallowCopy(transform_filter->GetOutput());
}


void LocalRegisterIntegralMap::CalError(const vcg::Matrix44d& mat,
                                        const vector<int> &neigh_list, 
                                        const size_t index, 
                                        MeshDataPtr &mesh_data)
{
  double diff = 0.0f;
  double sum_area = 0.0f;
  
  for(size_t i = 0; i < neigh_list.size(); ++i)
  {
    Mesh::FaceIterator tmp_fi1 = mesh_data->FBeginIter(kLeft);
    Mesh::FaceIterator tmp_fi2 = mesh_data->FBeginIter(kRight);
    
    tmp_fi1 += neigh_list[i];
    tmp_fi2 += neigh_list[i];
    
    vcg::Point4d normal1(tmp_fi1->cN()[0], tmp_fi1->cN()[1], tmp_fi1->cN()[2], 0.0f);
    vcg::Point4d normal2(tmp_fi2->cN()[0], tmp_fi2->cN()[1], tmp_fi2->cN()[2], 0.0f);
    normal2 = mat * normal2;
    double angle = vcg::Angle<double>(normal1, normal2);
    angle = (angle * 180.0f) / PI;
    int diff_idx = static_cast<int>(angle * 100);
    if(diff_idx > mesh_data->IntegralReflList().size() - 1)
      diff_idx = mesh_data->IntegralReflList().size() - 1;
    double tri_area = vcg::DoubleArea<Mesh::FaceType>((*tmp_fi2));
    sum_area += tri_area;
    diff += mesh_data->IntegralReflList()[diff_idx] * tri_area;
  }
  FaceIterator fi2 = mesh_data->FBeginIter(kRight) + index;
  if(!is_zero(sum_area))
    (*fi2).Q() = diff / sum_area;
}

}
}
