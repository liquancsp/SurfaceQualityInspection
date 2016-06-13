#pragma once
#ifndef UI_MENU_CONTEXT_EVENT_H
#define UI_MENU_CONTEXT_EVENT_H

#include <QtGui/QAction>
#include <QtGui/QMenu>

QT_BEGIN_NAMESPACE

namespace Ui
{

class ui_menu_context_event 
{
  
public :
 
  ui_menu_context_event(){}
  ~ui_menu_context_event(){}
  
  QAction* open_file_action() {return open_file_action_;}
  QAction* new_action() {return new_action_;}
  QAction* show_points_action() {return show_points_action_;}
  QAction* show_surface_action() {return show_surface_action_;}
  QAction* show_wireframe_action() {return show_wireframe_action_;}
  QAction* show_surface_with_edge_action() {return show_surface_with_edge_action_;}
  QAction* delete_model_action() {return delete_model_action_;}
  QActionGroup* action_group() {return action_group_;}
  
  void setup_ui(QMenu* pop_menu);
  void retranslate_ui(QMenu* pop_menu);

protected:
  
  QAction* open_file_action_;
  QAction* new_action_;
  QAction* show_points_action_;
  QAction* show_surface_action_;
  QAction* show_wireframe_action_;
  QAction* show_surface_with_edge_action_;
  QAction* delete_model_action_;
  QActionGroup* action_group_;
  
};

}
QT_END_NAMESPACE
#endif // UI_MENU_CONTEXT_EVENT_H
