#ifndef GAUSSCURVATUREZERNIKER_H
#define GAUSSCURVATUREZERNIKER_H

#include <Algorithms/SurfaceDescriptor.h>
#include <Facade/MenuOperator.h>

namespace sqi{
namespace alg{

class GaussCurvatureZerniker : public  SurfaceDescriptor{
  
public :
  
  GaussCurvatureZerniker();
  ~GaussCurvatureZerniker();
  void Descriptor(meshdata::MeshDataPtr &mesh_data,
                  vtkSmartPointer<lq::vtk_model_data>& data_set,
                  lq::WindowFlag win, lq::InspectFlag flag);
  
private:
  void CalLocalAlignIntegralDescriptor(meshdata::MeshDataPtr& mesh_data,
                                       vtkSmartPointer<lq::vtk_model_data>& data_set,
                                       lq::WindowFlag win, lq::InspectFlag flag);
};

}
}

#endif // GAUSSCURVATUREZERNIKER_H
