#ifndef CURVATURESMOOTHDIFFZERNIKER_H
#define CURVATURESMOOTHDIFFZERNIKER_H
#include <Algorithms/SurfaceDescriptor.h>

#include <vtkKdTreePointLocator.h>
#include <vtkSmartPointer.h>

namespace sqi {
namespace alg{

class CurvatureSmoothDiffZerniker : public SurfaceDescriptor{
  
public :
  CurvatureSmoothDiffZerniker();
  ~CurvatureSmoothDiffZerniker();
  void Descriptor(meshdata::MeshDataPtr &mesh_data, 
                  vtkSmartPointer<lq::vtk_model_data> &data_set, 
                  lq::WindowFlag win, lq::InspectFlag flag);
protected:
  
  void CreateVtkPolyData(vtkSmartPointer<vtkPolyData>& poly_data,
                         const meshdata::Mesh& mesh);
  
  void CalCurvatureZerniker(meshdata::Mesh& mesh,  std::vector<func_ptr>& radial_poly,
                            vtkSmartPointer<vtkKdTreePointLocator>& kd_tree,
                            std::vector<std::vector<std::vector<double> > >& cur_zerniker,
                            int idx, bool is_diff, double radius);
};

}
}
#endif // CURVATURESMOOTHDIFFZERNIKER_H
