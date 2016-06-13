/*************************************************************************************
 *
 *COPYRIGHT NOTICE
 *Copyright (c) 2013, Zhe Jiang University
 *All rights reserved.
 *
 *@file ui_menu_edit.h
 *@author li quan
 *
 *version 2.0
 *data 2013-3
 *ingroup QT/menu_plugin/ui_resource/
 *
 *@brief this file is to support the items of menubar->edit item. It include Inspection,
 *&Load Color, &Clear Color.
 *
 *************************************************************************************/

#include "ui_menu_edit_item.h"

namespace Ui
{

ui_menu_edit_item::ui_menu_edit_item()
{
}

ui_menu_edit_item::~ui_menu_edit_item()
{

}//end of ~ui_menu_edit_item

void ui_menu_edit_item::setup_ui(QMenu *menu_edit_item)
{
  if(menu_edit_item->objectName().isEmpty())
  {
    menu_edit_item->setObjectName(QString::fromUtf8("menu_eidt_item"));
  }
//  //&load_color
//  load_color_action = new QAction(menu_edit_item);
//  load_color_action->setObjectName(QString::fromUtf8("load_color_action"));
//  load_color_action->setEnabled(false);
//  //&clear_color
//  clear_color_action = new QAction(menu_edit_item);
//  clear_color_action->setObjectName(QString::fromUtf8("clear_color_action"));
//  clear_color_action->setEnabled(false);
  
  //color scalar
  color_scalar_action_ = new QAction(menu_edit_item);
  color_scalar_action_->setObjectName(QString::fromUtf8("color_scalar_action_"));
  
//  //config dialog
//  option_action_ = new QAction(menu_edit_item);
//  option_action_->setObjectName(QString::fromUtf8("option_action_"));
  
////  menu_edit_item->addAction(load_color_action);
////  menu_edit_item->addAction(clear_color_action);
  menu_edit_item->addAction(color_scalar_action_);
//  menu_edit_item->addSeparator();
//  menu_edit_item->addAction(option_action_);
  
  retranslate_ui(menu_edit_item);
  QMetaObject::connectSlotsByName(menu_edit_item);
}// end of setup_ui

void ui_menu_edit_item::retranslate_ui(QMenu *menu_edit_item)
{
  menu_edit_item->setTitle(QApplication::translate("menu_edit_item", "&Edit",
                                                   0, QApplication::UnicodeUTF8));

//  load_color_action->setText(QApplication::translate("menu_edit_item", "&Load Color...",
//                                                     0, QApplication::UnicodeUTF8));
//  load_color_action->setShortcut((QApplication::translate("menu_edit_item", "Ctrl+L",
//                                                          0, QApplication::UnicodeUTF8)));
//  load_color_action->setIcon(QIcon("./images/colors.png"));
//  clear_color_action->setText(QApplication::translate("menu_edit_item", "&Clear Color...",
//                                                      0, QApplication::UnicodeUTF8));
//  clear_color_action->setShortcut((QApplication::translate("menu_edit_item", "Ctrl+C",
//                                                           0, QApplication::UnicodeUTF8)));
  
  color_scalar_action_->setText(QApplication::translate("menu_edit_item", "Color Scalar...",
                                                        0,QApplication::UnicodeUTF8));
//  option_action_->setText(QApplication::translate("menu_edit_item", "Option",
//                                                  0,QApplication::UnicodeUTF8));
  
}// end of retranslate_ui

}//end of namespace 
