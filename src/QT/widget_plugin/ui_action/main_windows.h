/****************************************************************!**
 *COPYRIGHT NOTICE
 *Copyright (c) 2012, Zhe Jiang University
 *All rights reserved.
 *
 *@file model_win.h
 *@author li quan
 *@version 1.0
 *@date 2013-3
 *@ingroup QT/widget_plugin
 *
 *@brief this file function is to deal with the most operation of the
 * UI of QT. 
 *
 *******************************************************************/

#ifndef MAIN_WINDOWS_H
#define MAIN_WINDOWS_H

#include "left_region_win.h"

//#include <menu_plugin/ui_action/menu_edit_item.h>

#include <QMainWindow>
#include <vtkSmartPointer.h>

#include <MeshData.h>
#include <widget_operate/ModelNameTextActor.h>

#if defined(_MSC_VER)
#include <memory>
#endif

#if defined(__GNUC__)
#include <tr1/memory>
#endif

class Ui_ModelView;

namespace sqi{
namespace qt{
class StatusBar;
}
}

namespace lq
{

class menu_file_item;
class menu_inspect_item;
class menu_descriptor_item;
class menu_classify_item;
class menu_view_item;
class MenuStatusControl;
    
  /**
   *@class main_windows
   *@ingroup QT/widget_plugin/ui_action
   *@brief this file function is to deal with the most operation of the
   * UI of QT.
   *
   */
  class main_windows : public QMainWindow
  {
    Q_OBJECT
        
 public:
    main_windows();
    ~main_windows();

 public slots:

    void slot_add_tree_leaf(const std::string &file_name);
    void slot_show_left_region(int flag);
    void slot_new_action();
    void slot_delete_render_model(const QString &qt_file_name);
    void slot_delete_render_model(lq::WindowFlag flag);
    void slot_enabled_color_bar();
    void slot_update_text_actor();
    void slot_clear_text_actor(lq::WindowFlag flag);
    void slot_clear_two_actor();
    void SlotMenuStatusChange();
    void SlotMenuStatusResume();
    //void slot_
 protected:
    
    //    bool eventFilter(QObject *obj, QEvent *event);
    
 private:
  
    main_windows &operator = (const main_windows &other);
    void init();
    void create_action();
    vtkSmartPointer<vtk_model_data> data_set_;
    sqi::meshdata::MeshDataPtr mesh_data_;
    std::tr1::shared_ptr<Ui_ModelView> ui_main_panel_;
    std::tr1::shared_ptr<left_region_win> ui_left_region_;
    //menu items 
    std::tr1::shared_ptr<menu_file_item> ui_file_item_;
    std::tr1::shared_ptr<menu_inspect_item> ui_inspect_item_;
    std::tr1::shared_ptr<menu_descriptor_item> ui_descriptor_item_;
    std::tr1::shared_ptr<menu_classify_item> ui_classify_item_;
    std::tr1::shared_ptr<menu_view_item> ui_view_item_;
    std::tr1::shared_ptr<MenuStatusControl> menu_status_control_;
    std::tr1::shared_ptr<sqi::qt::StatusBar> ui_status_bar_;
    std::tr1::shared_ptr<sqi::WidgetOp::ModelNameTextActor<vtk_model_data> > model_name_text_actor_;
   
  };
}


#endif
