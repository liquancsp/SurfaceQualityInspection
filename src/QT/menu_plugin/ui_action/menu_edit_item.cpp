/*************************************************************************************
 *
 *COPYRIGHT NOTICE
 *Copyright (c) 2013, Zhe Jiang University
 *All rights reserved.
 *
 *@file menu_file_item.h
 *@author li quan
 *
 *version 2.0
 *data 2013-3
 *ingroup qt_vtk
 *
 *@brief this file is to support the function of menubar->Edit item. It include &Inspect,
 *&Load Color, &Clear Color
 *
 *************************************************************************************/
#include "menu_edit_item.h"
#include "../../configure/configure.h"


#include <QtGui>

namespace lq
{
using namespace sqi::meshdata;
using namespace std;
using namespace sqi::WidgetAction;

menu_edit_item::menu_edit_item(QWidget* parent) : QMenu(parent)
{
 
  //config_dialog_handle_.reset(new ConfigDialog());
  edit_menu_handle_.reset(new Ui::ui_menu_edit_item());
  edit_item_operate_.reset(new EditMenuOperate());
  view_point_operate_.reset(new view_point_operate());
  view_point_input_widget_handel_.reset(new view_point_input_widget());
  //color_scalar_input_widget_handel_.reset(new color_scalar_input_widget());
  this->edit_menu_handle_->setup_ui(this);
  
  create_action();
}

menu_edit_item::~menu_edit_item()
{
}

void menu_edit_item::set_data_set(const vtkSmartPointer<vtk_model_data> &other_data_set)
{
  this->data_set_ = other_data_set;
  edit_item_operate_->SetDataSet(data_set_);
  view_point_operate_->set_data_set(data_set_);
}

/********************************************************************************************/
//begin slots



void menu_edit_item::slot_load_color_action()
{
  edit_item_operate_->SetColorOnPoly();
//  edit_menu_handle_->load_color_action->setEnabled(false);
//  edit_menu_handle_->clear_color_action->setEnabled(true);
  emit signal_load_color();
}

void menu_edit_item::slot_clear_color_action()
{
  edit_item_operate_->ClearPolyColor();
  data_set_->QTW()->update();
//  edit_menu_handle_->load_color_action->setEnabled(true);
//  edit_menu_handle_->clear_color_action->setEnabled(false);
}

void menu_edit_item::slot_get_view_point(double x, double y, double z)
{
  point view_point(x, y, z);
  data_set_->ViewPoint() = view_point;
  view_point_input_widget_handel_->hide();
}

void menu_edit_item::slot_display_color_scalar_widget()
{
  color_scalar_input_widget_handel_->exec();
  
}

void menu_edit_item::slot_get_color_scalar(size_t flag, double max_scalar, 
                                           double min_scalar)
{
  // data_set_->ScalarType() = flag;
  mesh_data_->ScalarType() = flag;
  std::cout << "min scalar : " << min_scalar << std::endl;
  //  if(flag == kUniform)
  //  {
  if(lq::equal(mesh_data_->MaxColor(), max_scalar) && 
     lq::equal(mesh_data_->MinThreshold(), min_scalar))
    return ;
  mesh_data_->MaxColor() = max_scalar;
  mesh_data_->MinColor() = 0 - max_scalar;
  //  }
  
  mesh_data_->MinThreshold() = min_scalar;
  color_scalar_input_widget_handel_->hide();
  //repaint the error color
  if(data_set_->IsFirstColorScalar())
  {
    edit_item_operate_->MapErrorColor();
    slot_load_color_action();
  }
  
}

void menu_edit_item::slot_display_config_widget()
{
  //config_dialog_handle_->exec();
}

//end of slots
/******************************************************************************/
//begin private function

void menu_edit_item::create_action()
{
  
//  connect(this->edit_menu_handle_->load_color_action, SIGNAL(triggered()),
//          this, SLOT(slot_load_color_action()));
//  connect(this->edit_menu_handle_->clear_color_action, SIGNAL(triggered()),
//          this, SLOT(slot_clear_color_action()));
  connect(this->view_point_input_widget_handel_.get(), 
          SIGNAL(signal_view_point(double, double, double)),
          this, SLOT(slot_get_view_point(double, double, double)));
  
  connect(this->edit_menu_handle_->ColorScalarAction(), SIGNAL(triggered()),
          this, SLOT(slot_display_color_scalar_widget()));
//  connect(this->edit_menu_handle_->OptionAction(), SIGNAL(triggered()),
//          this, SLOT(slot_display_config_widget()));
  
  connect(this->color_scalar_input_widget_handel_.get(), 
          SIGNAL(signal_color_scalar(sqi::meshdata::ColorScalarType,double, double)),
          this, SLOT(slot_get_color_scalar(sqi::meshdata::ColorScalarType,double, double)));
}


}//end of namespace 
