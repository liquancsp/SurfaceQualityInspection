#ifndef GLOBALALIGN_H
#define GLOBALALIGN_H

#include <MeshData.h>
#include <menu_operate/AlignAlgorithm.h>

#include <vtkPolyData.h>

namespace sqi{
namespace alg{

class GlobalAlign : public AlignAlgorithm
{
public:
  
  GlobalAlign();
  virtual ~GlobalAlign();
  
  void ExecuteGlobalAlgin(meshdata::MeshDataPtr &mesh_data);
  void ExecuteGlobalAlginIcp(meshdata::MeshDataPtr &mesh_data);
protected:
  
  void SetIcpPolyData(vtkSmartPointer<vtkPolyData>& poly_data,
                      const vtkSmartPointer<vtkPoints>& p);
};

}
}

#endif // GLOBALALIGN_H
