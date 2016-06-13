#include <CurvatureDescriptor.h>
#include <CurvatureAlg.h>
#include <ExportTrainningData.h>
#include <data_type.h>

#include <vtkIdList.h>

using namespace std;
using namespace sqi::io;
using namespace lq;

namespace sqi {
namespace alg{

CurvatureDescriptor::CurvatureDescriptor() : SurfaceDescriptor()
{
  
}

CurvatureDescriptor::~CurvatureDescriptor()
{
  
}

void CurvatureDescriptor::Descriptor(meshdata::MeshDataPtr &mesh_data,
                                     vtkSmartPointer<vtk_model_data> &data_set, 
                                     WindowFlag win, InspectFlag flag)
{
  cout << "[#Info-alg]Curvature descriptor." << endl;
  timer_->StartTimer();
  double bound[6];
  double radius;
  SurfaceDescriptor::InitializeParameters(mesh_data, data_set, win, bound);
  vcg::Point3f left_bottom(bound[0], bound[2], bound[4]);
  vcg::Point3f right_top(bound[1], bound[3], bound[5]);
  double diagonal_len = vcg::Distance(left_bottom, right_top);
  double rect_edge = (2.5 * diagonal_len) / 100.0f;
  if(!lq::is_zero(mesh_data->RadiusRatio(), 1e-6))
    rect_edge /= mesh_data->RadiusRatio();
  radius = rect_edge / 2.0f;
  cout << "[#Info]Neighbor radius is : " << radius << endl;
  vector<double> mean_curvature(mesh_data->VNum(win));
  vector<double> gauss_curvature(mesh_data->VNum(win));
  vector<double> k1_curvature(mesh_data->VNum(win));
  vector<double> k2_curvature(mesh_data->VNum(win));
#pragma omp parallel for num_threads(thread_num(mesh_data->FNum(0), MIN_ITERATOR_NUM)) 
  for(int i = 0; i < mesh_data->VNum(win); ++i)
  {
    sqi::meshdata::VertexIterator vi = mesh_data->VBeginIter(win) + i;
    vtkSmartPointer<vtkIdList> idx_list = 
        vtkSmartPointer<vtkIdList>::New();
    double ref_point[3] = {vi->cP()[0], vi->cP()[1], vi->cP()[2]};
    data_set->KdTree()->FindPointsWithinRadius(radius, ref_point, idx_list);
    double kh = 0.0f,  kg = 0.0f,  k1 = 0.0f, k2 = 0.0f;
    for(int j = 0; j < idx_list->GetNumberOfIds(); ++j)
    {
      sqi::meshdata::VertexIterator tmp_vi = mesh_data->VBeginIter(win) + idx_list->GetId(j);
      kh += tmp_vi->Kh();
      kg += tmp_vi->Kg();
      k1 += tmp_vi->K1();
      k2 += tmp_vi->K2();
    }
    assert(idx_list->GetNumberOfIds() != 0);
    kh /= static_cast<double>(idx_list->GetNumberOfIds());
    kg /= static_cast<double>(idx_list->GetNumberOfIds());
    k1 /= static_cast<double>(idx_list->GetNumberOfIds());
    k2 /= static_cast<double>(idx_list->GetNumberOfIds());
    mean_curvature[i] = kh;
    gauss_curvature[i] = kg;
    k1_curvature[i] = k1;
    k2_curvature[i] = k2;
  }
  string out_file;
  SurfaceDescriptor::GenerateExportFileName(data_set->ModelName(win),
                                            "_mean.test", out_file);
  ExportTrainningData::ExportCurvatureTestData(out_file, mean_curvature);
  SurfaceDescriptor::GenerateExportFileName(data_set->ModelName(win),
                                            "_gauss.test", out_file);
  ExportTrainningData::ExportCurvatureTestData(out_file, gauss_curvature);
  SurfaceDescriptor::GenerateExportFileName(data_set->ModelName(win),
                                            "_k1.test", out_file);
  ExportTrainningData::ExportCurvatureTestData(out_file, k1_curvature);
  SurfaceDescriptor::GenerateExportFileName(data_set->ModelName(win),
                                            "_k2.test", out_file);
  ExportTrainningData::ExportCurvatureTestData(out_file, k2_curvature);
  timer_->StopTimer();
  cout << "[#Info]Curvature descriptor cost : " << timer_->GetElapsedTime() << endl;
  
}

}
}
