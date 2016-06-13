#include <CurvatureSmoothDiffZerniker.h>
#include <CurvatureAlg.h>
#include <ExportTrainningData.h>
#include <data_type.h>

#include <vtkIdList.h>
#include <vtkCellArray.h>

#include <vcg/complex/algorithms/smooth.h>

using namespace std;
using namespace lq;
using namespace sqi::io;
using namespace sqi::meshdata;

namespace sqi {
namespace alg {

CurvatureSmoothDiffZerniker::CurvatureSmoothDiffZerniker() : SurfaceDescriptor()
{
  
}

CurvatureSmoothDiffZerniker::~CurvatureSmoothDiffZerniker()
{
  
}

void CurvatureSmoothDiffZerniker::Descriptor(meshdata::MeshDataPtr &mesh_data, 
                                             vtkSmartPointer<lq::vtk_model_data> &data_set, 
                                             lq::WindowFlag win, lq::InspectFlag flag)
{
  assert(data_set != NULL && mesh_data.use_count() != 0);
  cout << "[#Info-alg]Smooth Curvature different descriptor." << endl;
  timer_->StartTimer();
  double bound[6];
  std::vector<func_ptr> radial_poly;
  double radius;
  //initialize smooth mesh data
  Mesh smooth_m;
  vcg::tri::Append<Mesh, Mesh>::Mesh(smooth_m, mesh_data->M(win), false, true);
//  vcg::tri::Smooth<Mesh>::VertexCoordLaplacianHC(smooth_m, mesh_data->SmoothStep());
  vcg::tri::Smooth<Mesh>::VertexCoordScaleDependentLaplacian_Fujiwara(smooth_m, mesh_data->SmoothStep(), 0.5);
  CurvatureAlg<sqi::meshdata::Mesh>::MeanAndGaussian(smooth_m);
  CurvatureAlg<meshdata::Mesh>::PrincipalCurvatureAndDirection(smooth_m);
  CurvatureAlg<meshdata::Mesh>::PrincipalCurWithDirection(smooth_m);
  //create smooth mesh's KD tree
  vtkSmartPointer<vtkPolyData> smooth_poly_data = 
      vtkSmartPointer<vtkPolyData>::New();
  CreateVtkPolyData(smooth_poly_data, smooth_m);
  vtkSmartPointer<vtkKdTreePointLocator> smooth_kdtree = 
      vtkSmartPointer<vtkKdTreePointLocator>::New();
  smooth_kdtree->SetDataSet(smooth_poly_data);
  smooth_kdtree->SetMaxLevel(8);
  smooth_kdtree->BuildLocator();
  //initialize some parameters which need for algorithm
  SurfaceDescriptor::InitializeParameters(mesh_data, data_set, win, bound);
  SurfaceDescriptor::InitializeParameters(mesh_data, radial_poly, bound, radius);
  cout << "[#Info]radius is : " << radius << endl;
  vector<vector<vector<double> > > cur_zerniker(4, vector<vector<double> >(mesh_data->VNum(win)));
  vector<vector<vector<double> > > smooth_cur_zerniker(4, vector<vector<double> >(mesh_data->VNum(win)));
#pragma omp parallel for num_threads(4/*thread_num(mesh_data->FNum(0), MIN_ITERATOR_NUM)*/)
  for(int i = 0; i < mesh_data->VNum(win); ++i)
  {
    this->CalCurvatureZerniker(mesh_data->M(win), radial_poly, data_set->KdTree(),
                               cur_zerniker, i, false, radius);
    //calculate the smooth mesh's zerniker coefficient and get the different between it and original.
    this->CalCurvatureZerniker(smooth_m, radial_poly, smooth_kdtree, 
                               smooth_cur_zerniker, i, false, radius);
  }
//  string out_file;
  
  cout << "[#Info]Complete zerniker coeffcient." << endl;
  vector<vector<double> > zer_coeff(mesh_data->VNum(win), vector<double>(25, 0.0f));
  SurfaceDescriptor::CalGaussianWeightZernikeCoeff(mesh_data->M(win), data_set->KdTree(),
                                                   cur_zerniker[0], zer_coeff,
      radius * mesh_data->HeightRadiusMulti(), false);

  SurfaceDescriptor::CalGaussianWeightZernikeCoeff(smooth_m, smooth_kdtree,
                                                   smooth_cur_zerniker[0], zer_coeff,
      radius * mesh_data->HeightRadiusMulti(), true);
   string out_file;
  
  SurfaceDescriptor::GenerateExportFileName(data_set->ModelName(win),
                                            "_zerniker_sdmean.test", out_file);
  
  ExportTrainningData::ExportZernikerTestData(out_file, zer_coeff);
  
  zer_coeff.clear();
  SurfaceDescriptor::CalGaussianWeightZernikeCoeff(mesh_data->M(win), data_set->KdTree(),
                                                   cur_zerniker[1], zer_coeff,
      radius * mesh_data->HeightRadiusMulti(), false);
  SurfaceDescriptor::CalGaussianWeightZernikeCoeff(smooth_m, smooth_kdtree,
                                                   smooth_cur_zerniker[1], zer_coeff,
      radius * mesh_data->HeightRadiusMulti(), true);
  SurfaceDescriptor::GenerateExportFileName(data_set->ModelName(win),
                                            "_zerniker_sdgauss.test", out_file);
  
  ExportTrainningData::ExportZernikerTestData(out_file, zer_coeff);
  zer_coeff.clear();
  SurfaceDescriptor::CalGaussianWeightZernikeCoeff(mesh_data->M(win), data_set->KdTree(),
                                                   cur_zerniker[2], zer_coeff,
      radius * mesh_data->HeightRadiusMulti(), false);
  SurfaceDescriptor::CalGaussianWeightZernikeCoeff(smooth_m, smooth_kdtree,
                                                   smooth_cur_zerniker[2], zer_coeff,
      radius * mesh_data->HeightRadiusMulti(), true);
  SurfaceDescriptor::GenerateExportFileName(data_set->ModelName(win),
                                            "_zerniker_sdk1.test", out_file);
  
  ExportTrainningData::ExportZernikerTestData(out_file, zer_coeff);
  zer_coeff.clear();
  SurfaceDescriptor::CalGaussianWeightZernikeCoeff(mesh_data->M(win), data_set->KdTree(),
                                                   cur_zerniker[3], zer_coeff,
      radius * mesh_data->HeightRadiusMulti(), false);
  SurfaceDescriptor::CalGaussianWeightZernikeCoeff(smooth_m, smooth_kdtree,
                                                   smooth_cur_zerniker[3], zer_coeff,
      radius * mesh_data->HeightRadiusMulti(), true);
  SurfaceDescriptor::GenerateExportFileName(data_set->ModelName(win),
                                            "_zerniker_sdk2.test", out_file);
  
  ExportTrainningData::ExportZernikerTestData(out_file, zer_coeff);
  
  timer_->StopTimer();
  cout << "[#Info]curvature zerniker coefficient cost : " << timer_->GetElapsedTime() << endl;
}

void CurvatureSmoothDiffZerniker::CreateVtkPolyData(vtkSmartPointer<vtkPolyData> &poly_data, 
                                                    const Mesh &mesh)
{
  vtkSmartPointer<vtkPoints> vtk_points = vtkSmartPointer<vtkPoints>::New();
  for(ConstVertexIterator vi = mesh.vert.begin(); vi != mesh.vert.end(); ++vi)
    vtk_points->InsertNextPoint(vi->P()[0], vi->P()[1], vi->P()[2]);
  vtkSmartPointer<vtkCellArray> triangle_cell =
      vtkSmartPointer<vtkCellArray>::New();
  vtkIdType pts[3];
  for(ConstFaceIterator fi = mesh.face.begin(); 
      fi != mesh.face.end(); ++fi)
  {
    pts[0] =  GetIndex<ConstVertexPointer, VertContainer>((*fi).cV(0), mesh.vert);
    pts[1] =  GetIndex<ConstVertexPointer, VertContainer>((*fi).cV(1), mesh.vert);
    pts[2] =  GetIndex<ConstVertexPointer, VertContainer>((*fi).cV(2), mesh.vert);
    triangle_cell->InsertNextCell(3, pts);
  }
  poly_data->SetPoints(vtk_points);
  poly_data->SetPolys(triangle_cell);
}

void CurvatureSmoothDiffZerniker::CalCurvatureZerniker(Mesh &mesh, std::vector<func_ptr>& radial_poly,
                                                       vtkSmartPointer<vtkKdTreePointLocator> &kd_tree, 
                                                       std::vector<std::vector<std::vector<double> > > &cur_zerniker,
                                                       int idx, bool is_diff, double radius)
{
  sqi::meshdata::VertexIterator vi =  mesh.vert.begin() + idx;
  vtkSmartPointer<vtkIdList> idx_list = 
      vtkSmartPointer<vtkIdList>::New();
  double ref_point[3] = {vi->cP()[0], vi->cP()[1], vi->cP()[2]};
  kd_tree->FindPointsWithinRadius(radius, ref_point, idx_list);
  vector<double> neighbor_mean(idx_list->GetNumberOfIds(), 0.0f);
  vector<double> neighbor_gauss(idx_list->GetNumberOfIds(), 0.0f);
  vector<double> neighbor_k1(idx_list->GetNumberOfIds(), 0.0f);
  vector<double> neighbor_k2(idx_list->GetNumberOfIds(), 0.0f);
  for(int j = 0; j < idx_list->GetNumberOfIds(); ++j)
  {
    sqi::meshdata::VertexIterator tmp_vi = mesh.vert.begin() + idx_list->GetId(j);
    neighbor_mean[j] = tmp_vi->Kh();
    neighbor_gauss[j] = tmp_vi->Kg();
    neighbor_k1[j] = tmp_vi->K1();
    neighbor_k2[j] = tmp_vi->K2();
  }
  SurfaceDescriptor::CalZernikerCoeff(mesh, neighbor_mean, idx_list,
                                      radial_poly, idx,  cur_zerniker[0], is_diff);
  SurfaceDescriptor::CalZernikerCoeff(mesh, neighbor_gauss, idx_list,
                                      radial_poly, idx, cur_zerniker[1], is_diff);
  SurfaceDescriptor::CalZernikerCoeff(mesh, neighbor_k1, idx_list,
                                      radial_poly, idx, cur_zerniker[2], is_diff);
  SurfaceDescriptor::CalZernikerCoeff(mesh, neighbor_k2, idx_list,
                                      radial_poly, idx, cur_zerniker[3], is_diff);
}

}

}
