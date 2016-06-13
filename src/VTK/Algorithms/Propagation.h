#ifndef PROPAGATION_H
#define PROPAGATION_H

#include <MeshData.h>
#include <BaseAlgorithm.h>
#include <vtk_data/vtk_model_data.h>

#include <vtkTimerLog.h>
#include <vtkKdTreePointLocator.h>
#include <vtkSmartPointer.h>
#include <vtkPoints.h>


//#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/registration/icp.h>

namespace sqi{
namespace alg {

class Propagation {
  
public :
  Propagation();
  ~Propagation();
  

  void PropagateLocalExemplar(meshdata::MeshDataPtr& mesh_data, 
                              std::vector<std::vector<double> >& zerniker_coeff,
                              vtkSmartPointer<vtkKdTreePointLocator>& tree, 
                              lq::WindowFlag win, double tau, double similar_threshold,
                              double radius, int exemplar_cente);
  
private:
  
  double ZernikerDistance(const std::vector<double>& z1, 
                          const std::vector<double>& z2)
  {
    assert(z1.size() == z2.size());
    double res = 0.0f;
    for(int i = 0; i < z1.size(); ++i)
    {
      double tmp = z1[i] - z2[i];
      res += tmp * tmp;
    }
    res = std::sqrt(res);
  }
  
  void SetPolyData(meshdata::MeshDataPtr& mesh_data,
                   vtkSmartPointer<vtkIdList>& result, 
                   vtkSmartPointer<vtkPolyData> poly,
                   lq::WindowFlag win);
  
  void SetPCLData(meshdata::MeshDataPtr& mesh_data,
                  vtkSmartPointer<vtkIdList>& result,
                  pcl::PointCloud<pcl::PointXYZ>::Ptr& points_list,
                  lq::WindowFlag win);
  
  void SetReadable(meshdata::MeshDataPtr& mesh_data,
                   vtkSmartPointer<vtkIdList>& result, lq::WindowFlag win);
  
  bool CheckOverlap(vtkSmartPointer<vtkIdList>& result,
                    meshdata::MeshDataPtr& mesh_data,
                    lq::WindowFlag win);
  
  void SetSelectFlag(vtkSmartPointer<vtkIdList>& id_list,
                     meshdata::MeshDataPtr& mesh_data,
                     lq::WindowFlag win);
  
  
  
};

}
}
#endif // PROPATATION_H
