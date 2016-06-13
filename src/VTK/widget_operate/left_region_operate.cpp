/*************************************************************************************
 *
 *COPYRIGHT NOTICE
 *Copyright (c) 2013, Zhe Jiang University
 *All rights reserved.
 *
 *@file left_region_operate.cpp
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

#include "left_region_operate.h"
#include <vtkCellData.h>
#include <vtkLookupTable.h>
namespace lq
{

left_region_operate::left_region_operate()
{
  cell_color_ = vtkSmartPointer<vtkUnsignedCharArray>::New();
}

left_region_operate::~left_region_operate()
{
  
}

void left_region_operate::set_data_set(const vtkSmartPointer<vtk_model_data> &other_data_set)
{

  this->data_set_ = other_data_set;
}

void left_region_operate::set_spin_value(int other)
{
  this->spin_value_ = other;
}

void left_region_operate::set_spin_standard(int other)
{
  this->spin_standard_ = other;
}

void left_region_operate::update_legend_color()
{
//  std::vector<color> sub_error_color;
//  get_color_.update_color_list(data_set_->color_list, sub_error_color,
//                               data_set_->legend_color_list_, spin_value_, spin_standard_);
//  vtkSmartPointer<vtkLookupTable> lut =
//      vtkSmartPointer<vtkLookupTable>::New();
//  lut->SetNumberOfTableValues(data_set_->legend_color_list_.size());
//  for(size_t i = 0 ; i < data_set_->legend_color_list_.size(); ++i)
//    lut->SetTableValue(i , double(data_set_->legend_color_list_[i].R / 255),
//                       double(data_set_->legend_color_list_[i].G / 255),
//                       double(data_set_->legend_color_list_[i].B / 255), 1);
//  data_set_->scalar_bar_->SetLookupTable(lut);
//  data_set_->QTW()->update();
  
}

void left_region_operate::update_color_on_poly()
{
//  data_set_->color_index.clear();
//  data_set_->legend_color_list_.clear();
//  get_color_.generate_error_color(data_set_->color_list, data_set_->triangle_error_,
//                                  spin_value_, spin_standard_,
//                                  data_set_->legend_color_list_, data_set_->color_index);
  
//  //it must clear old color first, otherwise it can not update the color in realtime
//  cell_color_->Initialize();
//  cell_color_->SetNumberOfComponents(3);
//  cell_color_->SetName("default_color");
//  unsigned char tmp_color[3] = {255, 255, 255};
//  for(size_t i = 0; i < data_set_->triangle_error_.size(); ++i)
//  {
//    cell_color_->InsertNextTupleValue(tmp_color);
//  }
//  set_color_on_poly();
}

void left_region_operate::set_color_on_poly()
{
//  cell_color_->Reset();
//  cell_color_->SetNumberOfComponents(3);
//  cell_color_->SetName("error_color");
//  for(size_t i = 0; i < data_set_->triangle_error_.size(); ++i)
//  {
//    unsigned char tmp_color[3];
//    tmp_color[0] = data_set_->color_list[data_set_->color_index[i]].R;
//    tmp_color[1] = data_set_->color_list[data_set_->color_index[i]].G;
//    tmp_color[2] = data_set_->color_list[data_set_->color_index[i]].B;
//    cell_color_->InsertNextTupleValue(tmp_color);
//  }
    
//  data_set_->PD(kRight)->GetCellData()->SetScalars(cell_color_);
}
}

