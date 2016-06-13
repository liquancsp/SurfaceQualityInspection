#ifndef CURVATUREDESCRIPTOR_H
#define CURVATUREDESCRIPTOR_H

#include <Algorithms/SurfaceDescriptor.h>

namespace sqi {
namespace alg{

class CurvatureDescriptor : public SurfaceDescriptor {
  
public :
  
  CurvatureDescriptor();
  ~CurvatureDescriptor();
  void Descriptor(meshdata::MeshDataPtr &mesh_data, 
                  vtkSmartPointer<lq::vtk_model_data> &data_set, 
                  lq::WindowFlag win, lq::InspectFlag flag);
  
};

}

}

#endif // CURVATUREDESCRIPTOR_H
