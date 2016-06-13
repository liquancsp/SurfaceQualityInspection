/********************************************************************************
** Form generated from reading UI file 'view_point_input_widge.ui'
**
** Created: Thu Mar 7 18:50:14 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIEW_POINT_INPUT_WIDGE_H
#define UI_VIEW_POINT_INPUT_WIDGE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSplitter>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
  
  class ui_view_point_input_widget

  {
    
 public:
    QLabel *label;
    QPushButton *btn_ok;
    QPushButton *btn_reset;
    QSplitter *splitter;
    QLabel *label_2;
    QLineEdit *edit_box_x;
    QLabel *label_3;
    QLineEdit *edit_box_y;
    QLabel *label_4;
    QLineEdit *edit_box_z;

    void setup_ui(QWidget *Form);

    void retranslate_ui(QWidget *Form);
  };
}

QT_END_NAMESPACE

#endif // UI_VIEW_POINT_INPUT_WIDGE_H
