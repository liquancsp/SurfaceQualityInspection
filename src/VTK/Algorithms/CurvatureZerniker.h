#ifndef CURVATUREZERNIKER_H
#define CURVATUREZERNIKER_H

#include <Algorithms/SurfaceDescriptor.h>

namespace sqi{
namespace alg{

class CurvatureZerniker : public SurfaceDescriptor{
  
public:
  CurvatureZerniker();
  ~CurvatureZerniker();
  void Descriptor(meshdata::MeshDataPtr &mesh_data, 
                  vtkSmartPointer<lq::vtk_model_data> &data_set, 
                  lq::WindowFlag win, lq::InspectFlag flag);
  
};

}
}

#endif // CURVATUREZERNIKER_H
