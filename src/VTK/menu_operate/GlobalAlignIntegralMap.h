#ifndef GLOBALALIGNINTEGRALMAP_H
#define GLOBALALIGNINTEGRALMAP_H
#include <BaseAlgorithm.h>
#include <menu_operate/GlobalAlign.h>

#include <vtkLandmarkTransform.h>
#include <vtkPoints.h>
#include <vtkSmartPointer.h>

namespace sqi {
namespace alg{

class GlobalAlignIntegralMap : public BaseAlgorithm, public GlobalAlign
{

public:
  GlobalAlignIntegralMap();
  ~GlobalAlignIntegralMap();
  void cal_tri_error(meshdata::MeshDataPtr &mesh_data, lq::InspectFlag flag);
protected:
  
//  void ExecuteGlobalAlgin(meshdata::MeshDataPtr &mesh_data);
  
//  void GenerateRigidTransMatrix4x4(const vtkSmartPointer<vtkPoints> &points1,
//                                   const vtkSmartPointer<vtkPoints> &points2,
//                                   vtkSmartPointer<vtkLandmarkTransform> &land_mark_transform);
//  void TransMatrix4x4FromVtkToVcg(const vtkSmartPointer<vtkMatrix4x4> &vtk_mat,
//                                  vcg::Matrix44d& vcg_mat);
};

}

}

#endif // GLOBALALIGNINTEGRALMAP_H
