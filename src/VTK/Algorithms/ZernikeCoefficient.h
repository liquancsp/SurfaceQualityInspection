#ifndef ZERNIKECOEFFICIENT_H
#define ZERNIKECOEFFICIENT_H
#include <cmath>
#include <vector>
#include <MeshData.h>
#include <Algorithms/SurfaceDescriptor.h>

#include <vtkSmartPointer.h>
#include <vtkKdTreePointLocator.h>
#include <vtkTimerLog.h>

namespace sqi{
namespace alg{

class ZernikeCoefficient : public SurfaceDescriptor{
  
public: 
  ZernikeCoefficient();
  ~ZernikeCoefficient();
  void CalZernikeCoeff(const std::vector<std::vector<double> >& height_map,
                       const double bound[6], std::vector<std::vector<double> > &zernike_coeff);
  void CalZernikeCoeff(const std::vector<std::vector<double> >& height_map,
                       const double rect_edge, std::vector<std::vector<double> > &zernike_coeff);
  
  void SetMeshData(sqi::meshdata::MeshDataPtr m)
  {
    mesh_data_ = m;
  }
  
  void CalZernikeCoeffWithGaussianWeight(meshdata::VertexIterator vi_begin,
                                         vtkSmartPointer<vtkKdTreePointLocator>& kd_tree,
                                         const std::vector<std::vector<double> >& height_map,
                                         const double bound[6], double neigh_radius,
                                         std::vector<std::vector<double> >& zer_coeff_gauss);
private:
  sqi::meshdata::MeshDataPtr mesh_data_; 
  struct PolarCoord {
    double rho, theta;
    PolarCoord() : rho(0.0f), theta(0.0f){}
    PolarCoord(double r, double t) : rho(r), theta(t){}
  };
  
  typedef double(*func_ptr)(double);
  void PushRadialPolyFunc(std::vector<func_ptr>& radial_poly);
  template<typename T>
  void MultiplyVector(std::vector<T>& vec, T num)
  {
    for(int i = 0; i < vec.size(); ++i)
      vec[i] *= num;
  }
  
  template<typename T>
  void PlusVector(std::vector<T>& vec, T num)
  {
    for(int i = 0; i < vec.size(); ++i)
      vec[i] += num;
  }
  
//  template<typename T>
//  void DivVector(std::vector<T>& vec, T num)
//  {
//    if(num == 0)
//      return;
//    for(int i = 0; i < vec.size(); ++i)
//      vec[i] /= num;
//  }
  vtkSmartPointer<vtkTimerLog> timer_;
};

}
}

#endif // ZERNIKECOEFFICIENT_H
