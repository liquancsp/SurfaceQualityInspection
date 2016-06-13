#include <Propagation.h>

#include <vtkIdList.h>
#include <vtkPolyData.h>
#include <vtkIterativeClosestPointTransform.h>
#include <vtkLandmarkTransform.h>
#include <vtkVertexGlyphFilter.h>

#include <vector>

using namespace std;
namespace  sqi {
namespace alg{


Propagation::Propagation()
{
  
}

Propagation::~Propagation()
{
  
}

void Propagation::PropagateLocalExemplar(meshdata::MeshDataPtr &mesh_data,
                                         vector<vector<double> >& zerniker_coeff,
                                         vtkSmartPointer<vtkKdTreePointLocator>& tree,
                                         lq::WindowFlag win, double tau, 
                                         double radius, double similar_threshold,
                                         int exemplar_center)
{
  vtkSmartPointer<vtkIdList> result = 
      vtkSmartPointer<vtkIdList>::New();
  meshdata::VertexIterator vi = mesh_data->VBeginIter(win) + exemplar_center;
  double center_point[3] = {vi->cP()[0], vi->cP()[1], vi->cP()[2]};
  tree->FindPointsWithinRadius(radius, center_point, result);
  cout << "[#Debug]The number of neighbor : " << result->GetNumberOfIds() << endl; 
//  for(int i = 0; i < result->GetNumberOfIds(); ++i)
//    cout << result->GetId(i) << endl;
  //prepare local exemplar data
  pcl::PointCloud<pcl::PointXYZ>::Ptr exemplar_points(new pcl::PointCloud<pcl::PointXYZ>);
  SetPCLData(mesh_data, result, exemplar_points, win);
  SetReadable(mesh_data, result, win);
  //search exemplar center's feature neighbor
  vector<int> feature_neighbor;
  double min_zerniker = static_cast<double>(INT_MAX);
  for(int i = 0; i < mesh_data->VNum(win); ++i)
  {
    vi = mesh_data->VBeginIter(win) + i;
    if(vi->IsS() || i == exemplar_center)
      continue;
    double zerniker_dist = ZernikerDistance(zerniker_coeff[exemplar_center], 
                                            zerniker_coeff[i]);
    min_zerniker = min(min_zerniker, zerniker_dist);
    if(lq::is_less(zerniker_dist, tau))
      feature_neighbor.push_back(i);
  }
  for(int i = 0; i < feature_neighbor.size(); ++i)
    cout << feature_neighbor[i] << endl;
  cout << "min_zerniker : " << min_zerniker << endl;
  cout << "[#Debug]Feature space size : " << feature_neighbor.size() << endl;
  for(int i = 0; i < feature_neighbor.size(); ++i)
  {
    vi = mesh_data->VBeginIter(win) + feature_neighbor[i];
    if(vi->IsS())
      continue;
    vtkSmartPointer<vtkIdList> id_list =
        vtkSmartPointer<vtkIdList>::New();
    double center[3] = {vi->cP()[0], vi->cP()[1], vi->cP()[2]};
    tree->FindPointsWithinRadius(radius, center, id_list);
    if(CheckOverlap(id_list, mesh_data, win))
      continue;
    pcl::PointCloud<pcl::PointXYZ>::Ptr target_points(new pcl::PointCloud<pcl::PointXYZ>);
    SetPCLData(mesh_data, id_list, target_points, win);
    pcl::IterativeClosestPoint<pcl::PointXYZ, pcl::PointXYZ> icp;
    pcl::PointCloud<pcl::PointXYZ> Final;
    icp.setInputSource(exemplar_points);
    icp.setInputTarget(target_points);
    icp.setMaximumIterations(20);
    icp.align(Final);
    double mean_dist = icp.getFitnessScore();
    if(lq::is_less(mean_dist, similar_threshold))
      SetSelectFlag(id_list, mesh_data, win);
  }
  
}

void Propagation::SetPolyData(meshdata::MeshDataPtr& mesh_data,
                              vtkSmartPointer<vtkIdList> &result,
                              vtkSmartPointer<vtkPolyData> poly,
                              lq::WindowFlag win)
{
  
  vtkSmartPointer<vtkPoints> points = 
      vtkSmartPointer<vtkPoints>::New();
  meshdata::VertexIterator vi;
  cout << "[#Debug]Result size is : " << result->GetNumberOfIds() << endl;
  ofstream fout("point.txt");
  for(int i = 0; i < result->GetNumberOfIds(); ++i)
  {
    vtkIdType idx = result->GetId(i);
    vi = mesh_data->VBeginIter(win) + idx;
    if(vi->IsS())
      continue;
    //vi->SetS();
    points->InsertNextPoint(vi->P()[0], vi->P()[1], vi->P()[2]);
    fout << vi->P()[0] << " " << vi->P()[1] << " " << vi->P()[2] << endl;
  }
  fout.close();
  vtkSmartPointer<vtkPolyData> temp = 
      vtkSmartPointer<vtkPolyData>::New();
  temp->SetPoints(points);
  
  vtkSmartPointer<vtkVertexGlyphFilter> vertexFilter = 
      vtkSmartPointer<vtkVertexGlyphFilter>::New();
#if VTK_MAJOR_VERSION <= 5
  vertexFilter->SetInputConnection(temp->GetProducerPort());
#else
  vertexFilter->SetInputData(temp);
#endif
  vertexFilter->Update();
  poly->ShallowCopy(vertexFilter->GetOutput());
}

void Propagation::SetPCLData(meshdata::MeshDataPtr &mesh_data, 
                             vtkSmartPointer<vtkIdList> &result, 
                             pcl::PointCloud<pcl::PointXYZ>::Ptr &points_list, 
                             lq::WindowFlag win)
{
  meshdata::VertexIterator vi;
  points_list->points.resize(result->GetNumberOfIds());
  for(int i = 0; i < result->GetNumberOfIds(); ++i)
  {
    vtkIdType idx = result->GetId(i);
    vi = mesh_data->VBeginIter(win) + idx;
    points_list->points[i].x = vi->cP()[0];
    points_list->points[i].y = vi->cP()[1];
    points_list->points[i].z = vi->cP()[2];
  }
}

void Propagation::SetReadable(meshdata::MeshDataPtr &mesh_data, 
                              vtkSmartPointer<vtkIdList> &result, lq::WindowFlag win)
{
  meshdata::VertexIterator vi;
  for(int i = 0; i < result->GetNumberOfIds(); ++i)
  {
    vi = mesh_data->VBeginIter(win) + result->GetId(i);
    vi->SetV();
    //vi->SetS();
  }
}

bool Propagation::CheckOverlap(vtkSmartPointer<vtkIdList> &result,
                               meshdata::MeshDataPtr &mesh_data,
                               lq::WindowFlag win)
{
  meshdata::VertexIterator vi;
  for(int i = 0; i < result->GetNumberOfIds(); ++i)
  {
    vtkIdType idx = result->GetId(i);
    vi = mesh_data->VBeginIter(win) + idx;
    if(vi->IsS())
      return true;
  }
  
  return false;
}

void Propagation::SetSelectFlag(vtkSmartPointer<vtkIdList> &id_list, 
                                meshdata::MeshDataPtr &mesh_data,
                                lq::WindowFlag win)
{
  meshdata::VertexIterator vi;
  for(int i = 0; i < id_list->GetNumberOfIds(); ++i)
  {
    vtkIdType idx = id_list->GetId(i);
    vi = mesh_data->VBeginIter(win) + idx;
    vi->SetS();
  }
}

}

}
