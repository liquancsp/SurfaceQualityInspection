#include <menu_plugin/ui_resource/ui_menu_context_event.h>ui_menu_context_event.h>
#include <QtGui/QApplication>
namespace Ui
{


void ui_menu_context_event::setup_ui(QMenu *pop_menu)
{
  if(pop_menu->objectName().isEmpty())
  {
    pop_menu->setObjectName(QString::fromUtf8("pop_menu"));
  }
  
  open_file_action_ = new QAction(pop_menu);
  open_file_action_->setObjectName(QString::fromUtf8("open_file_action_"));
  
  new_action_ = new QAction(pop_menu);
  new_action_->setObjectName(QString::fromUtf8("new_action_"));
  
  action_group_ = new QActionGroup(pop_menu);
  show_points_action_ = new QAction(pop_menu);
  show_points_action_->setObjectName(QString::fromUtf8("show_points_action_"));
  show_points_action_->setCheckable(true);
  
  show_surface_action_ = new QAction(pop_menu);
  show_surface_action_->setObjectName(QString::fromUtf8("show_surface_action_"));
  show_surface_action_->setCheckable(true);
  show_surface_action_->setChecked(true);
  
  show_wireframe_action_ = new QAction(pop_menu);
  show_wireframe_action_->setObjectName(QString::fromUtf8("show_wireframe_action_"));
  show_wireframe_action_->setCheckable(true);
  
  show_surface_with_edge_action_ = new QAction(pop_menu);
  show_surface_with_edge_action_->setObjectName(QString::fromUtf8("show_surface_with_edge_action_"));
  show_surface_with_edge_action_->setCheckable(true);  
  
  show_points_action_->setActionGroup(action_group_);
  show_surface_action_->setActionGroup(action_group_);
  show_wireframe_action_->setActionGroup(action_group_);
  show_surface_with_edge_action_->setActionGroup(action_group_);
  
  delete_model_action_ = new QAction(pop_menu);
  delete_model_action_->setObjectName(QString::fromUtf8("delete_model_action_"));
  
  pop_menu->addAction(open_file_action_);
  pop_menu->addAction(new_action_);
  pop_menu->addSeparator();
  pop_menu->addAction(show_points_action_);
  pop_menu->addAction(show_surface_action_);
  pop_menu->addAction(show_wireframe_action_);
  pop_menu->addAction(show_surface_with_edge_action_);
  pop_menu->addSeparator();
  pop_menu->addAction(delete_model_action_);
  retranslate_ui(pop_menu);
  QMetaObject::connectSlotsByName(pop_menu);
}

void ui_menu_context_event::retranslate_ui(QMenu *pop_menu)
{
  open_file_action_->setText(QApplication::translate("open_file_action_","Open...",
                                                     0, QApplication::UnicodeUTF8));
  new_action_->setText(QApplication::translate("new_action_","New",
                                               0, QApplication::UnicodeUTF8));
  
  show_points_action_->setText(QApplication::translate("show_points_action_","Points",
                                                       0, QApplication::UnicodeUTF8));
  show_surface_action_->setText(QApplication::translate("show_surface_action_","Surface",
                                                       0, QApplication::UnicodeUTF8));
  show_wireframe_action_->setText(QApplication::translate("show_wireframe_action_","Wireframe",
                                                       0, QApplication::UnicodeUTF8));
  show_surface_with_edge_action_->setText(QApplication::translate("show_surface_with_edge_action_",
                                                                 "Surface with edge", 0,
                                                                 QApplication::UnicodeUTF8));
  
  delete_model_action_->setText(QApplication::translate("delete_model_action_","Delete",
                                                       0, QApplication::UnicodeUTF8));
  
}

}
