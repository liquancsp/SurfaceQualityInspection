/********************************************************************************
** Form generated from reading UI file 'LoaclRadiusInput.ui'
**
** Created: Fri Jan 10 09:50:04 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOACLRADIUSINPUT_H
#define UI_LOACLRADIUSINPUT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSplitter>

QT_BEGIN_NAMESPACE

class Ui_AlignRadiusInput
{
public:
    QPushButton *ok_btn_action;
    QPushButton *cancel_btn_action;
    QGroupBox *group_box_1;
    QSplitter *splitter;
    QLabel *label;
    QLineEdit *align_radius_text;

    void setupUi(QDialog *AlignRadiusInput)
    {
        if (AlignRadiusInput->objectName().isEmpty())
            AlignRadiusInput->setObjectName(QString::fromUtf8("AlignRadiusInput"));
        AlignRadiusInput->resize(298, 156);
        ok_btn_action = new QPushButton(AlignRadiusInput);
        ok_btn_action->setObjectName(QString::fromUtf8("ok_btn_action"));
        ok_btn_action->setGeometry(QRect(140, 120, 61, 27));
        cancel_btn_action = new QPushButton(AlignRadiusInput);
        cancel_btn_action->setObjectName(QString::fromUtf8("cancel_btn_action"));
        cancel_btn_action->setGeometry(QRect(220, 120, 61, 27));
        group_box_1 = new QGroupBox(AlignRadiusInput);
        group_box_1->setObjectName(QString::fromUtf8("group_box_1"));
        group_box_1->setGeometry(QRect(10, 30, 271, 81));
        splitter = new QSplitter(group_box_1);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setGeometry(QRect(20, 40, 234, 20));
        splitter->setOrientation(Qt::Horizontal);
        label = new QLabel(splitter);
        label->setObjectName(QString::fromUtf8("label"));
        splitter->addWidget(label);
        align_radius_text = new QLineEdit(splitter);
        align_radius_text->setObjectName(QString::fromUtf8("align_radius_text"));
        splitter->addWidget(align_radius_text);
        label->raise();
        align_radius_text->raise();
        align_radius_text->raise();

        retranslateUi(AlignRadiusInput);

        QMetaObject::connectSlotsByName(AlignRadiusInput);
    } // setupUi

    void retranslateUi(QDialog *AlignRadiusInput)
    {
        AlignRadiusInput->setWindowTitle(QApplication::translate("AlignRadiusInput", "Dialog", 0, QApplication::UnicodeUTF8));
        ok_btn_action->setText(QApplication::translate("AlignRadiusInput", "OK", 0, QApplication::UnicodeUTF8));
        cancel_btn_action->setText(QApplication::translate("AlignRadiusInput", "Cancel", 0, QApplication::UnicodeUTF8));
        group_box_1->setTitle(QApplication::translate("AlignRadiusInput", "Radius", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("AlignRadiusInput", "Align Radius:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AlignRadiusInput: public Ui_AlignRadiusInput {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOACLRADIUSINPUT_H
