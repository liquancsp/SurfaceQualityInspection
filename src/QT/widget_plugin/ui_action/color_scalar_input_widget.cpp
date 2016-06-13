#include "color_scalar_input_widget.h"
#include <iostream>

namespace lq 
{

color_scalar_input_widget::color_scalar_input_widget(sqi::meshdata::MeshDataPtr mesh):
mesh_data_(mesh)
{
  this->color_scalar_input_handle_.reset(new Ui::ui_color_scalar_input());
  color_scalar_input_handle_->setup_ui(this);
  create_action();
  
}

color_scalar_input_widget::~color_scalar_input_widget()
{
  
}

void color_scalar_input_widget::InitParmeters()
{
  double max_error = std::max(mesh_data_->MaxE(), fabs(mesh_data_->MinE()));
  if(mesh_data_->ScalarType() == sqi::meshdata::kUniform)
  {
    if(!is_zero(mesh_data_->MaxColor()))
      this->color_scalar_input_handle_->color_scalar_text->setText(QString::number(mesh_data_->MaxColor()));
    else 
      this->color_scalar_input_handle_->color_scalar_text->setText(QString::number(max_error));
    this->color_scalar_input_handle_->min_threshold_text->setText(QString::number(mesh_data_->MinThreshold()));
  }
  else 
  {
    this->color_scalar_input_handle_->color_scalar_text->setText(QString::number(max_error));
    this->color_scalar_input_handle_->min_threshold_text->setText(QString::number(0.0f));
  }
}

void color_scalar_input_widget::create_action()
{
  connect(this->color_scalar_input_handle_->ok_btn_action, SIGNAL(clicked()),
          this, SLOT(slot_color_scalar()));
  connect(this->color_scalar_input_handle_->cancel_btn_action, SIGNAL(clicked()),
          this, SLOT(slot_cancel_btn()));
}

//begin slot

void color_scalar_input_widget::slot_color_scalar()
{
//  sqi::meshdata::ColorScalarType flag;
//  if(this->color_scalar_input_handle_->indepent_radio->isChecked())
//  {
//    flag = sqi::meshdata::kIndependent;
//  }
//  else 
//  flag = sqi::meshdata::kUniform;
  QString max_scalar;
  max_scalar = this->color_scalar_input_handle_->color_scalar_text->text();
  
  QString min_scalar;
  min_scalar = this->color_scalar_input_handle_->min_threshold_text->text();
  
  double scalar1, scalar2;
  std::string str_scalar;
  str_scalar = qPrintable(max_scalar);
  scalar1 = atof(str_scalar.c_str());
  str_scalar = qPrintable(min_scalar);
  scalar2 = atof(str_scalar.c_str());
  emit signal_color_scalar(sqi::meshdata::kUniform, scalar1, scalar2);
}

void color_scalar_input_widget::slot_cancel_btn()
{
  this->hide();
}

};
