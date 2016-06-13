#include <ZernikerPropagation.h>

#include <vtkIdList.h>
using namespace std;
using namespace lq;
using namespace sqi::io;
namespace sqi {
namespace alg {
vtkSmartPointer<vtk_model_data> ZernikerPropagation::data_set_ = NULL;

ZernikerPropagation::ZernikerPropagation() : BaseAlgorithm(),
  height_map_(new sqi::alg::CalHeightMap()),
  zerniker_coeff_(new ZernikeCoefficient()),
  propagate_(new Propagation())
{
}

ZernikerPropagation::~ZernikerPropagation()
{
  
}

void ZernikerPropagation::CalSingleProperty(meshdata::MeshDataPtr &mesh_data, 
                                            lq::InspectFlag flag, lq::WindowFlag win)
{
  assert(data_set_ != NULL);
  data_set_->BuildKdTree(win);
  double bounds[6];
  threshold_tau_ =  0.01;
  neighbor_radius_ = 0.55;
  double similar_threshold = 0.0022;
  exemplar_idx_ = 5930;
  data_set_->PD(win)->GetBounds(bounds);
  vector<vector<double> > height_map_vec;
  height_map_->SetDataSet(data_set_->PD(win));
  height_map_->CreateKDTree(data_set_->PD(win));
  string kdtree_out("kdtree_out_split.txt");
  height_map_->CalMeshHeightMapKDTreeSplit(mesh_data, kdtree_out, data_set_->KdTree(), 
                                           bounds, win, height_map_vec);
  vector<vector<double> > zerniker_coeff_vec;
  zerniker_coeff_->SetMeshData(mesh_data);
  zerniker_coeff_->CalZernikeCoeffWithGaussianWeight(mesh_data->VBeginIter(lq::kLeft),
                                                     data_set_->KdTree(), height_map_vec,
                                                     bounds, static_cast<double>(height_map_->GetRadius()),
                                                     zerniker_coeff_vec);
  cout << "[#Debug]Height radius is :" << height_map_->GetRadius() << endl;
  propagate_->PropagateLocalExemplar(mesh_data, zerniker_coeff_vec, 
                                     data_set_->KdTree(),  win,
                                     threshold_tau_, neighbor_radius_, 
                                     similar_threshold,
                                     exemplar_idx_);
  GenerateTriError(mesh_data, win);
}

void ZernikerPropagation::CreateTrainningData(meshdata::MeshDataPtr &mesh_data,
                                              const string& training_file,
                                              WindowFlag win, sqi::io::ClassifyType type)
{
  std::vector<std::vector<double> > zerniker_coeff_vec;
  GenerateZernikerCoeff(mesh_data, zerniker_coeff_vec, win);
  
  ExportTrainningData::ExportZerniker(training_file, type,
                                      zerniker_coeff_vec);
  
}

void ZernikerPropagation::CreateTrainningData(meshdata::MeshDataPtr &mesh_data, 
                                              const string& training_file,
                                              WindowFlag win, sqi::io::ClassifyType type,
                                              int exemplar_idx,  double radius)
{
  std::vector<std::vector<double> > zerniker_coeff_vec;
  GenerateZernikerCoeff(mesh_data, zerniker_coeff_vec, win);
  vtkSmartPointer<vtkIdList> result = 
      vtkSmartPointer<vtkIdList>::New();
  meshdata::VertexIterator vi = mesh_data->VBeginIter(win) + exemplar_idx;
  double center_point[3] = {vi->cP()[0], vi->cP()[1], vi->cP()[2]};
  data_set_->KdTree()->FindPointsWithinRadius(radius, center_point, result);
  vector<int> idx_list;
  idx_list.resize(result->GetNumberOfIds());
  for(int i = 0; i < result->GetNumberOfIds(); ++i)
    idx_list[i] = result->GetId(i);
  ExportTrainningData::ExportZerniker(training_file, type, 
                                      zerniker_coeff_vec, idx_list);
  
}

void ZernikerPropagation::CreateTrainningData(meshdata::MeshDataPtr &mesh_data,
                                              const string &training_file, 
                                              WindowFlag win, ClassifyType type, 
                                              std::vector<int> &idx_list)
{
  std::vector<std::vector<double> > zerniker_coeff_vec;
  GenerateZernikerCoeff(mesh_data, zerniker_coeff_vec, win);
  ExportTrainningData::ExportZerniker(training_file, type, 
                                      zerniker_coeff_vec, idx_list);
}

void ZernikerPropagation::CreateTestData(meshdata::MeshDataPtr &mesh_data, 
                                         const string &test_file, WindowFlag win)
{
  std::vector<std::vector<double> > zerniker_coeff_vec;
  GenerateZernikerCoeff(mesh_data, zerniker_coeff_vec, win);
  ExportTrainningData::ExportZernikerTestData(test_file, zerniker_coeff_vec);
}

void ZernikerPropagation::CreateTestData(meshdata::MeshDataPtr &mesh_data,
                                         const string &test_file, WindowFlag win,
                                         int exemplar_idx, double radius)
{
  std::vector<std::vector<double> > zerniker_coeff_vec;
  GenerateZernikerCoeff(mesh_data, zerniker_coeff_vec, win);
  vtkSmartPointer<vtkIdList> result = 
      vtkSmartPointer<vtkIdList>::New();
  meshdata::VertexIterator vi = mesh_data->VBeginIter(win) + exemplar_idx;
  double center_point[3] = {vi->cP()[0], vi->cP()[1], vi->cP()[2]};
  data_set_->KdTree()->FindPointsWithinRadius(radius, center_point, result);
  vector<int> idx_list;
//  idx_list.resize(result->GetNumberOfIds());
//  for(int i = 0; i < result->GetNumberOfIds(); ++i)
//  {
//    cout << "idx : " << result->GetId(i) << endl;
//    idx_list[i] = result->GetId(i);
//  }
  idx_list.push_back(exemplar_idx);
  ExportTrainningData::ExportZernikerTestData(test_file, zerniker_coeff_vec, idx_list);
}

void ZernikerPropagation::CreateTestData(meshdata::MeshDataPtr &mesh_data, 
                                         const string &test_file, 
                                         WindowFlag win, std::vector<int> &idx_list)
{
  std::vector<std::vector<double> > zerniker_coeff_vec;
  GenerateZernikerCoeff(mesh_data, zerniker_coeff_vec, win);
  ExportTrainningData::ExportZernikerTestData(test_file, zerniker_coeff_vec, idx_list);
}

void ZernikerPropagation::GenerateZernikerCoeff(meshdata::MeshDataPtr &mesh_data, 
                                                std::vector<std::vector<double> >&zerniker_coeff_vec,
                                                WindowFlag win)
{
  assert(data_set_ != NULL);
  data_set_->BuildKdTree(win);
  double bounds[6];
  data_set_->PD(win)->GetBounds(bounds);
  vector<vector<double> > height_map_vec;
  height_map_->SetDataSet(data_set_->PD(win));
  height_map_->CreateKDTree(data_set_->PD(win));
  height_map_->CalMeshHeightMapKDTreeSplit(mesh_data, "", data_set_->KdTree(), 
                                           bounds, win, height_map_vec);
  zerniker_coeff_->SetMeshData(mesh_data);
  zerniker_coeff_->CalZernikeCoeffWithGaussianWeight(mesh_data->VBeginIter(win),
                                                     data_set_->KdTree(), height_map_vec,
                                                     bounds, mesh_data->HeightRadiusMulti() * 
                                                     static_cast<double>(height_map_->GetRadius()),
                                                     zerniker_coeff_vec);
  cout << "height_map radius : " << height_map_->GetRadius() << endl;
}


void ZernikerPropagation::GenerateTriError(meshdata::MeshDataPtr &mesh_data, 
                                           WindowFlag win)
{
  assert(mesh_data.use_count() > 0);
  bool flag = false;
  for(meshdata::Mesh::FaceIterator fi = mesh_data->FBeginIter(win);
      fi != mesh_data->FEndIter(win); ++fi)
  {
    int cnt = 0;
    if(fi->V(0)->IsS() && !fi->V(0)->IsV())
      ++cnt;
    if(fi->V(1)->IsS() && !fi->V(1)->IsV())
      ++cnt;
    if(fi->V(2)->IsS() && !fi->V(2)->IsV())
      ++cnt;
    if(fi->V(0)->IsV())
      --cnt;
    if(fi->V(1)->IsV())
      --cnt;
    if(fi->V(2)->IsV())
      --cnt;
    if(cnt >= 2)
    {
      flag = true;
      fi->Q() = 1.0;
    }
    else if(cnt <= -2)
    {
      flag = true;
      fi->Q() = -1.0;
    }
    else 
      fi->Q() = 0.0;
  }
  if(flag)
    mesh_data->MaxE(win) = 1.0f;
  else 
    mesh_data->MaxE(win) = 0.0f;
  mesh_data->MinE(win) = -1.0f;
}


}

}
