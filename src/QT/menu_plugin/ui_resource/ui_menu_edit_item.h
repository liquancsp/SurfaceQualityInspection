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

#ifndef UI_MENU_EDIT_H
#define UI_MENU_EDIT_H

#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QMenu>

QT_BEGIN_NAMESPACE
namespace Ui
{
  
  class ui_menu_edit_item
  {
  
 public:

    QAction* ColorScalarAction()        {return color_scalar_action_;}
    //QAction* OptionAction()             {return option_action_;}
    ui_menu_edit_item();
    ~ui_menu_edit_item();
        
    void setup_ui(QMenu *menu_edit_item);

  protected:
    
    QAction *color_scalar_action_;
    //QAction* option_action_;
    
 private :
    void retranslate_ui(QMenu *menu_edit_item);
   
  };
   
}// namespace Ui
QT_END_NAMESPACE

#endif // UI_MENU_EDIT_H
