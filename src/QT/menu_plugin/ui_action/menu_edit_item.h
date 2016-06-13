/*************************************************************************************
 *
 *COPYRIGHT NOTICE
 *Copyright (c) 2013, Zhe Jiang University
 *All rights reserved.
 *
 *@file menu_edititem.h
 *@author li quan
 *
 *version 2.0
 *data 2013-3
 *ingroup QT/menu_plugin/ui_action
 *
 *@brief this file is to support the function of menubar->Edit item. It include &Inspect,
 *&Load Color, &Clear Color
 *
 *************************************************************************************/
#ifndef MENU_EDIT_ITEM_H
#define MENU_EDIT_ITEM_H

#include <menu_plugin/ui_resource/ui_menu_edit_item.h>
#include <vtk_data/vtk_model_data.h>
#include <menu_operate/edit_menu_operate.h>
#include <widget_operate/view_point_operate.h>
#include <widget_plugin/ui_action/view_point_input_widget.h>
#include <widget_plugin/ui_action/color_scalar_input_widget.h>
//#include <widget_plugin/ui_action/ConfigDialog.h>
#include <widget_plugin/option_widget/ConfigDialog.h>
#include <MeshData.h>

#include <vtkSmartPointer.h>

namespace lq
{

   /*********************************************************************************
   *
   *@class menu_edit_item
   *@ingroup qt_vtk
   *@brief this file is to support the function of menubar->Edit item. &Inspect,
   *&Load Color, &Clear Color
   *
  **********************************************************************************/
  class menu_edit_item : public QMenu
  {
      Q_OBJECT

  public:

   
    menu_edit_item(QWidget* PARENT = 0);
    ~menu_edit_item();
    void set_data_set(const vtkSmartPointer<vtk_model_data> &other_data_set);
    void set_mesh_data(const sqi::meshdata::MeshDataPtr& other)
    {
      this->mesh_data_ = other;
      edit_item_operate_->SetMeshData(mesh_data_);
    }
    
    std::tr1::shared_ptr<Ui::ui_menu_edit_item> edit_menu_handle_;
 signals:

    void signal_complete_inspect();
    void signal_load_color();
    
 public slots:

    //void slot_inspect_action();
   
    void slot_load_color_action();
    void slot_clear_color_action();
    void slot_get_view_point(double x, double y, double z);
    void slot_display_color_scalar_widget();
    void slot_get_color_scalar(size_t flag, double max_scalar, double min_scalar);
    void slot_display_config_widget();
    
 private:

    vtkSmartPointer<vtk_model_data> data_set_;
    sqi::meshdata::MeshDataPtr mesh_data_;
    std::tr1::shared_ptr<sqi::WidgetAction::ConfigDialog> config_dialog_handle_;
    std::tr1::shared_ptr<EditMenuOperate> edit_item_operate_;
    std::tr1::shared_ptr<view_point_operate> view_point_operate_;
    std::tr1::shared_ptr<color_scalar_input_widget> color_scalar_input_widget_handel_;
    std::tr1::shared_ptr<view_point_input_widget> view_point_input_widget_handel_;
   
    void create_action();
    
  };
  
}//end of namespace 


#endif
