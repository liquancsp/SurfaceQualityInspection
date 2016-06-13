/*******************************************************************************
 *COPYRIGHT NOTICE  
 *Copyright (c) 2012, Zhe Jiang University 
 *All rights reserved.
 *
 *@file interactor_style.h
 *@author li quan
 *@version 1.0
 *@date 2013-5-14
 *@ingroup VTK/callback
 *@brief the function of this class is to calculate the reflection map result when
 *user rotate and transform the model in real time
 *
 *@history
 ********************************************************************************/
#ifndef INTERACTOR_STYLE_H
#define INTERACTOR_STYLE_H
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkObjectFactory.h>
#include <vtkCamera.h>
#include <vtkSmartPointer.h>

#include <vtkMatrix4x4.h>
#include <data_type.h>
#include <vtk_data/vtk_model_data.h>
#include <Builder/InspectActionDirector.h>
#include <MeshData.h>

namespace lq 
{

class interactor_style : public vtkInteractorStyleTrackballCamera
{
public:

 
  static interactor_style* New()
  {
    return new interactor_style;
  }
  
  static interactor_style* New(sqi::meshdata::MeshDataPtr mesh,
                               vtkSmartPointer<lq::vtk_model_data>& data)
  {
    return new interactor_style(mesh, data);
  }
  
  vtkTypeMacro(interactor_style, vtkInteractorStyleTrackballCamera);  
  interactor_style() :
    inspect_director_ptr_(new sqi::common::InspectActionDirector()),
    rwi_(NULL)
  {}
  
  interactor_style(sqi::meshdata::MeshDataPtr mesh,
                   vtkSmartPointer<lq::vtk_model_data>& data) :
    inspect_director_ptr_(new sqi::common::InspectActionDirector(mesh, data)),
    rwi_(NULL)
  {}
  
  void set_data_set(const vtkSmartPointer<vtk_model_data> &other_data_set);
  void set_mesh_data(const std::tr1::shared_ptr<sqi::meshdata::MeshData<sqi::meshdata::Mesh> > &other)
  {
    this->mesh_data_ = other;
    inspect_director_ptr_->SetMeshData(other);
  }
  
  
  void set_points();
  void set_face();
  void set_wireframe();
  void set_surface_with_edge();
  int get_representation();
  
protected:
  
  virtual void OnMouseMove();
  virtual void OnMouseWheelForward();
  virtual void OnMouseWheelBackward();
  virtual void OnRightButtonDown();
  virtual void OnLeftButtonUp();
  virtual void OnChar();
  virtual void OnKeyDown();
  virtual void OnKeyPress();
  
private:
  
  
  vtkSmartPointer<vtk_model_data> data_set_;
  vtkSmartPointer<vtkRenderWindowInteractor> rwi_;
  std::tr1::shared_ptr<sqi::meshdata::MeshData<sqi::meshdata::Mesh> > mesh_data_;
  std::tr1::shared_ptr<sqi::common::InspectActionDirector> inspect_director_ptr_;
  bool is_position_equal(const double p1[3], const double p2[3]);
  void cal_error();
  double pos[3];
  double pre_pos[3];
  
};

//vtkStandardNewMacro(interactor_style);

}

#endif // INTERACTOR_STYLE_H
