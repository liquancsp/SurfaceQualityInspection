#ifndef MEANCURVATUREHISTOGRAM_H
#define MEANCURVATUREHISTOGRAM_H
#include <Algorithms/SurfaceDescriptor.h>

namespace sqi {
namespace alg{

class MeanCurvatureHistogram : public SurfaceDescriptor {
public :
  
  MeanCurvatureHistogram();
  ~MeanCurvatureHistogram();
  void Descriptor(meshdata::MeshDataPtr &mesh_data,
                  vtkSmartPointer<lq::vtk_model_data>& data_set,
                  lq::WindowFlag win, lq::InspectFlag flag);
  
  void Descriptor(meshdata::MeshDataPtr &mesh_data,
                  vtkSmartPointer<lq::vtk_model_data>& data_set,
                  lq::WindowFlag win, int idx, std::vector<double>& histogram,
                  int flag = 0);
  
};

}
}


#endif // MEANCURVATUREHISTOGRAM_H
