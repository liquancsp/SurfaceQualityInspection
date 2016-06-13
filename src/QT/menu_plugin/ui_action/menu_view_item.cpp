#include "menu_view_item.h"
#include <configure/configure.h>
#include <vtk_data/vtk_model_data.h>
#include <Facade/MenuOperator.h>
#include <widget_plugin/ui_action/color_scalar_input_widget.h>

#include <vtkRenderWindow.h>
#include <QtGui>

namespace lq
{
using namespace sqi::meshdata;
using namespace sqi::common;

menu_view_item::menu_view_item()
{
  
}

menu_view_item::menu_view_item(MeshDataPtr mesh, vtkSmartPointer<vtk_model_data> data,
                               QWidget *parent) : QMenu(parent),
  view_menu_handle_(new Ui::ui_menu_view_item()),
  mesh_data_(mesh), data_set_(data),
  menu_operator_(new MenuOperator(mesh, data)),
  color_scalar_input_widget_handel_(new color_scalar_input_widget(mesh))
{
  view_menu_handle_->setup_ui(this);
  create_action();
}

menu_view_item::~menu_view_item()
{
}

void menu_view_item::set_data_set(const vtkSmartPointer<vtk_model_data> &other_data_set)
{
  this->data_set_ = other_data_set;
}

Ui::ui_menu_view_item* menu_view_item::GetUiHandle()
{
  return this->view_menu_handle_.get();
}

/******************************************************************************/
//begin slots

void menu_view_item::slot_show_left_region()
{
//  if(view_menu_handle_->ShowBorderPanelAction()->isChecked())
//  {
//    emit signal_show_left_region(kChecked);
//  }
//  else
//  {
//    emit signal_show_left_region(kNotChecked);
//  }
}

void menu_view_item::slot_add_color_legend()
{
 // view_item_operate_->set_mesh_data(mesh_data_);
 // view_item_operate_->add_color_legend();
}

void menu_view_item::slot_resize_color_bar()
{
  menu_operator_->ResizeColorLegend();
}

void menu_view_item::slot_clear_color()
{
  menu_operator_->ClearSceneColorItem(lq::kLeft);
  menu_operator_->ClearSceneColorItem(lq::kRight);
  emit signal_clear_color();
}

void menu_view_item::slot_show_back_side()
{
  //view_item_operate_->show_back_side();
  emit signal_flip_model();
}

void menu_view_item::slot_load_color_action()
{
  //edit_item_operate_->set_color_on_poly();
  emit signal_load_color();
}

void menu_view_item::slot_display_color_scalar_widget()
{
  color_scalar_input_widget_handel_->InitParmeters();
  color_scalar_input_widget_handel_->exec();
}

void menu_view_item::slot_get_color_scalar(sqi::meshdata::ColorScalarType flag, 
                                           double max_scalar, double min_scalar)
{
  menu_operator_->UpdateColorScalar(flag, max_scalar, min_scalar);
  
  color_scalar_input_widget_handel_->hide();
}

void menu_view_item::slot_flip_left_action()
{
  menu_operator_->FlipNormalOperator(kLeft);
}

void menu_view_item::slot_flip_right_action()
{
  menu_operator_->FlipNormalOperator(kRight);
}

void menu_view_item::slot_new_action()
{
  view_menu_handle_->FlipLeftAction()->setChecked(false);
  view_menu_handle_->FlipRightAction()->setChecked(false);
}

//end of slots
//==============================================================================

void menu_view_item::create_action()
{
//  connect(this->view_menu_handle_->ShowBorderPanelAction(), SIGNAL(triggered()),
//          this, SLOT(slot_show_left_region()));
  connect(this->view_menu_handle_->ColorScalarAction(), SIGNAL(triggered()),
          this, SLOT(slot_display_color_scalar_widget()));
  connect(this->color_scalar_input_widget_handel_.get(), 
          SIGNAL(signal_color_scalar(sqi::meshdata::ColorScalarType,double, double)),
          this, SLOT(slot_get_color_scalar(sqi::meshdata::ColorScalarType,double, double)));
  
  connect(this->view_menu_handle_->ClearColorAction(),SIGNAL(triggered()),
          this, SLOT(slot_clear_color()));
  
  connect(this->view_menu_handle_->ShowBackSide(), SIGNAL(triggered()),
          this, SLOT(slot_show_back_side()));
  connect(this->view_menu_handle_->FlipLeftAction(), SIGNAL(triggered()),
          this, SLOT(slot_flip_left_action()));
  connect(this->view_menu_handle_->FlipRightAction(), SIGNAL(triggered()),
          this, SLOT(slot_flip_right_action()));
  //  connect(this->view_menu_handle_->add_color_legend_action, SIGNAL(triggered()),
//          this, SLOT(slot_add_color_legend()));

}

}//end of namespace
