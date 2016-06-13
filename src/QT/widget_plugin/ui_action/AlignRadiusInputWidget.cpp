#include <widget_plugin/ui_action/AlignRadiusInputWidget.h>

namespace lq {

AlignRadiusInputWidget::AlignRadiusInputWidget():
  radius_input_handle_(new Ui_AlignRadiusInput())
{
  radius_input_handle_->setupUi(this);
  create_action();
}

AlignRadiusInputWidget::~AlignRadiusInputWidget()
{
  
}

void AlignRadiusInputWidget::slot_ok_btn_action()
{
  QString qt_radius;
  qt_radius = this->radius_input_handle_->align_radius_text->text();
  
  double radius;
  std::string str_radius;
  str_radius = qPrintable(qt_radius);
  radius = atof(str_radius.c_str());
  emit SignalAlignRadius(radius);
}

void AlignRadiusInputWidget::slot_cancel_btn_action()
{
  this->hide();
}

void AlignRadiusInputWidget::create_action()
{
  connect(radius_input_handle_->ok_btn_action, SIGNAL(clicked()),
          this, SLOT(slot_ok_btn_action()));
  connect(radius_input_handle_->cancel_btn_action, SIGNAL(clicked()),
          this, SLOT(slot_cancel_btn_action()));
}



}
