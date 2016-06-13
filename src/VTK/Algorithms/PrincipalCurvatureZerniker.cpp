#include <PrincipalCurvatureZerniker.h>
#include <CurvatureAlg.h>
#include <ExportTrainningData.h>

#include <data_type.h>

#include <vtkIdList.h>

using namespace std;
using namespace sqi::io;
using namespace lq;
namespace sqi {
namespace alg{

PrincipalCurvatureZerniker::PrincipalCurvatureZerniker() : SurfaceDescriptor()
{
  
}

PrincipalCurvatureZerniker::~PrincipalCurvatureZerniker()
{
  
}

void PrincipalCurvatureZerniker::Descriptor(meshdata::MeshDataPtr &mesh_data, 
                                            vtkSmartPointer<lq::vtk_model_data> &data_set, 
                                            lq::WindowFlag win, lq::InspectFlag flag)
{
  assert(data_set != NULL && mesh_data.use_count() != 0);
  cout << "[#Info-alg]Principal curvature descriptor." << endl;
  timer_->StartTimer();
  double bound[6];
  std::vector<func_ptr> radial_poly;
  double radius;
  SurfaceDescriptor::InitializeParameters(mesh_data, data_set, win, bound);
  SurfaceDescriptor::InitializeParameters(mesh_data, radial_poly, bound, radius);
  
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
      if(flag == lq::kCalK1ZernikerCoeff)
        neighbor_mean[j] = tmp_vi->K1();
      else if(flag == lq::kCalK2ZernikerCoeff)
        neighbor_mean[j] = tmp_vi->K2();
    }
    SurfaceDescriptor::CalZernikerCoeff(mesh_data, neighbor_mean, idx_list,
                                        radial_poly, i, win, gauss_zerniker);
  }
  SurfaceDescriptor::CalGaussianWeightZernikeCoeff(mesh_data, data_set->KdTree(),
                                                   gauss_zerniker, zer_coeff_gauss,
                                                   radius * mesh_data->HeightRadiusMulti(), win);
  string out_file;
  string flag_str;
  if(flag == lq::kCalK1ZernikerCoeff)
    flag_str = "_zerniker_zk1.test";
  else if(flag == lq::kCalK2ZernikerCoeff)
    flag_str = "_zerniker_zk2.test";
  SurfaceDescriptor::GenerateExportFileName(data_set->ModelName(win),
                                            flag_str, out_file);
  
  ExportTrainningData::ExportZernikerTestData(out_file, zer_coeff_gauss);
  timer_->StopTimer();
  cout << "[#Info]Principal curvature zerniker coefficient cost : " << timer_->GetElapsedTime() << endl;
}


}

}
