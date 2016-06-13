#include <GLDepthBuffer.h>
#include <CurvatureAlg.h>
#include <Facade/MenuOperator.h>

#include <vtkMath.h>

#include <cstdlib>

namespace sqi {
namespace gl{
using namespace std;

GLDepthBuffer::GLDepthBuffer(sqi::meshdata::MeshDataPtr& m,
                             vtkSmartPointer<lq::vtk_model_data>& d) :
  mesh_data_(m), data_set_(d)
{
  GLDepthBuffer::Preprocessor();
}

GLDepthBuffer::GLDepthBuffer() : mesh_data_(new sqi::meshdata::MeshData<sqi::meshdata::Mesh>),
  data_set_(vtkSmartPointer<lq::vtk_model_data>::New())
{
  
}

GLDepthBuffer::~GLDepthBuffer()
{
  
}

void GLDepthBuffer::LoadFile(const string &file_name)
{
  std::auto_ptr<sqi::common::MenuOperator> opt(new sqi::common::MenuOperator(mesh_data_,
                                                                             data_set_));
  opt->LoadModelFile(file_name, lq::kLeft);
  opt->SetRenderData(lq::kLeft);
  GLDepthBuffer::Preprocessor();
}

void GLDepthBuffer::SetEyesPosition(double x, double y, double z)
{
  eyes_[0] = x;
  eyes_[1] = y;
  eyes_[2] = z;
}

void GLDepthBuffer::SetEyesPosition(const double e[3])
{
  SetEyesPosition(e[0], e[1], e[2]);
}

void GLDepthBuffer::SetFocalPosition(double x, double y, double z)
{
  focal_[0] = x;
  focal_[1] = y;
  focal_[2] = z;
}

void GLDepthBuffer::SetFocalPosition(const double f[3])
{
  SetFocalPosition(f[0], f[1], f[2]);
}

void GLDepthBuffer::SetProjectRange(double left, double right, double bottom, double top, double near, double far)
{
  project_range_[0] = left;
  project_range_[1] = right;
  project_range_[2] = bottom;
  project_range_[3] = top;
  project_range_[4] = near;
  project_range_[5] = far;
}

void GLDepthBuffer::SetProjectRange(const double range[6])
{
  SetProjectRange(range[0], range[1], range[2],
      range[3], range[4], range[5]);
}

void GLDepthBuffer::SetWindowsSize(int w, int h)
{
  size_[0] = w;
  size_[1] = h;
}

void GLDepthBuffer::SetWindowsSize(const int s[2])
{
  SetWindowsSize(s[0], s[1]);
}

void GLDepthBuffer::GenerageDepthValue()
{
  //glutDisplayFunc(GLDepthBuffer::RenderScene);
}

void GLDepthBuffer::Preprocessor()
{
  alg::CurvatureAlg<meshdata::Mesh>::PrincipalCurWithDirection(mesh_data_->M(lq::kLeft));
  double bound[6];
  data_set_->PD(lq::kLeft)->GetBounds(bound);
  double left_bottom[3] = {bound[0], bound[2], bound[4]};
  double right_top[3] = {bound[1], bound[3], bound[5]};
  diagonal_len_ = sqrt(vtkMath::Distance2BetweenPoints(left_bottom, right_top));
  rect_edge_ = (2.5 * diagonal_len_) / 100.0f;
  radius_ = rect_edge_ / 2.0f;
  e_ = rect_edge_ / 16;
  project_range_[0] = project_range_[2] = -radius_;
  project_range_[1] = project_range_[3] = radius_;
  project_range_[4] = diagonal_len_ , project_range_[5] = 2 * diagonal_len_;
  cout << " project : ";
  for(int i = 0; i < 6; ++i)
    cout << project_range_[i] << " ";
  cout << endl;
  size_[0] = size_[1] = 16;
  alg::SurfaceDescriptor::CalGridFlag(flag_, e_, radius_);
}

void GLDepthBuffer::UpdateParameters(int idx)
{
  meshdata::VertexIterator vi = mesh_data_->VBeginIter(lq::kLeft) + idx;
  vi->N().Norm();
  eyes_ = vi->N() * diagonal_len_ + vi->P();
  focal_ = vi->P();
  up_view_ = vi->PD1();
  vcg::Point3f dir1 = focal_ - eyes_;
  vcg::Point3f dir2 = eyes_ - vcg::Point3f(0, 0, 0);
  double dot1 =  dir1.dot(up_view_);
  double dot2 = dir2.dot(up_view_);
  cout << "dirt : " << dot1 << endl;
  cout << "dirt2 : " << dot2 << endl;
  cout << "eyes : " << eyes_[0] << " " << eyes_[1] << " " << eyes_[2] << endl;
  cout << "focal : " << focal_[0] << " " << focal_[1] << " " << focal_[2] << endl;
  cout << "up : " << up_view_[0] << " " << up_view_[1] << " " << up_view_[2] << endl;
}

}

}
