/********************************************************************************
** Form generated from reading UI file 'SmoothDiffInput.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SMOOTHDIFFINPUT_H
#define UI_SMOOTHDIFFINPUT_H

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

class Ui_HeightMapRadiusRatio
{
public:
    QPushButton *ok_btn_action;
    QPushButton *cancel_btn_action;
    QGroupBox *group_box_1;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *smooth_ratio_action_;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout;
    QLabel *label_3;
    QLineEdit *smooth_step_action_;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QLineEdit *smooth_multiplier_action_;

    void setupUi(QDialog *HeightMapRadiusRatio)
    {
        if (HeightMapRadiusRatio->objectName().isEmpty())
            HeightMapRadiusRatio->setObjectName(QString::fromUtf8("HeightMapRadiusRatio"));
        HeightMapRadiusRatio->resize(327, 196);
        ok_btn_action = new QPushButton(HeightMapRadiusRatio);
        ok_btn_action->setObjectName(QString::fromUtf8("ok_btn_action"));
        ok_btn_action->setGeometry(QRect(140, 160, 61, 27));
        cancel_btn_action = new QPushButton(HeightMapRadiusRatio);
        cancel_btn_action->setObjectName(QString::fromUtf8("cancel_btn_action"));
        cancel_btn_action->setGeometry(QRect(230, 160, 61, 27));
        group_box_1 = new QGroupBox(HeightMapRadiusRatio);
        group_box_1->setObjectName(QString::fromUtf8("group_box_1"));
        group_box_1->setGeometry(QRect(30, 20, 277, 109));
        layoutWidget = new QWidget(group_box_1);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(67, 40, 208, 29));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        smooth_ratio_action_ = new QLineEdit(layoutWidget);
        smooth_ratio_action_->setObjectName(QString::fromUtf8("smooth_ratio_action_"));

        horizontalLayout_2->addWidget(smooth_ratio_action_);

        layoutWidget1 = new QWidget(group_box_1);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(30, 80, 246, 29));
        horizontalLayout = new QHBoxLayout(layoutWidget1);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(layoutWidget1);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout->addWidget(label_3);

        smooth_step_action_ = new QLineEdit(layoutWidget1);
        smooth_step_action_->setObjectName(QString::fromUtf8("smooth_step_action_"));

        horizontalLayout->addWidget(smooth_step_action_);

        widget = new QWidget(group_box_1);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(0, 0, 277, 29));
        horizontalLayout_3 = new QHBoxLayout(widget);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_3->addWidget(label_2);

        smooth_multiplier_action_ = new QLineEdit(widget);
        smooth_multiplier_action_->setObjectName(QString::fromUtf8("smooth_multiplier_action_"));

        horizontalLayout_3->addWidget(smooth_multiplier_action_);


        retranslateUi(HeightMapRadiusRatio);

        QMetaObject::connectSlotsByName(HeightMapRadiusRatio);
    } // setupUi

    void retranslateUi(QDialog *HeightMapRadiusRatio)
    {
        HeightMapRadiusRatio->setWindowTitle(QApplication::translate("HeightMapRadiusRatio", "Dialog", 0, QApplication::UnicodeUTF8));
        ok_btn_action->setText(QApplication::translate("HeightMapRadiusRatio", "OK", 0, QApplication::UnicodeUTF8));
        cancel_btn_action->setText(QApplication::translate("HeightMapRadiusRatio", "Cancel", 0, QApplication::UnicodeUTF8));
        group_box_1->setTitle(QString());
        label->setText(QApplication::translate("HeightMapRadiusRatio", "Ratios : ", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("HeightMapRadiusRatio", "Smooth Step:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("HeightMapRadiusRatio", "Height Multiplier :", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class HeightMapRadiusRatio: public Ui_HeightMapRadiusRatio {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SMOOTHDIFFINPUT_H
