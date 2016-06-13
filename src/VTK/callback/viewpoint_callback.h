/*******************************************************************************
 *COPYRIGHT NOTICE  
 *Copyright (c) 2012, Zhe Jiang University 
 *All rights reserved.
 *
 *@file interactor_style.h
 *@author li quan
 *@version 1.0
 *@date 2013-4-12
 *@ingroup VTK/callback
 *@brief the function of this class is to re-calculate the reflection map result when
 *user change view point sphere widget.
 *
 *@history
 ********************************************************************************/

#ifndef VIEWPOINT_CALLBACK_H
#define VIEWPOINT_CALLBACK_H

#include <vtk_data/vtk_model_data.h>
#include <menu_operate/edit_menu_operate.h>
#include <MeshData.h>

#include <vtkSmartPointer.h>
#include <vtkCommand.h>

namespace lq
{

class viewpoint_callback : public vtkCommand
{
  
public:
  
  viewpoint_callback();
  
  static viewpoint_callback *New()
  {
    return new viewpoint_callback;
  }
  void set_data_set(const vtkSmartPointer<vtk_model_data> &other_data_set);
  void set_mesh_data(const sqi::meshdata::MeshDataPtr& other)
  {
    this->mesh_data_ = other;
  }

  virtual void Execute(vtkObject *obj, unsigned long, void *cell_data);
  
private:
  
  vtkSmartPointer<vtk_model_data> data_set_;
  sqi::meshdata::MeshDataPtr mesh_data_;
  //edit_menu_operate edit_operate_;
  
};

};
#endif
