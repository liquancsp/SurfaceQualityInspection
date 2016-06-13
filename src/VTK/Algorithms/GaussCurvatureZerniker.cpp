#include <GaussCurvatureZerniker.h>
#include <CurvatureAlg.h>
#include <ExportTrainningData.h>
#include <data_type.h>

#include <vtkIdList.h>

using namespace std;
using namespace sqi::io;
using namespace lq;
namespace sqi
{
namespace alg{

GaussCurvatureZerniker::GaussCurvatureZerniker() : SurfaceDescriptor()
{
  
}

GaussCurvatureZerniker::~GaussCurvatureZerniker()
{
  
}

void GaussCurvatureZerniker::Descriptor(meshdata::MeshDataPtr &mesh_data,
                                        vtkSmartPointer<lq::vtk_model_data> &data_set,
                                        lq::WindowFlag win, lq::InspectFlag flag)
{
  assert(data_set != NULL && mesh_data.use_count() != 0);
//  cout << "[#Info-alg]Gaussian curvature descriptor." << endl;
//  timer_->StartTimer();
//  double bound[6];
//  std::vector<func_ptr> radial_poly;
//  double radius;
//  SurfaceDescriptor::InitializeParameters(mesh_data, data_set, win, bound);
//  SurfaceDescriptor::InitializeParameters(mesh_data, radial_poly, bound, radius);
  
//  vector<vector<double> > gauss_zerniker(mesh_data->VNum(win));
//  vector<vector<double> > zer_coeff_gauss(mesh_data->VNum(win), vector<double>(25, 0.0f));
//#pragma omp parallel for num_threads(thread_num(mesh_data->FNum(0), MIN_ITERATOR_NUM))
//  for(int i = 0; i < mesh_data->VNum(win); ++i) 
//  {
//    cout << "id : " << i << endl;
//    sqi::meshdata::VertexIterator vi =  mesh_data->VBeginIter(win) + i;
//    vtkSmartPointer<vtkIdList> idx_list = 
//        vtkSmartPointer<vtkIdList>::New();
//    double ref_point[3] = {vi->cP()[0], vi->cP()[1], vi->cP()[2]};
//    data_set->KdTree()->FindPointsWithinRadius(radius, ref_point, idx_list);
//    vector<double> neighbor_mean(idx_list->GetNumberOfIds(), 0.0f);
//    for(int j = 0; j < idx_list->GetNumberOfIds(); ++j)
//    {
//      sqi::meshdata::VertexIterator tmp_vi = mesh_data->VBeginIter(win) + idx_list->GetId(j);
//      neighbor_mean[j] = tmp_vi->Kg();
//    }
//    SurfaceDescriptor::CalZernikerCoeff(mesh_data, neighbor_mean, idx_list,
//                                        radial_poly, i, win, gauss_zerniker);
//  }
//  SurfaceDescriptor::CalGaussianWeightZernikeCoeff(mesh_data, data_set->KdTree(),
//                                                   gauss_zerniker, zer_coeff_gauss,
//                                                   radius * mesh_data->HeightRadiusMulti(), win);
//  string out_file;
//  SurfaceDescriptor::GenerateExportFileName(data_set->ModelName(win),
//                                            "_zerniker_zgauss.test", out_file);
  
//  ExportTrainningData::ExportZernikerTestData(out_file, zer_coeff_gauss);
//  timer_->StopTimer();
//  cout << "[#Info]Gaussian curvature zerniker coefficient cost : " << timer_->GetElapsedTime() << endl;
  CalLocalAlignIntegralDescriptor(mesh_data, data_set, win, flag);
}

void GaussCurvatureZerniker::CalLocalAlignIntegralDescriptor(meshdata::MeshDataPtr &mesh_data, 
                                                             vtkSmartPointer<vtk_model_data> &data_set,
                                                             WindowFlag win, InspectFlag flag)
{
  timer_->StartTimer();
  double bound[6];
  std::vector<func_ptr> radial_poly;
  double radius;
  SurfaceDescriptor::InitializeParameters(mesh_data, data_set, win, bound);
  SurfaceDescriptor::InitializeParameters(mesh_data, radial_poly, bound, radius);
  ifstream fin("local_align.txt");
  string input;
  vector<double> vertex_value;
  while(!fin.eof())
  {
    getline(fin, input);
    vertex_value.push_back(atof(input.c_str()));
  }
  fin.close();
  vector<vector<double> > gauss_zerniker(mesh_data->VNum(win));
  vector<vector<double> > zer_coeff_gauss(mesh_data->VNum(win), vector<double>(25, 0.0f));
#pragma omp parallel for num_threads(thread_num(mesh_data->FNum(0), MIN_ITERATOR_NUM))
  for(int i = 0; i < mesh_data->VNum(win); ++i) 
  {
    cout << "id : " << i << endl;
    sqi::meshdata::VertexIterator vi =  mesh_data->VBeginIter(win) + i;
    vtkSmartPointer<vtkIdList> idx_list = 
        vtkSmartPointer<vtkIdList>::New();
    double ref_point[3] = {vi->cP()[0], vi->cP()[1], vi->cP()[2]};
    data_set->KdTree()->FindPointsWithinRadius(radius, ref_point, idx_list);
    vector<double> neighbor_mean(idx_list->GetNumberOfIds(), 0.0f);
    for(int j = 0; j < idx_list->GetNumberOfIds(); ++j)
    {
      sqi::meshdata::VertexIterator tmp_vi = mesh_data->VBeginIter(win) + idx_list->GetId(j);
      neighbor_mean[j] = vertex_value[idx_list->GetId(j)];
    }
    SurfaceDescriptor::CalZernikerCoeff(mesh_data, neighbor_mean, idx_list,
                                        radial_poly, i, win, gauss_zerniker);
  }
  SurfaceDescriptor::CalGaussianWeightZernikeCoeff(mesh_data, data_set->KdTree(),
                                                   gauss_zerniker, zer_coeff_gauss,
                                                   radius * mesh_data->HeightRadiusMulti(), win);
  string out_file;
  SurfaceDescriptor::GenerateExportFileName(data_set->ModelName(win),
                                            "_zerniker_zlocalalign.test", out_file);
  
  ExportTrainningData::ExportZernikerTestData(out_file, zer_coeff_gauss);
  timer_->StopTimer();
  cout << "[#Info]Gaussian curvature zerniker coefficient cost : " << timer_->GetElapsedTime() << endl;
}

}
}
