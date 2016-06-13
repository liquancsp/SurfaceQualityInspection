#include "ui_color_scalar_input.h"

namespace Ui
{

//ui_color_scalar_input::~ui_color_scalar_input()
//{
//  delete color_scalar_text;
//  delete ok_btn_action;
//  delete cancel_btn_action;
//  delete label;
//}

void ui_color_scalar_input::setup_ui(QDialog *color_scalar_input)
{
  if (color_scalar_input->objectName().isEmpty())
      color_scalar_input->setObjectName(QString::fromUtf8("color_scalar_input"));
  QIcon icon;
  icon.addFile(QString::fromUtf8("./images/colors.png"), QSize(), 
               QIcon::Normal, QIcon::On);
  color_scalar_input->setWindowIcon(icon);
  color_scalar_input->resize(321, 223);
  color_scalar_input->setMaximumSize(QSize(321, 239));
  color_scalar_input->setMinimumSize(QSize(321, 239));
  group_box_1 = new QGroupBox(color_scalar_input);
  group_box_1->setObjectName(QString::fromUtf8("group_box_1"));
  group_box_1->setGeometry(QRect(10, 20, 293, 169));
  label = new QLabel(group_box_1);
  label->setObjectName(QString::fromUtf8("label"));
  label->setGeometry(QRect(20, 100, 141, 21));
  color_scalar_text = new QLineEdit(group_box_1);
  color_scalar_text->setObjectName(QString::fromUtf8("color_scalar_text"));
  color_scalar_text->setGeometry(QRect(170, 100, 113, 29));
  label_2 = new QLabel(group_box_1);
  label_2->setObjectName(QString::fromUtf8("label_2"));
  label_2->setGeometry(QRect(20, 40, 141, 21));
  min_threshold_text = new QLineEdit(group_box_1);
  min_threshold_text->setObjectName(QString::fromUtf8("min_threshold_text"));
  min_threshold_text->setGeometry(QRect(170, 40, 113, 29));
  widget = new QWidget(color_scalar_input);
  widget->setObjectName(QString::fromUtf8("widget"));
  widget->setGeometry(QRect(150, 190, 158, 25));
  horizontalLayout = new QHBoxLayout(widget);
  horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
  horizontalLayout->setContentsMargins(0, 0, 0, 0);
  ok_btn_action = new QPushButton(widget);
  ok_btn_action->setObjectName(QString::fromUtf8("ok_btn_action"));
  horizontalLayout->addWidget(ok_btn_action);
  cancel_btn_action = new QPushButton(widget);
  cancel_btn_action->setObjectName(QString::fromUtf8("cancel_btn_action"));
  horizontalLayout->addWidget(cancel_btn_action);
  retranslate_ui(color_scalar_input);
  QMetaObject::connectSlotsByName(color_scalar_input);
}

void ui_color_scalar_input::retranslate_ui(QDialog *color_scalar_input)
{
  color_scalar_input->setWindowTitle(QApplication::translate("color_scalar_input", 
                                                             "Color Threshold", 0,
                                                             QApplication::UnicodeUTF8));
  group_box_1->setTitle(QApplication::translate("color_scalar_input", "Color Threshold",
                                                0, QApplication::UnicodeUTF8));
  label->setText(QApplication::translate("color_scalar_input", "Max Error Threshold:", 
                                         0, QApplication::UnicodeUTF8));
  label_2->setText(QApplication::translate("color_scalar_input", "Min Error Threshold:",
                                           0, QApplication::UnicodeUTF8));
  ok_btn_action->setText(QApplication::translate("color_scalar_input", "OK", 0,
                                                 QApplication::UnicodeUTF8));
  cancel_btn_action->setText(QApplication::translate("color_scalar_input", "Cancel", 0, 
                                                     QApplication::UnicodeUTF8));
}
};
