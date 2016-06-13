/******************************************************************************************
 *COPYRIGHT NOTICE
 *Copyright (c) 2012, Zhe Jiang University
 *All rights reserved.
 *
 *@file view_point_input_widget.cpp
 *@author li quan
 *@version 2.0
 *@date 2013-3-7
 *@ingroup QT/widget_plugin
 *
 *@brief this file function is to get view point input
 *
 *
 ******************************************************************************************/

#include "view_point_input_widget.h"
#include <iostream>
namespace lq
{

view_point_input_widget::view_point_input_widget()
{
  this->view_point_input_widget_handel_.reset(new Ui::ui_view_point_input_widget);
  view_point_input_widget_handel_->setup_ui(this);
  create_action();
}

view_point_input_widget::~view_point_input_widget()
{
  //delete view_point_input_widget_handel_;
}


void view_point_input_widget::slot_view_point()
{
  QString qt_x, qt_y, qt_z;
  qt_x = this->view_point_input_widget_handel_->edit_box_x->text();
  qt_y = this->view_point_input_widget_handel_->edit_box_y->text();
  qt_z = this->view_point_input_widget_handel_->edit_box_z->text();
  double  x, y, z;
  std::string str_x, str_y, str_z;
  str_x = qPrintable(qt_x);
  str_y = qPrintable(qt_y);
  str_z = qPrintable(qt_z);
  x = atof(str_x.c_str());
  y = atof(str_y.c_str());
  z = atof(str_z.c_str());
  emit signal_view_point(x, y, z);
}

void view_point_input_widget::create_action()
{
  connect(this->view_point_input_widget_handel_->btn_ok, SIGNAL(clicked()),
          this, SLOT(slot_view_point()));
}

}
