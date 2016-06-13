#include "ui_view_point_input_widget.h"

namespace Ui
{


void ui_view_point_input_widget::setup_ui(QWidget *Form)
{
  if (Form->objectName().isEmpty())
    Form->setObjectName(QString::fromUtf8("Form"));
  Form->setFixedSize(372, 287);
  label = new QLabel(Form);
  label->setObjectName(QString::fromUtf8("label"));
  label->setGeometry(QRect(30, 40, 161, 31));
  btn_ok = new QPushButton(Form);
  btn_ok->setObjectName(QString::fromUtf8("btn_ok"));
  btn_ok->setGeometry(QRect(160, 230, 96, 27));
  btn_reset = new QPushButton(Form);
  btn_reset->setObjectName(QString::fromUtf8("btn_reset"));
  btn_reset->setGeometry(QRect(270, 230, 96, 27));
  btn_reset->setLayoutDirection(Qt::RightToLeft);
  splitter = new QSplitter(Form);
  splitter->setObjectName(QString::fromUtf8("splitter"));
  splitter->setGeometry(QRect(30, 120, 301, 27));
  splitter->setOrientation(Qt::Horizontal);
  label_2 = new QLabel(splitter);
  label_2->setObjectName(QString::fromUtf8("label_2"));
  splitter->addWidget(label_2);
  edit_box_x = new QLineEdit(splitter);
  edit_box_x->setObjectName(QString::fromUtf8("edit_box_x"));
  splitter->addWidget(edit_box_x);
  label_3 = new QLabel(splitter);
  label_3->setObjectName(QString::fromUtf8("label_3"));
  splitter->addWidget(label_3);
  edit_box_y = new QLineEdit(splitter);
  edit_box_y->setObjectName(QString::fromUtf8("edit_box_y"));
  splitter->addWidget(edit_box_y);
  label_4 = new QLabel(splitter);
  label_4->setObjectName(QString::fromUtf8("label_4"));
  splitter->addWidget(label_4);
  edit_box_z = new QLineEdit(splitter);
  edit_box_z->setObjectName(QString::fromUtf8("edit_box_z"));
  splitter->addWidget(edit_box_z);

  retranslate_ui(Form);

  QMetaObject::connectSlotsByName(Form);
}

void ui_view_point_input_widget::retranslate_ui(QWidget *Form)
{
  Form->setWindowTitle(QApplication::translate("View Point Input", "View Point Input",
                                               0, QApplication::UnicodeUTF8));
  label->setText(QApplication::translate("Form", "View Point Coordinate\357\274\232",
                                         0, QApplication::UnicodeUTF8));
  btn_ok->setText(QApplication::translate("Form", "OK", 0, QApplication::UnicodeUTF8));
  btn_reset->setText(QApplication::translate("Form", "Reset", 0, QApplication::UnicodeUTF8));
  label_2->setText(QApplication::translate("Form", "x:", 0, QApplication::UnicodeUTF8));
  label_3->setText(QApplication::translate("Form", "y:", 0, QApplication::UnicodeUTF8));
  label_4->setText(QApplication::translate("Form", "z:", 0, QApplication::UnicodeUTF8));
}

}
