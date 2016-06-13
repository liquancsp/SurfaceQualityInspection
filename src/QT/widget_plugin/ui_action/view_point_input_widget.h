/******************************************************************************************
 *COPYRIGHT NOTICE
 *Copyright (c) 2012, Zhe Jiang University
 *All rights reserved.
 *
 *@file view_point_input_widget.h
 *@author li quan
 *@version 2.0
 *@date 2013-3-7
 *@ingroup QT/widget_plugin
 *
 *@brief this file function is to get view point input
 *
 *
 ******************************************************************************************/
#ifndef VIEW_POINT_INPUT_WIDGET_H
#define VIEW_POINT_INPUT_WIDGET_H
#include <QDialog>
#include <widget_plugin/ui_resource/ui_view_point_input_widget.h>
#if defined(_MSC_VER)
#include <memory>
#endif

#if defined(__GNUC__)
#include <tr1/memory>
#endif
namespace lq
{

  class view_point_input_widget : public QDialog
  {
    Q_OBJECT

  public:

    view_point_input_widget();
    ~view_point_input_widget();

 signals:

    void signal_view_point(double x, double y, double z);

 public slots:

    void slot_view_point();
    
 private:

    std::tr1::shared_ptr<Ui::ui_view_point_input_widget> view_point_input_widget_handel_;
    void create_action();
    
  };
}

#endif
