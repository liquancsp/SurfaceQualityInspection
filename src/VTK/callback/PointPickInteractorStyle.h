#ifndef POINTPICKINTERACTORSTYLE_H
#define POINTPICKINTERACTORSTYLE_H

#include <vtk_data/vtk_model_data.h>
#include <MeshData.h>

#include <vtkInteractorStyleRubberBandPick.h>
#include <callback/interactor_style.h>
#include <vtkDataSetMapper.h>

#include <Builder/InspectActionDirector.h>

#include <QMenu>

namespace sqi{
namespace vtk{

class PointPickInteractorStyle : public vtkInteractorStyleRubberBandPick{
public :
  PointPickInteractorStyle();
  ~PointPickInteractorStyle();
  PointPickInteractorStyle(sqi::meshdata::MeshDataPtr mesh,
                           vtkSmartPointer<lq::vtk_model_data>& data);
  
  static PointPickInteractorStyle* New()
  {
    return new PointPickInteractorStyle;
  }
  
  static PointPickInteractorStyle* New(sqi::meshdata::MeshDataPtr mesh,
                                       vtkSmartPointer<lq::vtk_model_data>& data)
  {
    return new PointPickInteractorStyle(mesh, data);
  }
  
  vtkTypeMacro(PointPickInteractorStyle, vtkInteractorStyleRubberBandPick);
  
  void SetDataSet(const vtkSmartPointer<lq::vtk_model_data>& data);
  void SetMeshData(const sqi::meshdata::MeshDataPtr& mesh);
  void SetWinFlag(const lq::WindowFlag& flag);
  int GetRepresentation();
  void SetPointRepresentation();
  void SetFaceRepresentation();
  void SetWireframeRepresentation();
  void SetSurfaceWithEdgeRepresentation();
protected:
  
  virtual void OnKeyRelease();
  virtual void OnLeftButtonUp();
  virtual void OnRightButtonDown();
  virtual void OnMouseMove();
  virtual void OnMouseWheelForward();
  virtual void OnMouseWheelBackward();
  
  void SetLightParameter(const lq::WindowFlag flag,
                         vtkSmartPointer<vtkActor> actor);

  
private :
  
  void UpdateClassifyIndex(std::vector<int>& list, int label);
  void RecalError();
  vtkSmartPointer<lq::vtk_model_data> data_set_;
  sqi::meshdata::MeshDataPtr mesh_data_;
  lq::WindowFlag win_flag_;
  std::tr1::shared_ptr<sqi::common::InspectActionDirector> inspect_director_ptr_;
  std::vector<vtkSmartPointer<vtkDataSetMapper> > selected_mapper_list_;
  std::vector<vtkSmartPointer<vtkActor> > selected_actor_list_;
  std::vector<std::vector<int> > index_list_;
  
  vtkSmartPointer<vtkRenderWindowInteractor> rwi_;
  
};

}

}


#endif // POINTPICKINTERACTORSTYLE_H
