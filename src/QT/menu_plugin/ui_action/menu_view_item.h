/*************************************************************************************
 *
 *COPYRIGHT NOTICE
 *Copyright (c) 2013, Zhe Jiang University
 *All rights reserved.
 *
 *@file menu_view_item.h
 *@author li quan
 *
 *version 2.0
 *data 2013-3-4
 *ingroup QT/menu_plugin/ui_action
 *
 *@brief this file is to support the function of menubar->View item. It include &show border panel
 *
 *************************************************************************************/
#ifndef MENU_VIEW_ITEM_H
#define MENU_VIEW_ITEM_H

#include <menu_plugin/ui_resource/ui_menu_view_item.h>
#include <MeshData.h>
#include <vtkSmartPointer.h>

namespace sqi {
namespace common{
class MenuOperator;
}
}

namespace lq
{

class vtk_model_data;
class color_scalar_input_widget;

   /*********************************************************************************
   *
   *@class menu_view_item
   *@ingroup QT/menu_plugin/ui_action
   *@brief this file is to support the function of menubar->View item. &show border panel
   *
  **********************************************************************************/
  class menu_view_item : public QMenu
  {
    Q_OBJECT

    public:

    menu_view_item(sqi::meshdata::MeshDataPtr mesh,
                   vtkSmartPointer<vtk_model_data> data,QWidget* parent = 0);
    ~menu_view_item();
    void set_data_set(const vtkSmartPointer<vtk_model_data> &other_data_set);
    void set_mesh_data(const sqi::meshdata::MeshDataPtr& other)
    {
      this->mesh_data_ = other;
    }
	Ui::ui_menu_view_item* GetUiHandle();
 signals:

    void signal_show_left_region(int flag);
    void signal_flip_model();
    void signal_load_color();
    void signal_clear_color();
    
 public slots:

    void slot_show_left_region();
    void slot_add_color_legend();
    void slot_resize_color_bar();
    void slot_clear_color();
    void slot_show_back_side();
    void slot_display_color_scalar_widget();
    void slot_get_color_scalar(sqi::meshdata::ColorScalarType flag,
                               double max_scalar, double min_scalar);
    void slot_load_color_action();
    void slot_flip_left_action();
    void slot_flip_right_action();
    void slot_new_action();
    
 private:
    menu_view_item();
    std::tr1::shared_ptr<Ui::ui_menu_view_item> view_menu_handle_;    
    sqi::meshdata::MeshDataPtr mesh_data_;
    vtkSmartPointer<vtk_model_data> data_set_;
    std::tr1::shared_ptr<sqi::common::MenuOperator> menu_operator_;
    std::tr1::shared_ptr<color_scalar_input_widget> color_scalar_input_widget_handel_;
    void create_action();
  };
  
}//end of namespace 


#endif
