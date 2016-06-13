/*************************************************************************************
 *
 *COPYRIGHT NOTICE
 *Copyright (c) 2013, Zhe Jiang University
 *All rights reserved.
 *
 *@file ui_menu_view.h
 *@author li quan
 *
 *version 2.0
 *data 2013-3-3
 *ingroup QT/menu_plugin/ui_resource/
 *
 *@brief this file is to support the items of menubar->View item. It include &show border panel,
 *
 *************************************************************************************/

#ifndef UI_MENU_VIEW_H
#define UI_MENU_VIEW_H

#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QMenu>

QT_BEGIN_NAMESPACE
namespace Ui
{
  
  class ui_menu_view_item
  {
  
 public:
  
//    QAction* ShowBorderPanelAction()  {return show_border_panel_action_;}
    QAction* const ColorScalarAction()    const    {return color_scalar_action_;}
    QAction* const ClearColorAction()     const    {return clear_color_action_;}
    QAction* const ShowBackSide()         const    {return show_back_side_action_;}
    QAction* const FlipLeftAction()       const    {return flip_left_action_;}
    QAction* const FlipRightAction()      const    {return flip_right_action_;}
    //QAction *add_color_legend_action;
    ui_menu_view_item();
    ~ui_menu_view_item();
        
    void setup_ui(QMenu *menu_view_item);
    void SetActionEnable(bool flag, QAction* const action)  
    {action->setEnabled(flag);}
    
    void SetActionEnable(bool flag)
    {
      SetActionEnable(flag, color_scalar_action_);
      SetActionEnable(flag, clear_color_action_);
      SetActionEnable(flag, flip_left_action_);
      SetActionEnable(flag, flip_right_action_);
    }
    
  protected:
//     QAction *show_border_panel_action_;
    QAction *color_scalar_action_; 
    QAction *show_back_side_action_;
    QAction *clear_color_action_;
    QAction *flip_left_action_;
    QAction *flip_right_action_;
    QMenu *flip_menu_;
     
    
 private :
    void retranslate_ui(QMenu *menu_view_item);
   
  };
   
}// namespace Ui
QT_END_NAMESPACE

#endif // UI_MENU_VIEW_H
