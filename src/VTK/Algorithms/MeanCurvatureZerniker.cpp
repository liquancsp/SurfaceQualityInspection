#include <MeanCurvatureZerniker.h>
#include <CurvatureAlg.h>
#include <ExportTrainningData.h>
#include <data_type.h>

#include <vtkIdList.h>

using namespace std;
using namespace sqi::io;
using namespace lq;
namespace sqi{
namespace alg{

MeanCurvatureZerniker::MeanCurvatureZerniker() : SurfaceDescriptor()
{
  
}

MeanCurvatureZerniker::~MeanCurvatureZerniker()
{
  
}

void MeanCurvatureZerniker::Descriptor(meshdata::MeshDataPtr &mesh_data, 
                                       vtkSmartPointer<lq::vtk_model_data> &data_set, 
                                       lq::WindowFlag win, lq::InspectFlag flag)
{
  assert(data_set != NULL && mesh_data.use_count() != 0);
  cout << "[#Info-alg]Mean curvature descriptor." << "  win : " << win <<endl;
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
  vector<vector<double> > mean_zerniker;
  mean_zerniker.resize(mesh_data->VNum(win));
  vector<vector<double> > zer_coeff_gauss(mesh_data->VNum(win), vector<double>(25, 0.0f));
  double bound[6];
  data_set->PD(win)->GetBounds(bound);
  std::vector<func_ptr> radial_poly;
  double radius;
  SurfaceDescriptor::InitializeParameters(mesh_data, radial_poly, bound, radius);
  cout << "[#Info-Parameters]Radius : " << radius << endl;
#pragma omp parallel for num_threads(thread_num(mesh_data->FNum(0), MIN_ITERATOR_NUM))
  for(int i = 0; i < mesh_data->VNum(win); ++i) 
  {
    sqi::meshdata::VertexIterator vi =  mesh_data->VBeginIter(win) + i;
    vtkSmartPointer<vtkIdList> idx_list = 
        vtkSmartPointer<vtkIdList>::New();
    double ref_point[3] = {vi->cP()[0], vi->cP()[1], vi->cP()[2]};
    data_set->KdTree()->FindPointsWithinRadius(radius, ref_point, idx_list);
    vector<double> neighbor_mean(idx_list->GetNumberOfIds(), 0.0f);
    for(int j = 0; j < idx_list->GetNumberOfIds(); ++j)
    {
      sqi::meshdata::VertexIterator tmp_vi = mesh_data->VBeginIter(win) + idx_list->GetId(j);
      neighbor_mean[j] = tmp_vi->Kh();
    }
    SurfaceDescriptor::CalZernikerCoeff(mesh_data, neighbor_mean, idx_list,
                                        radial_poly, i, win, mean_zerniker);
  }
  SurfaceDescriptor::CalGaussianWeightZernikeCoeff(mesh_data, data_set->KdTree(),
                                                   mean_zerniker, zer_coeff_gauss,
                                                   radius * mesh_data->HeightRadiusMulti(), win);
  std::string out_file = data_set->ModelName(win);
  int flag1 = out_file.find_last_of("/");
  int flag2 = out_file.find_last_of(".");
  out_file = out_file.substr(flag1 + 1, flag2 - flag1 - 1);
  out_file += "_zerniker_zmean.test";
  ExportTrainningData::ExportZernikerTestData(out_file, zer_coeff_gauss);
//  SurfaceDescriptor::WriteFile(mean_zerniker, "MeanZernikeCoeff.txt");
  timer_->StopTimer();
  cout << "[#Info]Mean curvature zerniker coefficient cost : " << timer_->GetElapsedTime() << endl;
}

}

}
