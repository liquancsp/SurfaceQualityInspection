/********************************************************************************
** Form generated from reading UI file 'color_scalar_input.ui'
**
** Created: Fri Mar 28 16:58:15 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COLOR_SCALAR_INPUT_H
#define UI_COLOR_SCALAR_INPUT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class ui_color_scalar_input
{
public:
    QGroupBox *group_box_1;
    QLabel *label;
    QLineEdit *color_scalar_text;
    QLabel *label_2;
    QLineEdit *min_threshold_text;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *ok_btn_action;
    QPushButton *cancel_btn_action;

    void setup_ui(QDialog *color_scalar_input);
    

    void retranslate_ui(QDialog *color_scalar_input);
 
};


  //  class color_scalar_input: public Ui_color_scalar_input {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COLOR_SCALAR_INPUT_H
