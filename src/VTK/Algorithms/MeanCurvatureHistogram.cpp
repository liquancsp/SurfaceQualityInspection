#include <MeanCurvatureHistogram.h>
#include <CurvatureAlg.h>
#include <ExportTrainningData.h>
#include <data_type.h>

#include <vtkIdList.h>

using namespace std;
using namespace sqi::io;
using namespace lq;

namespace sqi{
namespace alg{

MeanCurvatureHistogram::MeanCurvatureHistogram() : SurfaceDescriptor()
{
  
}

MeanCurvatureHistogram::~MeanCurvatureHistogram(){
  
}

void MeanCurvatureHistogram::Descriptor(meshdata::MeshDataPtr &mesh_data, 
                                        vtkSmartPointer<vtk_model_data> &data_set,
                                        WindowFlag win, InspectFlag flag)
{
  assert(data_set != NULL && mesh_data.use_count() != 0);
  cout << "[#Info-alg]Mean curvature histogram." << endl;
  timer_->StartTimer();
  sqi::meshdata::MeshHandleB is_have_convex = 
      vcg::tri::Allocator<sqi::meshdata::Mesh>::GetPerMeshAttribute<bool>(mesh_data->M(win), 
                                                                          std::string("IsHaveConvex"));
  if(!is_have_convex())
  {
    CurvatureAlg<sqi::meshdata::Mesh>::MeanAndGaussian(mesh_data->M(win));
    is_have_convex() = true;
  }
  if(!mesh_data->IsHasPrinCur())
  {
    mesh_data->IsHasPrinCur() = true;
    CurvatureAlg<meshdata::Mesh>::PrincipalCurvatureAndDirection(mesh_data->M(win));
    CurvatureAlg<meshdata::Mesh>::PrincipalCurWithDirection(mesh_data->M(win));
  }
  data_set->BuildKdTree(win);
  int num_dim = 70;
  vector<vector<double> > mean_histogram(mesh_data->VNum(win), vector<double>(num_dim, 0.0f));
  vector<vector<double> > gauss_histogram(mesh_data->VNum(win), vector<double>(num_dim, 0.0f));
  vector<vector<double> > k1_histogram(mesh_data->VNum(win), vector<double>(num_dim, 0.0f));
  vector<vector<double> > k2_histogram(mesh_data->VNum(win), vector<double>(num_dim, 0.0f));
  double radius;
  double bound[6];
  data_set->PD(win)->GetBounds(bound);
  SurfaceDescriptor::InitializeParameters(mesh_data, bound, radius);
  cout << "[#Info]Radius is : " << radius << endl;
#pragma omp parallel for num_threads(thread_num(mesh_data->FNum(0), MIN_ITERATOR_NUM))
  for(int i = 0; i < mesh_data->VNum(win); ++i)
  {
    sqi::meshdata::VertexIterator vi =  mesh_data->VBeginIter(win) + i;
    vtkSmartPointer<vtkIdList> idx_list = 
        vtkSmartPointer<vtkIdList>::New();
    double ref_point[3] = {vi->cP()[0], vi->cP()[1], vi->cP()[2]};
    data_set->KdTree()->FindPointsWithinRadius(radius, ref_point, idx_list);
    int n = idx_list->GetNumberOfIds();
    for(int j = 0; j < n; ++j)
    {
      sqi::meshdata::VertexIterator tmp_vi = mesh_data->VBeginIter(win) + idx_list->GetId(j);
      int idx = SurfaceDescriptor::MapCurvatureToBin(tmp_vi->Kh(), num_dim);
      mean_histogram[i][idx] += 1.0f;
      idx = SurfaceDescriptor::MapCurvatureToBin(tmp_vi->Kg(), num_dim);
      gauss_histogram[i][idx] += 1.0f;
      idx = SurfaceDescriptor::MapCurvatureToBin(tmp_vi->K1(), num_dim);
      k1_histogram[i][idx] += 1.0f;
      idx = SurfaceDescriptor::MapCurvatureToBin(tmp_vi->K2(), num_dim);
      k2_histogram[i][idx] += 1.0f;
    }
    for(int j = 0; j < mean_histogram[i].size(); ++j)
    {
      mean_histogram[i][j] /= static_cast<double>(n);
      gauss_histogram[i][j] /= static_cast<double>(n);
      k1_histogram[i][j] /= static_cast<double>(n);
      k2_histogram[i][j] /= static_cast<double>(n);
    }
  }
  string out_file;
  SurfaceDescriptor::GenerateExportFileName(data_set->ModelName(win),
                                            "_histogram_hmean.test", out_file);
  
  ExportTrainningData::ExportZernikerTestData(out_file, mean_histogram);
  SurfaceDescriptor::GenerateExportFileName(data_set->ModelName(win),
                                            "_histogram_hgauss.test", out_file);
  
  ExportTrainningData::ExportZernikerTestData(out_file, gauss_histogram);
  SurfaceDescriptor::GenerateExportFileName(data_set->ModelName(win),
                                            "_histogram_hk1.test", out_file);
  
  ExportTrainningData::ExportZernikerTestData(out_file, k1_histogram);
  SurfaceDescriptor::GenerateExportFileName(data_set->ModelName(win),
                                            "_histogram_hk2.test", out_file);
  
  ExportTrainningData::ExportZernikerTestData(out_file, k2_histogram);
  timer_->StopTimer();
  cout << "[#Info]curvature zerniker coefficient cost : " << timer_->GetElapsedTime() << endl;
}

void MeanCurvatureHistogram::Descriptor(meshdata::MeshDataPtr &mesh_data, 
                                        vtkSmartPointer<vtk_model_data> &data_set,
                                        WindowFlag win, int idx,
                                        std::vector<double> &histogram, int flag)
{
  assert(data_set != NULL && mesh_data.use_count() != 0);
  cout << "[#Info-alg]Mean curvature histogram." << endl;
  timer_->StartTimer();
  sqi::meshdata::MeshHandleB is_have_convex = 
      vcg::tri::Allocator<sqi::meshdata::Mesh>::GetPerMeshAttribute<bool>(mesh_data->M(win), 
                                                                          std::string("IsHaveConvex"));
  if(!is_have_convex())
  {
    CurvatureAlg<sqi::meshdata::Mesh>::MeanAndGaussian(mesh_data->M(win));
    is_have_convex() = true;
  }
  if(!mesh_data->IsHasPrinCur())
  {
    mesh_data->IsHasPrinCur() = true;
    CurvatureAlg<meshdata::Mesh>::PrincipalCurvatureAndDirection(mesh_data->M(win));
    CurvatureAlg<meshdata::Mesh>::PrincipalCurWithDirection(mesh_data->M(win));
  }
  data_set->BuildKdTree(win);
  double radius;
  double bound[6];
  data_set->PD(win)->GetBounds(bound);
  SurfaceDescriptor::InitializeParameters(mesh_data, bound, radius);
  cout << "[#Info]Radius is : " << radius << endl;
  sqi::meshdata::VertexIterator vi =  mesh_data->VBeginIter(win) + idx;
  vtkSmartPointer<vtkIdList> idx_list = 
      vtkSmartPointer<vtkIdList>::New();
  double ref_point[3] = {vi->cP()[0], vi->cP()[1], vi->cP()[2]};
  data_set->KdTree()->FindPointsWithinRadius(radius, ref_point, idx_list);
  int n = idx_list->GetNumberOfIds();
  int num_dim = 70;
  histogram.resize(num_dim);
  for(int j = 0; j < n; ++j)
  {
    sqi::meshdata::VertexIterator tmp_vi = mesh_data->VBeginIter(win) + idx_list->GetId(j);
    int tmp_idx;
    if(flag == 0)
      tmp_idx = SurfaceDescriptor::MapCurvatureToBin(tmp_vi->Kh(), num_dim);
    else if(flag == 1)
      tmp_idx = SurfaceDescriptor::MapCurvatureToBin(tmp_vi->Kg(), num_dim);
    else if(flag == 2)
      tmp_idx = SurfaceDescriptor::MapCurvatureToBin(tmp_vi->K1(), num_dim);
    else if(flag == 3)
      tmp_idx = SurfaceDescriptor::MapCurvatureToBin(tmp_vi->K2(), num_dim);
    histogram[tmp_idx] += 1.0f;
  }
  for(int j = 0; j < histogram.size(); ++j)
  {
    histogram[j] /= static_cast<double>(n);
    cout << histogram[j] << " ";
  }
  cout << endl;
}

}
}
