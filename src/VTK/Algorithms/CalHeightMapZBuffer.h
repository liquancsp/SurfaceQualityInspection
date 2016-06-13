#ifndef CALHEIGHTMAPZBUFFER_H
#define CALHEIGHTMAPZBUFFER_H

#include <Algorithms/SurfaceDescriptor.h>
#include <MeshData.h>

#include <vtkPolyData.h>
#include <vtkSmartPointer.h>

#include <vector>

namespace sqi{
namespace alg{

class CalHeightMapZBuffer : public SurfaceDescriptor{
  
public :
  
  CalHeightMapZBuffer();
  ~CalHeightMapZBuffer();
  void HeightMapZBuffer(meshdata::MeshDataPtr& mesh_data, std::string &out_name,
                        vtkSmartPointer<vtkPolyData>& poly,
                        double bound[6], lq::WindowFlag win, 
  std::vector<std::vector<double> >& height_map);
private:
 
};

}

}

#endif // CALHEIGHTMAPZBUFFER_H
