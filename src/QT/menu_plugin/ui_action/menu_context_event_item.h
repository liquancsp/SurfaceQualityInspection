#pragma once
#ifndef MENU_CONTEXT_EVENT_ITEM_H
#define MENU_CONTEXT_EVENT_ITEM_H

#include <menu_plugin/ui_resource/ui_menu_context_event.h>
#include <vtkSmartPointer.h>


#if defined(_MSC_VER)
#include <memory>
#endif

#if defined(__GNUC__)
#include <tr1/memory>
#endif

namespace lq
{

class menu_context_event_item : public QMenu
{
  Q_OBJECT
public:
  menu_context_event_item(QWidget* parent = 0);
  ~menu_context_event_item();
  QAction* open_file_action() {return context_menu_handle_->open_file_action();}
  QAction* new_action() {return context_menu_handle_->new_action();}
  QAction* show_points_action() {return context_menu_handle_->show_points_action();}
  QAction* show_surface_action() {return context_menu_handle_->show_surface_action();}
  QAction* show_wireframe_action() {return context_menu_handle_->show_wireframe_action();}
  QAction* show_surface_with_edge_action() {return context_menu_handle_->show_surface_with_edge_action();}
  QAction* delete_model_action() {return context_menu_handle_->delete_model_action();}
  
protected:
  std::tr1::shared_ptr<Ui::ui_menu_context_event> context_menu_handle_;
};
    
}

#endif // MENU_CONTEXT_EVENT_ITEM_H
