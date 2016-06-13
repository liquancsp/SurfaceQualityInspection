/********************************************************************************
** Form generated from reading UI file 'MainPanel.ui'
**
** Created: Wed Oct 17 19:21:33 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINPANEL_H
#define UI_MAINPANEL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainPanel
{
public:
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout;
    QTreeWidget *tree_widget;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer_3;
    QPushButton *delete_btn_action;
    QSpacerItem *horizontalSpacer;
    //QPushButton *apply_btn;
    QSpacerItem *verticalSpacer;
    QSpacerItem *verticalSpacer_4;
    QLabel *label;
    QSpacerItem *horizontalSpacer_2;
    //QSpinBox *std_spin_box;
    //QSlider *std_slider;
    QWidget *widget;
    QGridLayout *gridLayout_2;
    QSpacerItem *verticalSpacer_5;
    // QPushButton *color_legend_btn;

    void setupUi(QDockWidget *MainPanel)
    {
        if (MainPanel->objectName().isEmpty())
            MainPanel->setObjectName(QString::fromUtf8("MainPanel"));
        //if add the code below the left region can just by user as they want
        /* MainPanel->resize(250, 553); */
        /* MainPanel->setFloating(false); */
        /* MainPanel->setFeatures(QDockWidget::NoDockWidgetFeatures); */
        /* MainPanel->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea); */
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout = new QVBoxLayout(dockWidgetContents);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tree_widget = new QTreeWidget(dockWidgetContents);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        tree_widget->setHeaderItem(__qtreewidgetitem);
        tree_widget->setObjectName(QString::fromUtf8("tree_widget"));

        verticalLayout->addWidget(tree_widget);

        scrollArea = new QScrollArea(dockWidgetContents);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 230, 250));
        layoutWidget = new QWidget(scrollAreaWidgetContents);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(9, 0, 212, 173));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        verticalSpacer_2 = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 0, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_3, 0, 5, 1, 1);

        delete_btn_action = new QPushButton(layoutWidget);
        delete_btn_action->setObjectName(QString::fromUtf8("delete_btn_action"));
        delete_btn_action->setMinimumSize(QSize(85, 27));
        delete_btn_action->setMaximumSize(QSize(85, 16777215));

        gridLayout->addWidget(delete_btn_action, 1, 0, 1, 2);

        horizontalSpacer = new QSpacerItem(28, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 2, 1, 2);

        //apply_btn = new QPushButton(layoutWidget);
        //apply_btn->setObjectName(QString::fromUtf8("apply_btn"));

        //gridLayout->addWidget(apply_btn, 1, 4, 1, 2);

        verticalSpacer = new QSpacerItem(20, 48, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 2, 0, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 48, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_4, 2, 5, 1, 1);

        //label = new QLabel(layoutWidget);
        //label->setObjectName(QString::fromUtf8("label"));

        //gridLayout->addWidget(label, 3, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(28, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 3, 1, 1, 2);

        //std_spin_box = new QSpinBox(layoutWidget);
        //std_spin_box->setObjectName(QString::fromUtf8("std_spin_box"));

        //gridLayout->addWidget(std_spin_box, 3, 3, 1, 2);

        //std_slider = new QSlider(layoutWidget);
        //std_slider->setObjectName(QString::fromUtf8("std_slider"));
        //std_slider->setOrientation(Qt::Horizontal);

        //gridLayout->addWidget(std_slider, 4, 0, 1, 6);

        widget = new QWidget(scrollAreaWidgetContents);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(30, 170, 142, 63));
        gridLayout_2 = new QGridLayout(widget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalSpacer_5 = new QSpacerItem(20, 28, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_5, 0, 0, 1, 1);

        /* color_legend_btn = new QPushButton(widget); */
        /* color_legend_btn->setObjectName(QString::fromUtf8("color_legend_btn")); */

        // gridLayout_2->addWidget(color_legend_btn, 1, 0, 1, 1);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);

        MainPanel->setWidget(dockWidgetContents);

        retranslateUi(MainPanel);
        //QObject::connect(std_spin_box, SIGNAL(valueChanged(int)), std_slider, SLOT(setValue(int)));

        QMetaObject::connectSlotsByName(MainPanel);
    } // setupUi

    void retranslateUi(QDockWidget *MainPanel)
    {
        MainPanel->setWindowTitle(QApplication::translate("MainPanel", "contral panel", 0, QApplication::UnicodeUTF8));
        delete_btn_action->setText(QApplication::translate("MainPanel", "Delete", 0, QApplication::UnicodeUTF8));
        //apply_btn->setText(QApplication::translate("MainPanel", "Apply", 0, QApplication::UnicodeUTF8));
        //label->setText(QApplication::translate("MainPanel", "Standard:", 0, QApplication::UnicodeUTF8));
        // color_legend_btn->setText(QApplication::translate("MainPanel", "show color legend", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainPanel: public Ui_MainPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINPANEL_H
