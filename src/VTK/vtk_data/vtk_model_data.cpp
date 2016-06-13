/*****************************************************************************************
 *
 *COPYRIGHT NOTICE
 *Copyright (c) 2013, Zhe Jiang University
 *All rights reserved
 *
 *@file vtk_model_data.cpp
 *@author li quan
 *
 *version 2.0
 *data 2013-3
 *ingroup VTK/menu_operate
 *
 *@brief this file is to manager all the datas of models and the windows handel of QT
 *
 *
 ***********************************************************************************************/
#include "vtk_model_data.h"
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkCellData.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
namespace lq {

double vtk_model_data::render_view_port_[2][4] = 
{{0, 0, 0.5, 1.0},
 {0.5, 0.0, 1.0, 1.0}};

void vtk_model_data::Reset(lq::WindowFlag flag)
{
  mapper_[flag]->RemoveAllInputs();
  poly_data_[flag]->GetPoints()->Initialize();
  poly_data_[flag]->GetCellData()->Initialize();
  poly_data_[flag]->GetPolys()->Initialize();
  poly_data_[flag]->Initialize();
  is_build_kdtree_ = false;
  is_has_mapper_and_actor_[flag] = false;
  ResetColorBar(flag);
  vtkSmartPointer<vtkCamera> camera = 
      vtkSmartPointer<vtkCamera>::New();
  render_[flag]->SetActiveCamera(camera);
  qvtk_->update();
}

void vtk_model_data::ResetColorBar(lq::WindowFlag win)
{
  if(IsHasScalarBar(win))
  {
    scalar_bar_[win]->VisibilityOff();
    is_has_scalar_bar_[win] = false;
  }
  qvtk_->update();
}

void vtk_model_data::BuildKdTree(lq::WindowFlag win)
{
  if(kd_tree_ == NULL)
    kd_tree_ = vtkSmartPointer<vtkKdTreePointLocator>::New();
  if(!is_build_kdtree_)
  {
    kd_tree_->Initialize();
    kd_tree_->SetDataSet(poly_data_[win]);
    kd_tree_->SetMaxLevel(8);
    kd_tree_->BuildLocator();
    is_build_kdtree_ = true;
  }
}

void vtk_model_data::BuildOBBTree(lq::WindowFlag win)
{
  if(obb_tree_ == NULL)
    obb_tree_ = vtkSmartPointer<vtkOBBTree>::New();
  if(!is_build_obbtree_)
  {
    obb_tree_->Initialize();
    obb_tree_->SetDataSet(poly_data_[win]);
    obb_tree_->BuildLocator();
  }
}

void vtk_model_data::BuildBSPTree(WindowFlag win)
{
  if(bsp_tree_ == NULL)
    bsp_tree_ = vtkSmartPointer<vtkModifiedBSPTree>::New();
  if(!is_build_bsptree_)
  {
    bsp_tree_->Initialize();
    bsp_tree_->SetDataSet(poly_data_[win]);
    bsp_tree_->SetMaxLevel(6);
    bsp_tree_->BuildLocator();
  }
}

vtk_model_data::vtk_model_data() : 
  is_has_axis_(false), is_has_iter_style_(false), is_sphere_widget_create_(false),
  is_view_point_text_widget_create_(false),
  is_build_kdtree_(false), area_pick_type_(false),
  color_scalar_type_(Independent), color_scalar_value_(1.0f),
  algorithm_type_(lq::kNull),
  scalar_bar_widget_(vtkSmartPointer<vtkScalarBarWidget>::New()),
  view_point_text_actor_(vtkSmartPointer<vtkTextActor>::New()),
  view_point_text_widget_(vtkSmartPointer<vtkTextWidget>::New()),
  camera_(vtkSmartPointer<vtkCamera>::New()), axes_(vtkSmartPointer<vtkAxesActor>::New()),
  axes_widget_(vtkSmartPointer<vtkOrientationMarkerWidget>::New()),
  init_camera_(vtkSmartPointer<vtkCamera>::New())
  /*,
  vtk_prop_(new sqi::VtkData::VtkPropManage)*/
{
  for(size_t i = 0; i < 2; ++i)
  {
    render_[i] = vtkSmartPointer<vtkRenderer>::New();
    poly_data_[i] = vtkSmartPointer<vtkPolyData>::New();
    mapper_[i] = vtkSmartPointer<vtkPolyDataMapper>::New();
    actor_[i] = vtkSmartPointer<vtkActor>::New();
    model_name_text_actor_[i] = vtkSmartPointer<vtkTextActor>::New();
    scalar_bar_[i] = vtkSmartPointer<vtkScalarBarActor>::New();
    legend_lut_[i] = vtkSmartPointer<vtkLookupTable>::New();
    light_[i] = vtkSmartPointer<vtkLight>::New();
    is_has_pick_iter_style_[i] = false;
    is_has_mapper_and_actor_[i] = false;
    is_has_scalar_bar_[i] = false;
    is_first_set_color_scalar_[i] = false;
  }
}

vtk_model_data::~vtk_model_data()
{
}

}
