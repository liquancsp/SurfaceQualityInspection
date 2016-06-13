#ifndef PRINCIPALCURVATUREZERNIKER_H
#define PRINCIPALCURVATUREZERNIKER_H

#include <Algorithms/SurfaceDescriptor.h>

namespace sqi {
namespace alg {
class PrincipalCurvatureZerniker : public SurfaceDescriptor
{
  
public :
  PrincipalCurvatureZerniker();
  ~PrincipalCurvatureZerniker();
  void Descriptor(meshdata::MeshDataPtr &mesh_data,
                  vtkSmartPointer<lq::vtk_model_data>& data_set,
                  lq::WindowFlag win, lq::InspectFlag flag);
  
};

}

}

#endif // PRINCIPALCURVATUREZERNIKER_H
