#include <CurvatureZerniker.h>
#include <CurvatureAlg.h>
#include <ExportTrainningData.h>
#include <data_type.h>

#include <vtkIdList.h>

using namespace std;
using namespace sqi::io;
using namespace lq;

namespace sqi{
namespace alg{

CurvatureZerniker::CurvatureZerniker() : SurfaceDescriptor()
{
  
}

CurvatureZerniker::~CurvatureZerniker()
{
  
}

void CurvatureZerniker::Descriptor(meshdata::MeshDataPtr &mesh_data, 
                                   vtkSmartPointer<vtk_model_data> &data_set, 
                                   WindowFlag win, InspectFlag flag)
{
  assert(data_set != NULL && mesh_data.use_count() != 0);
  cout << "[#Info-alg]Curvature descriptor." << endl;
  timer_->StartTimer();
  double bound[6];
  std::vector<func_ptr> radial_poly;
  double radius;
  SurfaceDescriptor::InitializeParameters(mesh_data, data_set, win, bound);
  SurfaceDescriptor::InitializeParameters(mesh_data, radial_poly, bound, radius);
  cout << "[#Info]radius is : " << radius << endl;
  vector<vector<double> > mean_zerniker(mesh_data->VNum(win));
  vector<vector<double> > gauss_zerniker(mesh_data->VNum(win));
  vector<vector<double> > k1_zerniker(mesh_data->VNum(win));
  vector<vector<double> > k2_zerniker(mesh_data->VNum(win));
  
//  vector<vector<double> > zer_coeff_gauss(mesh_data->VNum(win), vector<double>(25, 0.0f));
#pragma omp parallel for num_threads(thread_num(mesh_data->FNum(0), MIN_ITERATOR_NUM))
  for(int i = 0; i < mesh_data->VNum(win); ++i) 
  {
    sqi::meshdata::VertexIterator vi =  mesh_data->VBeginIter(win) + i;
    vtkSmartPointer<vtkIdList> idx_list = 
        vtkSmartPointer<vtkIdList>::New();
    double ref_point[3] = {vi->cP()[0], vi->cP()[1], vi->cP()[2]};
    data_set->KdTree()->FindPointsWithinRadius(radius, ref_point, idx_list);
    vector<double> neighbor_mean(idx_list->GetNumberOfIds(), 0.0f);
    vector<double> neighbor_gauss(idx_list->GetNumberOfIds(), 0.0f);
    vector<double> neighbor_k1(idx_list->GetNumberOfIds(), 0.0f);
    vector<double> neighbor_k2(idx_list->GetNumberOfIds(), 0.0f);
    for(int j = 0; j < idx_list->GetNumberOfIds(); ++j)
    {
      sqi::meshdata::VertexIterator tmp_vi = mesh_data->VBeginIter(win) + idx_list->GetId(j);
      neighbor_mean[j] = tmp_vi->Kh();
      neighbor_gauss[j] = tmp_vi->Kg();
      neighbor_k1[j] = tmp_vi->K1();
      neighbor_k2[j] = tmp_vi->K2();
    }
    SurfaceDescriptor::CalZernikerCoeff(mesh_data, neighbor_mean, idx_list,
                                        radial_poly, i, win, mean_zerniker);
    SurfaceDescriptor::CalZernikerCoeff(mesh_data, neighbor_gauss, idx_list,
                                        radial_poly, i, win, gauss_zerniker);
    SurfaceDescriptor::CalZernikerCoeff(mesh_data, neighbor_k1, idx_list,
                                        radial_poly, i, win, k1_zerniker);
    SurfaceDescriptor::CalZernikerCoeff(mesh_data, neighbor_k2, idx_list,
                                        radial_poly, i, win, k2_zerniker);
  }
  vector<vector<double> > zer_coeff(mesh_data->VNum(win), vector<double>(25, 0.0f));
  SurfaceDescriptor::CalGaussianWeightZernikeCoeff(mesh_data, data_set->KdTree(),
                                                   mean_zerniker, zer_coeff,
                                                   radius * mesh_data->HeightRadiusMulti(), win);
  string out_file;
  SurfaceDescriptor::GenerateExportFileName(data_set->ModelName(win),
                                            "_zerniker_zmean.test", out_file);
  
  ExportTrainningData::ExportZernikerTestData(out_file, zer_coeff);
  zer_coeff.clear();
  SurfaceDescriptor::CalGaussianWeightZernikeCoeff(mesh_data, data_set->KdTree(),
                                                   gauss_zerniker, zer_coeff,
                                                   radius * mesh_data->HeightRadiusMulti(), win);
  SurfaceDescriptor::GenerateExportFileName(data_set->ModelName(win),
                                            "_zerniker_zgauss.test", out_file);
  
  ExportTrainningData::ExportZernikerTestData(out_file, zer_coeff);
  zer_coeff.clear();
  SurfaceDescriptor::CalGaussianWeightZernikeCoeff(mesh_data, data_set->KdTree(),
                                                   k1_zerniker, zer_coeff,
                                                   radius * mesh_data->HeightRadiusMulti(), win);
  SurfaceDescriptor::GenerateExportFileName(data_set->ModelName(win),
                                            "_zerniker_zk1.test", out_file);
  
  ExportTrainningData::ExportZernikerTestData(out_file, zer_coeff);
  zer_coeff.clear();
  SurfaceDescriptor::CalGaussianWeightZernikeCoeff(mesh_data, data_set->KdTree(),
                                                   k2_zerniker, zer_coeff,
                                                   radius * mesh_data->HeightRadiusMulti(), win);
  SurfaceDescriptor::GenerateExportFileName(data_set->ModelName(win),
                                            "_zerniker_zk2.test", out_file);
  
  ExportTrainningData::ExportZernikerTestData(out_file, zer_coeff);
  timer_->StopTimer();
  cout << "[#Info]curvature zerniker coefficient cost : " << timer_->GetElapsedTime() << endl;
}

}
}
