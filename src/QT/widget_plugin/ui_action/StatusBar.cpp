#include <widget_plugin/ui_action/StatusBar.h>
#include <configure/configure.h>
#include <QHBoxLayout>
#include <iostream>

#include <QTimer>

namespace sqi {
namespace qt{

using namespace lq;

StatusBar::StatusBar(QWidget *parent) : QStatusBar(parent),
  label_(new QLabel(this)), 
  progress_bar_(new QProgressBar(this)),
  count(0)
{
  //this->setSizeGripEnabled(true);
  this->setMaximumHeight(20);
  this->progress_bar_->setMinimumHeight(15);
  this->progress_bar_->setVisible(false);
  this->addPermanentWidget(progress_bar_.get());
  this->addWidget(label_.get());
}

StatusBar::~StatusBar()
{
  
}

void StatusBar::SlotActionDone(const char *str)
{
  label_->setText(str);
  progress_bar_->setVisible(false);
  ShowSuccessAnimation();
}

void StatusBar::SlotChangeLabelColorGreen()
{
  label_->setStyleSheet("color:rgb(192, 255, 62)");
}

void StatusBar::SlotChangeLabelColorRed()
{
  label_->setStyleSheet("color:rgb(255, 69, 0)");
}

void StatusBar::SlotChangeLabelColorWhite()
{
  label_->setStyleSheet("color:white");
}

void StatusBar::ShowSuccessAnimation()
{
  for(count = 0; count < 20; ++count)
  {
    if((count & 1) == 0)
      QTimer::singleShot(count * 100, this, SLOT(SlotChangeLabelColorWhite()));
    else 
      QTimer::singleShot(count * 100, this, SLOT(SlotChangeLabelColorGreen()));
  }
}

void StatusBar::ShowFailAnimation()
{
  for(count = 0; count < 20; ++count)
  {
    if((count & 1) == 0)
      QTimer::singleShot(count * 100, this, SLOT(SlotChangeLabelColorWhite()));
    else 
      QTimer::singleShot(count * 100, this, SLOT(SlotChangeLabelColorGreen()));
  }
}

void StatusBar::SlotProgressRange(int begin, int end)
{
  progress_bar_->setVisible(true);
  progress_bar_->setRange(begin, end);
}

void StatusBar::SlotProgressValue(int value)
{
  progress_bar_->setValue(value);
}



}
}
