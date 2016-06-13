/*************************************************************************************
 *
 *COPYRIGHT NOTICE
 *Copyright (c) 2013, Zhe Jiang University
 *All rights reserved.
 *
 *@file ui_menu_view.cpp
 *@author li quan
 *
 *version 2.0
 *data 2013-3-3
 *ingroup QT/menu_plugin/ui_resource/
 *
 *@brief this file is to support the items of menubar->View item. It include &show border panel,
 *
 *************************************************************************************/

#include "ui_menu_view_item.h"

namespace Ui
{

ui_menu_view_item::ui_menu_view_item()
{

}

ui_menu_view_item::~ui_menu_view_item()
{
 
}

void ui_menu_view_item::setup_ui(QMenu *menu_view_item)
{
  if(menu_view_item->objectName().isEmpty())
  {
    menu_view_item->setObjectName(QString::fromUtf8("menu_view_item"));
  }
//  show_border_panel_action_ = new QAction(menu_view_item);
//  show_border_panel_action_->setObjectName(QString::fromUtf8("show_border_panel_action"));
//  show_border_panel_action_->setCheckable(true);
  color_scalar_action_ = new QAction(menu_view_item);
  color_scalar_action_->setObjectName(QString::fromUtf8("color_scalar_action_"));
  
  clear_color_action_ = new QAction(menu_view_item);
  clear_color_action_->setObjectName(QString::fromUtf8("clear_color_action_"));
  clear_color_action_->setCheckable(false);
  
  show_back_side_action_ = new QAction(menu_view_item);
  show_back_side_action_->setObjectName(QString::fromUtf8("show_back_side_action"));
  show_back_side_action_->setCheckable(true);
  
//  add_color_legend_action = new QAction(menu_view_item);
//  add_color_legend_action->setObjectName(QString::fromUtf8("add_color_legend_action"));
//  add_color_legend_action->setEnabled(false);
  
//  link_camera_action = new QAction(menu_view_item);
//  link_camera_action->setObjectName(QString::fromUtf8("link_camera_action"));
  
//  menu_view_item->addAction(show_border_panel_action_);
  
  menu_view_item->addAction(color_scalar_action_);
  menu_view_item->addAction(clear_color_action_);
 // menu_view_item->addAction(show_back_side_action_);
  flip_menu_ = menu_view_item->addMenu("Flip");
  
  flip_left_action_ = new QAction(flip_menu_);
  flip_left_action_->setObjectName(QString::fromUtf8("flip_left_action_"));
  flip_left_action_->setCheckable(true);
  
  flip_right_action_ = new QAction(flip_menu_);
  flip_right_action_->setObjectName(QString::fromUtf8("flip_right_action_"));
  flip_right_action_->setCheckable(true);
  
  flip_menu_->addAction(flip_left_action_);
  flip_menu_->addAction(flip_right_action_);
  
  //menu_view_item->addAction(add_color_legend_action);
  retranslate_ui(menu_view_item);
  QMetaObject::connectSlotsByName(menu_view_item);
}

void ui_menu_view_item::retranslate_ui(QMenu *menu_view_item)
{
  menu_view_item->setTitle(QApplication::translate("menu_view_item", "&View",
                                                    0, QApplication::UnicodeUTF8));

//  show_border_panel_action_->setText(QApplication::translate("menu_view_item", "&Show Border Panel",
//                                                             0, QApplication::UnicodeUTF8));

//  show_border_panel_action_->setShortcut(QApplication::translate("menu_view_item", "Ctrl+S",
//                                                                 0, QApplication::UnicodeUTF8));
//  show_border_panel_action_->setIcon(QIcon("./images/show-hide.png"));
  
  color_scalar_action_->setText(QApplication::translate("menu_view_item", "Color Scalar",
                                                        0,QApplication::UnicodeUTF8));
  
  color_scalar_action_->setIcon(QIcon("./images/colors.png"));
  
  clear_color_action_->setText(QApplication::translate("menu_view_item", "Clear Color",
                                                       0, QApplication::UnicodeUTF8));
  
  
  show_back_side_action_->setText(QApplication::translate("menu_view_item", "&Show Back Side", 
                                                          0, QApplication::UnicodeUTF8));
 
  flip_left_action_->setText(QApplication::translate("flip_menu_", "&Flip Left",
                                                     0, QApplication::UnicodeUTF8));
  
  flip_right_action_->setText(QApplication::translate("flip_menu_", "&Flip right",
                                                     0, QApplication::UnicodeUTF8));
//  add_color_legend_action->setText(QApplication::translate("menu_view_item", "&Add Color Legend",
//                                                            0, QApplication::UnicodeUTF8));
//  add_color_legend_action->setShortcut(QApplication::translate("menu_view_item", "Ctrl+A",
//                                                                0, QApplication::UnicodeUTF8));

 
}

}// end of namespace
