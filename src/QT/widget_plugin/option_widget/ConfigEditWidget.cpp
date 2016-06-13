#include <widget_plugin/option_widget/ConfigEditWidget.h>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QGraphicsScene>
#include <QColorDialog>
#include <QFrame>
#include <QMouseEvent>

#include <iostream>
namespace sqi 
{
namespace WidgetAction {

ColorEdit::ColorEdit(QRgb initialColor, int id)
  : color_(initialColor), id_(id)
{
  QHBoxLayout *layout = new QHBoxLayout;
  setLayout(layout);
  layout->setContentsMargins(0, 0, 0, 0);
  
  line_edit_ = new QLineEdit(QString::number(color_, 16));
  layout->addWidget(line_edit_);
  
  button_ = new QFrame;
  QPalette palette = button_->palette();
  palette.setColor(QPalette::Window, QColor(color_));
  button_->setPalette(palette);
  button_->setAutoFillBackground(true);
  button_->setMinimumSize(32, 0);
  button_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
  button_->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
  layout->addWidget(button_);
  
  connect(line_edit_, SIGNAL(editingFinished()), this, SLOT(SlotEditDone()));
}

void ColorEdit::SlotEditDone()
{
  bool ok;
  QRgb new_color = line_edit_->text().toUInt(&ok, 16);
  if(ok)
    SetColor(new_color);
}

void ColorEdit::SetColor(QRgb color)
{
  color_ = color;
  line_edit_->setText(QString::number(color_, 16)); // "Clean up" text
  QPalette palette = button_->palette();
  palette.setColor(QPalette::Window, QColor(color_));
  button_->setPalette(palette);
  emit SignalColorChanged(color_, id_);
}

void ColorEdit::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton) 
  {
    QColor color(color_);
    QColorDialog dialog(color, 0);
    dialog.setOption(QColorDialog::ShowAlphaChannel, true);
    // The ifdef block is a workaround for the beta, TODO: remove when bug 238525 is fixed
#ifdef Q_WS_MAC
    dialog.setOption(QColorDialog::DontUseNativeDialog, true);
#endif
    dialog.move(280, 120);
    if (dialog.exec() == QDialog::Rejected)
      return;
    QRgb new_color = dialog.selectedColor().rgba();
    if (new_color == color_)
      return;
    SetColor(new_color);
  }
}

}
}
