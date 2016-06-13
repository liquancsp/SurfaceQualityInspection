#ifndef MEANCURVATUREZERNIKER_H
#define MEANCURVATUREZERNIKER_H
#include <Algorithms/SurfaceDescriptor.h>


namespace sqi{
namespace alg{

class MeanCurvatureZerniker : public SurfaceDescriptor{
  
public:
  MeanCurvatureZerniker();
  ~MeanCurvatureZerniker();
  void Descriptor(meshdata::MeshDataPtr &mesh_data,
                  vtkSmartPointer<lq::vtk_model_data>& data_set,
                  lq::WindowFlag win, lq::InspectFlag flag);
  
};

}

}
#endif // MEANCURVATUREZERNIKER_H
