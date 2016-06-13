#include <HeightMapRadiusRatioWidget.h>

namespace sqi{
namespace ui {

HeightMapRadiusRatioWidget::HeightMapRadiusRatioWidget() :
  ratio_input_handle_(new Ui_HeightMapRadiusRatio())
{
  ratio_input_handle_->setupUi(this);
  CreateAction();
}

HeightMapRadiusRatioWidget::~HeightMapRadiusRatioWidget()
{
  
}

void HeightMapRadiusRatioWidget::SlotOkAction()
{
  QString qt_ratio, qt_multi, qt_step;
  qt_ratio = this->ratio_input_handle_->ratio_action_->text();
  qt_multi = this->ratio_input_handle_->multiplier_action_->text();
  qt_step = this->ratio_input_handle_->smooth_step_action_->text();
  
  double ratio, multi;
  std::string str_ratio, str_multi, str_step;
  str_ratio = qPrintable(qt_ratio);
  str_multi = qPrintable(qt_multi);
  str_step = qPrintable(qt_step);

  ratio = atof(str_ratio.c_str());
  multi = atof(str_multi.c_str());
  int step;
  step = atoi(str_step.c_str());
  
  emit SignalRadiusRatio(multi, ratio, step);
}

void HeightMapRadiusRatioWidget::SlotCancelAction()
{
  this->hide();
}

void HeightMapRadiusRatioWidget::CreateAction()
{
  connect(ratio_input_handle_->ok_btn_action, SIGNAL(clicked()),
          this, SLOT(SlotOkAction()));
  connect(ratio_input_handle_->cancel_btn_action, SIGNAL(clicked()),
          this, SLOT(SlotCancelAction()));
}

}

}
