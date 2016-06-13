#include <Algorithms/CalHeightMap.h>
#include <CurvatureAlg.h>
#include <data_type.h>
#include <Topology/UpdateRingNeighbor.h>

#include <vtkPolyData.h>
#include <vtkMath.h>
#include <vtkIdList.h>
#include <vtkCell.h>

#include <vector>

namespace sqi {
namespace alg {
using namespace std;
using namespace lq;

CalHeightMap::CalHeightMap()
{
  timer_ = vtkSmartPointer<vtkTimerLog>::New();
}

CalHeightMap::~CalHeightMap(){
  
}

void CalHeightMap::CreateModifyTree(vtkSmartPointer<vtkPolyData> data)
{
  
  for(int i = 0; i < 7; ++i)
  {
    tree_[i] = vtkSmartPointer<vtkModifiedBSPTree>::New();
    poly_[i]= vtkSmartPointer<vtkPolyData>::New();
    poly_[i]->DeepCopy(data);
    tree_[i]->Initialize();
    tree_[i]->SetDataSet(poly_[i]);
    tree_[i]->BuildLocator();
  }
}

void CalHeightMap::CreateKDTree(vtkSmartPointer<vtkPolyData> data)
{
  for(int i = 0; i < 7; ++i)
  {
    kd_tree_[i] = vtkSmartPointer<vtkKdTreePointLocator>::New();
    poly_[i] = vtkSmartPointer<vtkPolyData>::New();
    poly_[i]->DeepCopy(data);
    kd_tree_[i]->Initialize();
    kd_tree_[i]->SetDataSet(poly_[i]);
    kd_tree_[i]->BuildLocator();
  }
  poly_[7] = vtkSmartPointer<vtkPolyData>::New();
  poly_[7]->DeepCopy(data);
}

void CalHeightMap::SetPolyData(vtkSmartPointer<vtkPolyData>& data)
{
  for(int i = 0; i < 8; ++i)
  {
    poly_[i] = vtkSmartPointer<vtkPolyData>::New();
    poly_[i]->DeepCopy(data);
  }
}

void CalHeightMap::CalMeshHeightMapBSPTree(meshdata::MeshDataPtr &mesh_data, 
                                           const std::string &out_name, 
                                           vtkSmartPointer<vtkModifiedBSPTree>& tree,
                                           double bound[6], lq::WindowFlag win,
std::vector<std::vector<double> >& height_map)
{
  if(mesh_data == NULL)
    return;
  Initialize(mesh_data, bound, win, height_map);
  SurfaceDescriptor::CalGridFlag(flag_, e_, radius_);
  clock_t start, end;
  start = clock();
  timer_->StartTimer();
  int quad = mesh_data->VNum(win) / 8;
#pragma omp parallel
  {
#pragma omp sections
    {
#pragma omp section
      {
        CalHeightMapSection(mesh_data, tree, 0, quad, win, height_map);
      }
#pragma omp section
      {
        CalHeightMapSection(mesh_data, tree_[0], quad + 1, 2 * quad, 
            win, height_map);
      }
#pragma omp section
      {
        CalHeightMapSection(mesh_data, tree_[1], 2 * quad + 1, 3 * quad,
            win, height_map);
      }
#pragma omp section
      {
        CalHeightMapSection(mesh_data, tree_[2], 3 * quad + 1, 4 * quad,
            win, height_map);
      }
#pragma omp section
      {
        CalHeightMapSection(mesh_data, tree_[3], 4 * quad + 1, 5 * quad,
            win, height_map);
      }
#pragma omp section
      {
        CalHeightMapSection(mesh_data, tree_[4], 5 * quad + 1, 6 * quad,
            win, height_map);
      }
#pragma omp section
      {
        CalHeightMapSection(mesh_data, tree_[5], 6 * quad + 1, 7 * quad,
            win, height_map);
      }
#pragma omp section
      {
        CalHeightMapSection(mesh_data, tree_[6], 7 * quad + 1, mesh_data->VNum(win) - 1,
            win, height_map);
      }
      
    }
  }
  end = clock();
  timer_->StopTimer();
  SurfaceDescriptor::WriteFile(height_map, out_name);
  cout << "[#Info]BSP tree cost time is : " << difftime(end, start)/CLOCKS_PER_SEC << endl;
  cout << "[#Info]BSP tree cost vtk timer's time is : " << timer_->GetElapsedTime() << endl;
}

void CalHeightMap::CalMeshHeightMapOBBTree(meshdata::MeshDataPtr &mesh_data, 
                                           const string &out_name, 
                                           vtkSmartPointer<vtkOBBTree> &tree,
                                           double bound[], WindowFlag win, 
                                           std::vector<std::vector<double> > &height_map)
{
  Initialize(mesh_data, bound, win, height_map);
  SurfaceDescriptor::CalGridFlag(flag_, e_, radius_);
  clock_t start, end;
  start = clock();
  timer_->StartTimer();
#pragma omp parallel for num_threads(thread_num(mesh_data->FNum(win), MIN_ITERATOR_NUM))
  for(int i = 0; i < mesh_data->VNum(win); ++i)
  {
    meshdata::VertexIterator vi = mesh_data->VBeginIter(win);
    vi += i;
    cout << i << endl;
    if(vi->IsB())
      continue;
    CalLineIntersectWithMesh intersect_processor; 
    for(int t = 0; t < 256; ++t)
    {
      if(!flag_[t])//if the grid cell is out of the radius, just set it -1
      {
        height_map[i][t] = -1.0f;
        continue;
      }
      int j = t / 16 - 7;
      int k = t % 16 - 7;
      vcg::Point3f center = vi->P();
      center += vi->PD1() * (static_cast<float>(j) - 0.5) * e_;
      center += vi->PD2() * (static_cast<float>(k) - 0.5) * e_;
      vcg::Point3f p0 = center + vi->N() * diagonal_len_;
      vcg::Point3f p1 = center - vi->N() * diagonal_len_;
      double min_dist = static_cast<double>(INT_MAX);
      bool flag;
      flag = intersect_processor.CalMinIntersect(tree, p0[0], p0[1], p0[2], 
          p1[0], p1[1], p1[2], center[0], center[1], center[2], min_dist);
      if(!flag)
        min_dist = 1.5 * rect_edge_;
      height_map[i][t] = min_dist;
    }
  }
  end = clock();
  timer_->StopTimer();
  SurfaceDescriptor::WriteFile(height_map, out_name);
  cout << "[#Info]OBB tree Cost time is : " << difftime(end, start)/CLOCKS_PER_SEC << endl;
  cout << "[#Info]OBB tree cost vtk timer's time is : " << timer_->GetElapsedTime() << endl;
}

void CalHeightMap::CalMeshHeightMapKDTree(meshdata::MeshDataPtr &mesh_data, 
                                          const string &out_name,
                                          vtkSmartPointer<vtkKdTreePointLocator> &tree, 
                                          double bound[], WindowFlag win, 
                                          std::vector<std::vector<double> > &height_map)
{
  Initialize(mesh_data, bound, win, height_map);
  SurfaceDescriptor::CalGridFlag(flag_, e_, radius_);
  timer_->StartTimer();
  double search_radius = 1.5 * rect_edge_;
  cout << "[#Info]The number of threads : " << thread_num(mesh_data->FNum(win), MIN_ITERATOR_NUM) << endl; 
#pragma omp parallel for num_threads(thread_num(mesh_data->FNum(win), MIN_ITERATOR_NUM))
  for(int i = 0; i < mesh_data->VNum(win); ++i)
  {
    cout << "id : " << i << endl;
    meshdata::VertexIterator vi = mesh_data->VBeginIter(win) + i;
    if(vi->IsB())
      continue;
    //prepare the triangle cell which need to calculate intersection in the next section
    vtkSmartPointer<vtkIdList> idx_list = 
        vtkSmartPointer<vtkIdList>::New();
    double ref_point[3] = {vi->cP()[0], vi->cP()[1], vi->cP()[2]};
    tree->FindPointsWithinRadius(search_radius, ref_point, idx_list);
    meshdata::VertexType* vert_ptr;
    set<int> tri_list;
    auto_ptr<alg::UpdateRigngNeighbor<meshdata::Mesh> > top_alg(new alg::UpdateRigngNeighbor<meshdata::Mesh>());
    //obtain triangle need to calculate intersection
    for(size_t j = 0; j < idx_list->GetNumberOfIds(); ++j)
    {
      int vertex_idx = idx_list->GetId(j);
      vert_ptr = &mesh_data->M(win).vert[vertex_idx];
      top_alg->VertexOneRingFaceNeighborVF(mesh_data->M(win), vert_ptr, tri_list);
    }
    vert_ptr = &mesh_data->M(win).vert[i];
    top_alg->VertexOneRingFaceNeighborVF(mesh_data->M(win), vert_ptr, tri_list);
    //calculate i'th vertex's height map 
    for(int t = 0; t < 256; ++t)
    {
      if(!flag_[t])//if the grid cell is out of the radius, just set it -1
      {
        height_map[i][t] = -1.0f;
        continue;
      }
      vcg::Point3f center, p0, p1;
      GenerateTransferVertex(t, vi, center, p0, p1);//calculate the cell center for the t^th cell
      vcg::Segment3f seg(p0, p1);
      //the fix value of maximum distance writen in paper.
      double min_dist = 1.5 * rect_edge_;
      meshdata::FaceIterator fi ;
      vcg::Point3f intersect(0, 0, 0);
      float tmp_dist = 0.0f;
      float u, v;
      //calculate the intersection point
      for(set<int>::iterator iter = tri_list.begin(); iter != tri_list.end(); ++iter)
      {
//        fi = mesh_data->FBeginIter(win) + (*iter);
//        //u, v, (1- u - v) are the weight relate to triagnle's three vertex
//        bool flag = IntersectionSegmentTriangle(seg, fi->cP(0), fi->cP(1), 
//                                                fi->cP(2), u, v);
//        if(!flag)
//          continue;
//        intersect = fi->cP(1) * u + fi->cP(2) * v + fi->cP(0) * (1 - u - v);
//        tmp_dist = (center - intersect).Norm();
//        if(min_dist > tmp_dist + 1e-8)
//          min_dist = tmp_dist;
        double v1[3] = {p0[0], p0[1], p0[2]};
        double v2[3] = {p1[0], p1[1], p1[2]};
        double inter[3], pcoord[3];
        int subid = 0;
        double t;
        bool flag = poly_intersect_->GetCell(*iter)->IntersectWithLine(v1, v2, 1e-6, t, inter, pcoord, subid);
        if(!flag)
          continue;
        vcg::Point3f intersect(inter[0], inter[1], inter[2]);
        tmp_dist = (center - intersect).Norm();
        if(min_dist > tmp_dist + 1e-8)
          min_dist = tmp_dist;
      }
      height_map[i][t] = min_dist;
    }
  }
  timer_->StopTimer();
  SurfaceDescriptor::WriteFile(height_map, out_name);
  cout << "[#Info]Kd tree cost vtk timer's time is : " << timer_->GetElapsedTime() << endl;
}


void CalHeightMap::CalMeshHeightMapKDTreeSplit(meshdata::MeshDataPtr& mesh_data, const std::string& out_name,
                                               vtkSmartPointer<vtkKdTreePointLocator>& tree,
                                               double bound[6], lq::WindowFlag win, std::vector<std::vector<double> >& height_map)
{
  if(mesh_data == NULL)
    return;
  Initialize(mesh_data, bound, win, height_map);
  
  SurfaceDescriptor::CalGridFlag(flag_, e_, radius_);
  timer_->StartTimer();
  int quad = mesh_data->VNum(win) / 8;
#pragma omp parallel
  {
#pragma omp sections
    {
#pragma omp section
      {
        CalHeightMapSection(mesh_data, 0, tree, 0, quad, win, height_map);
      }
#pragma omp section
      {
        CalHeightMapSection(mesh_data, 1, kd_tree_[0], quad + 1, 2 * quad, 
            win, height_map);
      }
#pragma omp section
      {
        CalHeightMapSection(mesh_data, 2, kd_tree_[1], 2 * quad + 1, 3 * quad,
            win, height_map);
      }
#pragma omp section
      {
        CalHeightMapSection(mesh_data, 3, kd_tree_[2], 3 * quad + 1, 4 * quad,
            win, height_map);
      }
#pragma omp section
      {
        CalHeightMapSection(mesh_data, 4, kd_tree_[3], 4 * quad + 1, 5 * quad,
            win, height_map);
      }
#pragma omp section
      {
        CalHeightMapSection(mesh_data, 5, kd_tree_[4], 5 * quad + 1, 6 * quad,
            win, height_map);
      }
#pragma omp section
      {
        CalHeightMapSection(mesh_data, 6, kd_tree_[5], 6 * quad + 1, 7 * quad,
            win, height_map);
      }
#pragma omp section
      {
        CalHeightMapSection(mesh_data, 7, kd_tree_[6], 7 * quad + 1, mesh_data->VNum(win) - 1,
            win, height_map);
      } 
    }
  }
  cout << "[#Info]Height multi : " << mesh_data->HeightRadiusMulti() << " Ratio : " << mesh_data->RadiusRatio() << endl;
  cout << "[#Info]Diagonal_len is : " << diagonal_len_ << " rectangle edge len : " << 
          rect_edge_   << "  height map radius : " << radius_ * mesh_data->HeightRadiusMulti() << endl; 
  timer_->StopTimer();
  SurfaceDescriptor::WriteFile(height_map, out_name);
  cout << "[#Info]KD tree cost vtk timer's time is : " << timer_->GetElapsedTime() << endl;
}

void CalHeightMap::CalHeightMapSection(meshdata::MeshDataPtr& mesh_data,
                                       vtkSmartPointer<vtkModifiedBSPTree>& tree,
                                       int start, int end, lq::WindowFlag win,
                                       std::vector<std::vector<double> >& height_map)
{
  CalLineIntersectWithMesh intersect_processor;
  for(int i = start; i <= end &&  i < mesh_data->VNum(win); ++i)
  {
    std::cout << i << std::endl;
    meshdata::VertexIterator vi = mesh_data->VBeginIter(win) + i;
    if(vi->IsB())
      continue;
    for(int t = 0; t < 256; ++t)
    {
      if(!flag_[t])
      {
        height_map[i][t] = -1.0f;
        continue;
      }
      vcg::Point3f center, p0, p1;
      GenerateTransferVertex(t, vi, center, p0, p1);
      double min_dist = static_cast<double>(INT_MAX);
      bool flag;
      flag = intersect_processor.CalMinIntersect(tree, p0[0], p0[1], p0[2], 
          p1[0], p1[1], p1[2], center[0], center[1], center[2], min_dist);
      if(!flag)
        min_dist = 1.5 * rect_edge_;
      height_map[i][t] = min_dist;
    }
  }
}

void CalHeightMap::CalHeightMapSection(meshdata::MeshDataPtr &mesh_data, int poly_id,
                                       vtkSmartPointer<vtkKdTreePointLocator> &tree, 
                                       int start, int end, WindowFlag win, 
                                       std::vector<std::vector<double> > &height_map)
{
  double search_radius = 1.5 * rect_edge_;
  for(int i = start; i <= end &&  i < mesh_data->VNum(win); ++i)
  {
//    cout << "id : " << i << endl; 
    meshdata::VertexIterator vi = mesh_data->VBeginIter(win) + i;
    if(vi->IsB())
      continue;
    vtkSmartPointer<vtkIdList> idx_list = 
        vtkSmartPointer<vtkIdList>::New();
    double ref_point[3] = {vi->cP()[0], vi->cP()[1], vi->cP()[2]};
    tree->FindPointsWithinRadius(search_radius, ref_point, idx_list);
    meshdata::VertexType* vert_ptr;
    set<int> tri_list;
    auto_ptr<alg::UpdateRigngNeighbor<meshdata::Mesh> > top_alg(new alg::UpdateRigngNeighbor<meshdata::Mesh>());
    //obtain triangle need to calculate intersection
    for(size_t j = 0; j < idx_list->GetNumberOfIds(); ++j)
    {
      int vertex_idx = idx_list->GetId(j);
      vert_ptr = &mesh_data->M(win).vert[vertex_idx];
      top_alg->VertexOneRingFaceNeighborVF(mesh_data->M(win), 
                                           vert_ptr, tri_list);
    }
    vert_ptr = &mesh_data->M(win).vert[i];
    top_alg->VertexOneRingFaceNeighborVF(mesh_data->M(win), 
                                         vert_ptr, tri_list);
    //calculate i'th vertex's height map 
    for(int t = 0; t < 256; ++t)
    {
      if(!flag_[t])//if the grid cell is out of the radius, just set it -1
      {
        height_map[i][t] = -1.0f;
        continue;
      }
      vcg::Point3f center, p0, p1;
      GenerateTransferVertex(t, vi, center, p0, p1);//calculate the cell center for the t^th cell
      //the fix value of maximum distance writen in paper.
      double min_dist = 1.5 * rect_edge_;
      float tmp_dist = 0.0f;
      //calculate the intersection point
      for(set<int>::iterator iter = tri_list.begin(); iter != tri_list.end(); ++iter)
      {
        double v1[3] = {p0[0], p0[1], p0[2]};
        double v2[3] = {p1[0], p1[1], p1[2]};
        double inter[3], pcoord[3];
        int subid = 0;
        double t;
        bool flag = poly_[poly_id]->GetCell(*iter)->IntersectWithLine(v1, v2, 1e-6, t, 
                                                                      inter, pcoord, subid);
        if(!flag)
          continue;
        vcg::Point3f intersect(inter[0], inter[1], inter[2]);
        tmp_dist = (center - intersect).Norm();
        if(min_dist > tmp_dist + 1e-8)
          min_dist = tmp_dist;
//        fi = mesh_data->FBeginIter(win) + (*iter);
//        //1
//        //u, v, (1- u - v) are the weight relate to triagnle's three vertex
//        bool flag = IntersectionSegmentTriangle(seg, fi->cP(0), fi->cP(1), 
//                                                fi->cP(2), u, v);
//        if(!flag)
//          continue;
//        intersect = fi->cP(1) * u + fi->cP(2) * v + fi->cP(0) * (1 - u - v);
//        tmp_dist = (center - intersect).Norm();
//        if(min_dist > tmp_dist + 1e-8)
//          min_dist = tmp_dist;
      }
      height_map[i][t] = min_dist;
    }
  }
}

void CalHeightMap::Initialize(meshdata::MeshDataPtr &mesh_data, double bound[],
                              lq::WindowFlag win, std::vector<std::vector<double> >& height_map)
{
  CurvatureAlg<meshdata::Mesh>::PrincipalCurWithDirection(mesh_data->M(win));
  double left_bottom[3] = {bound[0], bound[2], bound[4]};
  double right_top[3] = {bound[1], bound[3], bound[5]};
  height_map.clear();
  height_map.resize(mesh_data->VNum(win), vector<double>(256, 0));
  diagonal_len_ = sqrt(vtkMath::Distance2BetweenPoints(left_bottom, right_top));
  rect_edge_ = (2.5 * diagonal_len_) / 100.0f;
  if(!lq::is_zero(mesh_data->RadiusRatio(), 1e-6))
    rect_edge_ /= mesh_data->RadiusRatio();
  radius_ = rect_edge_ / 2.0f;
//  radius_ *= 2.0;
  e_ = rect_edge_ / 16;
}

bool CalHeightMap::IntersectionLineTriangle(const vcg::Line3f &line, 
                                            const vcg::Point3f &vert0, 
                                            const vcg::Point3f &vert1, 
                                            const vcg::Point3f &vert2, 
                                            float &t, float &u, float &v)
{
#define EPSIL 0.000001
  vcg::Point3f edge1, edge2, tvec, pvec, qvec;
  float det,inv_det;
  /* find vectors for two edges sharing vert0 */
  edge1 = vert1 - vert0;
  edge2 = vert2 - vert0;
  /* begin calculating determinant - also used to calculate U parameter */
  pvec =  line.Direction() ^ edge2;
  /* if determinant is near zero, line lies in plane of triangle */
  det =  edge1 *  pvec;
  /* calculate distance from vert0 to line origin */
  tvec = line.Origin() - vert0;
  inv_det = 1.0 / det;
  qvec = tvec ^ edge1;
  if (det > EPSIL)
  {
    u =  tvec * pvec ;
    if ( u < 0.0 ||  u > det)
      return 0;
    
    /* calculate V parameter and test bounds */
    v =  line.Direction() * qvec;
    if ( v < 0.0 ||  u +  v > det)
      return 0;
    
  }
  else if(det < -EPSIL)
  {
    /* calculate U parameter and test bounds */
    u =  tvec * pvec ;
    if ( u > 0.0 ||  u < det)
      return 0;
    
    /* calculate V parameter and test bounds */
    v =  line.Direction() * qvec  ;
    if ( v > 0.0 ||  u +  v < det)
      return 0;
  }
  else return 0;  /* line is parallell to the plane of the triangle */
  t = edge2 *  qvec  * inv_det;
  (u) *= inv_det;
  (v) *= inv_det;
  return 1;
}


bool CalHeightMap::IntersectionLineBox(const vcg::Box3f &box, 
                                       const vcg::Line3f &r, 
                                       vcg::Point3f &coord)
{
  const int NUMDIM = 3;
  const int RIGHT  = 0;
  const int LEFT	 = 1;
  const int MIDDLE = 2;
  
  int inside = 1;
  char quadrant[NUMDIM];
  int i;
  int whichPlane;
  vcg::Point4f maxT,candidatePlane;
  
  // Find candidate planes; this loop can be avoided if
  // rays cast all from the eye(assume perpsective view)
  for(i = 0; i < NUMDIM; ++i)
  {
    if(r.Origin()[i] < box.min[i])
    {
      quadrant[i] = LEFT;
      candidatePlane[i] = box.min[i];
      inside = 0;
    }
    else if (r.Origin()[i] > box.max[i])
    {
      quadrant[i] = RIGHT;
      candidatePlane[i] = box.max[i];
      inside = 0;
    }
    else
      quadrant[i] = MIDDLE;
  }
  
  // Ray origin inside bounding box
  if(inside)
  {
    coord = r.Origin();
    return true;
  }
  
  // Calculate T distances to candidate planes 
  for (i = 0; i < NUMDIM; ++i)
  {
    if (quadrant[i] != MIDDLE && r.Direction()[i] !=0.)
      maxT[i] = (candidatePlane[i]-r.Origin()[i]) / r.Direction()[i];
    else
      maxT[i] = -1.;
  }
  
  // Get largest of the maxT's for final choice of intersection
  whichPlane = 0;
  for (i = 1; i < NUMDIM; ++i)
    if (maxT[whichPlane] < maxT[i])
      whichPlane = i;
  
  // Check final candidate actually inside box 
  if (maxT[whichPlane] < 0.) return false;
  for (i = 0; i < NUMDIM; ++i)
  {
    if (whichPlane != i)
    {
      coord[i] = r.Origin()[i] + maxT[whichPlane] * r.Direction()[i];
      if (coord[i] < box.min[i] || coord[i] > box.max[i])
        return false;
    }
    else
      coord[i] = candidatePlane[i];
  }
  return true;			// ray hits box
}	

bool CalHeightMap::IntersectionSegmentBox(const vcg::Box3f &box, const vcg::Segment3f &s, vcg::Point3f &coord)
{
  //as first perform box-box intersection
  vcg::Box3f test;
  test.Add(s.P0());
  test.Add(s.P1());
  if (!test.Collide(box))
    return false;
  else
  {
    vcg::Line3f l;
    vcg::Point3f dir = s.P1() - s.P0();
    dir.Normalize();
    l.SetOrigin(s.P0());
    l.SetDirection(dir);
    if(IntersectionLineBox(box, l, coord))
      return (test.IsIn(coord));
    return false;
  }
}

int CalHeightMap::IntersectionSegmentBox(const vcg::Box3f &box, const vcg::Segment3f &s,
                                         vcg::Point3f &coord0, vcg::Point3f &coord1)
{
  int num = 0;
  vcg::Segment3f test = s;
  if (IntersectionSegmentBox(box, test, coord0))
  {
    num++;
    vcg::Point3f swap = test.P0();
    test.P0() = test.P1();
    test.P1() = swap;
    if(IntersectionSegmentBox(box, test, coord1))
      num++;
  }
  return num;
}	

bool CalHeightMap::IntersectionSegmentTriangle(const vcg::Segment3<float> &seg, 
                                               const vcg::Point3f &vert0, 
                                               const vcg::Point3f &vert1, 
                                               const vcg::Point3f &vert2,
                                               float &a, float &b)
{
  //control intersection of bounding boxes
  vcg::Box3<float> bb0, bb1;
  bb0.Add(seg.P0());
  bb0.Add(seg.P1());
  bb1.Add(vert0);
  bb1.Add(vert1);
  bb1.Add(vert2);
  vcg::Point3f inter;
  if (!bb0.Collide(bb1))
    return false;
  if (!IntersectionSegmentBox(bb1, seg, inter))
    return false;
  //first set both directions of ray
  vcg::Line3f line;
  vcg::Point3f dir;
  float length = seg.Length();
  dir=(seg.P1() - seg.P0());
  dir.Normalize();
  line.Set(seg.P0(), dir);
  float orig_dist;
  if(IntersectionLineTriangle(line, vert0, vert1, vert2, orig_dist, a, b))
    return (orig_dist >= 0 && orig_dist <= length);
  return false;
}

}

}
