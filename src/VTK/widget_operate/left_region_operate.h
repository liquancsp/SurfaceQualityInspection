/*************************************************************************************
 *
 *COPYRIGHT NOTICE
 *Copyright (c) 2013, Zhe Jiang University
 *All rights reserved.
 *
 *@file left_region_operate.h
 *@author li quan
 *
 *version 2.0
 *data 2013-3-6
 *ingroup VTK/widget_operate
 *
 *@brief this file is to support the operate of ui_left_region. It include change color
 *delete model 
 *
 *************************************************************************************/
#ifndef LEFT_REGION_OPERATE_H
#define LEFT_REGION_OPERATE_H
#include <vtk_data/vtk_model_data.h>
#include <load_color.h>

#include <vtkSmartPointer.h>
#include <vtkUnsignedCharArray.h>


namespace lq
{
  /***************************************************************************************
   *@class edit_menu_operate
   *@brief  this class is to support the operate of ui_left_region. It include change color
   *delete model 
   *
   *@detail
   *
   *@data 2013-3-6
   *@version 2.0
   ****************************************************************************************/
  class left_region_operate
  {

 public :

    left_region_operate();
    ~left_region_operate();
    void set_data_set(const vtkSmartPointer<vtk_model_data> &other_data_set);
    void set_spin_value(int other);
    void set_spin_standard(int other);
    void update_legend_color();
    void update_color_on_poly();
    
 private:

    vtkSmartPointer<vtk_model_data> data_set_;
    vtkSmartPointer<vtkUnsignedCharArray> cell_color_;
    void set_color_on_poly();
    load_color get_color_;
    int spin_value_;
    int spin_standard_;
  };

}

#endif
