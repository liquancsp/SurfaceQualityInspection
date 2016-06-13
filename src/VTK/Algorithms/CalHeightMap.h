#ifndef CALHEIGHTMAP_H
#define CALHEIGHTMAP_H

#include <MeshData.h>
#include <string>
#include <vtkSmartPointer.h>
#include <vtkOBBTree.h>
#include <vtkModifiedBSPTree.h>
#include <vtkKdTreePointLocator.h>
#include <vtkTimerLog.h>

#include <Algorithms/CalLineIntersectWithMesh.h>
#include <Algorithms/SurfaceDescriptor.h>

namespace sqi {
namespace alg {


class CalHeightMap : public SurfaceDescriptor{
  
public:
  CalHeightMap();
  ~CalHeightMap();
  void CalMeshHeightMapBSPTree(meshdata::MeshDataPtr& mesh_data, const std::string& out_name,
                               vtkSmartPointer<vtkModifiedBSPTree>& tree,
                               double bound[6], lq::WindowFlag win, std::vector<std::vector<double> >& height_map);
  
  void CalMeshHeightMapOBBTree(meshdata::MeshDataPtr& mesh_data, const std::string& out_name,
                               vtkSmartPointer<vtkOBBTree>& tree,
                               double bound[6], lq::WindowFlag win, std::vector<std::vector<double> >& height_map);
  
  void CalMeshHeightMapKDTree(meshdata::MeshDataPtr& mesh_data, const std::string& out_name,
                              vtkSmartPointer<vtkKdTreePointLocator>& tree,
                              double bound[6], lq::WindowFlag win, std::vector<std::vector<double> >& height_map);
  
  void CalMeshHeightMapKDTreeSplit(meshdata::MeshDataPtr& mesh_data, 
                                   const std::string& out_name,
                                   vtkSmartPointer<vtkKdTreePointLocator>& tree,
                                   double bound[6], lq::WindowFlag win, std::vector<std::vector<double> >& height_map);
  
  void CalHeightMapSection(meshdata::MeshDataPtr& mesh_data,
                           vtkSmartPointer<vtkModifiedBSPTree>& tree,
                           int start, int end, lq::WindowFlag win,
                           std::vector<std::vector<double> >& height_map);
  
  void CalHeightMapSection(meshdata::MeshDataPtr& mesh_data, int poly_id,
                           vtkSmartPointer<vtkKdTreePointLocator>& tree,
                           int start, int end, lq::WindowFlag win,
                           std::vector<std::vector<double> >& height_map);
  
  void CreateModifyTree(vtkSmartPointer<vtkPolyData> data);
  void CreateKDTree(vtkSmartPointer<vtkPolyData> data);
  void SetDataSet(vtkSmartPointer<vtkPolyData>& data)   {poly_intersect_ = data;}
  void SetPolyData(vtkSmartPointer<vtkPolyData>& data);
  vtkSmartPointer<vtkTimerLog>& GetTimer()   {return timer_;}
  
  float& GetRadius()                         {return radius_;}
  
private:
  
  void Initialize(meshdata::MeshDataPtr& mesh_data, double bound[6],
  lq::WindowFlag win, std::vector<std::vector<double> >& height_map);
  void GenerateTransferVertex(int i, const meshdata::VertexIterator vi, vcg::Point3f& center,
                              vcg::Point3f& p0, vcg::Point3f& p1)
  {
    int j = i / 16 - 7;
    int k = i % 16 - 7;
    center = vi->P();
    center += vi->PD1() * (static_cast<float>(j) - 0.5) * e_;
    center += vi->PD2() * (static_cast<float>(k) - 0.5) * e_;
    p0 = center + vi->N() * diagonal_len_;
    p1 = center - vi->N() * diagonal_len_;
  }
  
  bool IntersectionLineTriangle(const vcg::Line3f & line, const vcg::Point3f & vert0, 
                                const vcg::Point3f & vert1, const vcg::Point3f & vert2,
                                float & t ,float & u, float & v);
  
  
  bool IntersectionLineBox(const vcg::Box3f& box, const vcg::Line3f & r, vcg::Point3f & coord );
  
  bool IntersectionSegmentBox(const vcg::Box3f& box, const vcg::Segment3f & s, 
                              vcg::Point3f & coord );
  
  
  int IntersectionSegmentBox(const vcg::Box3f & box, const vcg::Segment3f & s,
                             vcg::Point3f & coord0, vcg::Point3f & coord1);
  
  bool IntersectionSegmentTriangle(const vcg::Segment3<float> & seg,
                                   const vcg::Point3f & vert0, 
                                   const vcg::Point3f & vert1,
                                   const vcg::Point3f & vert2,
                                   float & a , float & b);
  
  vtkSmartPointer<vtkModifiedBSPTree> tree_[7];
  vtkSmartPointer<vtkKdTreePointLocator> kd_tree_[7];
  vtkSmartPointer<vtkPolyData> poly_[8];
  vtkSmartPointer<vtkTimerLog> timer_;
  vtkSmartPointer<vtkPolyData> poly_intersect_;
  std::vector<bool> flag_;
  float diagonal_len_;
  float rect_edge_;
  float radius_;
  float e_;
};

}

}

#endif // CALHEIGHTMAP_H
