#ifndef LOCALREGISTERINTEGRALMAP_H
#define LOCALREGISTERINTEGRALMAP_H
#include <vtkLandmarkTransform.h>
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtk_data/vtk_model_data.h>
#include <menu_operate/LocalAlign.h>
#include <BaseAlgorithm.h>
#include <MeshData.h>

#include <vcg/space/deprecated_point3.h>


namespace sqi{
namespace alg{

class LocalRegisterIntegralMap : public BaseAlgorithm , public LocalAlign
{
  
public:
  
  LocalRegisterIntegralMap();
  ~LocalRegisterIntegralMap();
  
  virtual void cal_tri_error(meshdata::MeshDataPtr &mesh_data, lq::InspectFlag flag);
  void SetDataSet(vtkSmartPointer<lq::vtk_model_data>& data)
  {
      this->data_set_ = data;
  }
private:
  
  void DoTransformation(vtkSmartPointer<vtkPolyData> &poly,
                        vtkSmartPointer<vtkLandmarkTransform> land_mark_transform);
  void GetPointFromPoly(const vtkSmartPointer<vtkPolyData> &poly, const size_t idx,
                        vcg::Point3d& p);
  void CalError(const vcg::Matrix44d& mat,
                const std::vector<int>& neigh_list,
                const size_t index,
                meshdata::MeshDataPtr &mesh_data);
  
  
  vtkSmartPointer<lq::vtk_model_data> data_set_;
};

}

}

#endif // LOCALREGISTERINTEGRALMAP_H
