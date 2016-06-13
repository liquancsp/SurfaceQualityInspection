#ifndef GLDEPTHBUFFER_H
#define GLDEPTHBUFFER_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <MeshData.h>
#include <vtk_data/vtk_model_data.h>
#include <Algorithms/SurfaceDescriptor.h>

namespace sqi {
namespace gl {

class GLDepthBuffer : public alg::SurfaceDescriptor {
public :
  
  GLDepthBuffer(sqi::meshdata::MeshDataPtr& m,
                vtkSmartPointer<lq::vtk_model_data>& d);
  GLDepthBuffer();
  ~GLDepthBuffer();
  void LoadFile(const std::string& file_name);
  void SetEyesPosition(const double e[3]);
  void SetEyesPosition(double x, double y, double z);
  void SetFocalPosition(const double f[3]);
  void SetFocalPosition(double x, double y, double z);
  void SetProjectRange(const double range[6]);
  void SetProjectRange(double left, double right, double bottom, double top,
                       double near, double far);
  void SetWindowsSize(int w, int h);
  void SetWindowsSize(const int s[2]);
  
  vcg::Point3f& GetEyesPosition()                          {return eyes_;}
  vcg::Point3f& GetFocalPosition()                         {return focal_;}
  vcg::Point3f& GetUpView()                                {return up_view_;}
  float* GetProjectRange()                                 {return project_range_;}
  vcg::Point2i& GetWindowsSize()                           {return size_;}
  bool IsInRange(int id)                                   {assert(id >= 0 && id <= 255); return flag_[id];}
  void GenerageDepthValue();
  void Preprocessor();
  void UpdateParameters(int idx);
  meshdata::MeshDataPtr GetMeshData()                      {return mesh_data_;}
  
private:
  
  sqi::meshdata::MeshDataPtr mesh_data_;
  vtkSmartPointer<lq::vtk_model_data> data_set_;
  //left, right, bottom, top, near, far clipping planes
  float project_range_[6];
  vcg::Point3f eyes_;
  vcg::Point3f focal_;
  vcg::Point3f up_view_;
  //width height
  vcg::Point2i size_;
  std::vector<bool> flag_;
  float diagonal_len_;
  float rect_edge_;
  float radius_;
  float e_;
  
};

}

}

#endif
