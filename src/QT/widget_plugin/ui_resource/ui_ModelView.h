/********************************************************************************
** Form generated from reading UI file 'ModelView.ui'
**
** Created: Sun Nov 25 16:57:14 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODELVIEW_H
#define UI_MODELVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#include <QVTKWidget.h>
#include <widget_plugin/ui_action/QtVtkWidget.h>

QT_BEGIN_NAMESPACE

class Ui_ModelView
{
public:
    QAction *actionOpenFile;
    QAction *actionExit;
    QAction *actionPrint;
    QAction *actionHelp;
    QAction *actionSave;
    QAction *edit_action;
    QAction *actionMain_Panel;
    QAction *about_action;
    QAction *open_action;
    QAction *action_Save_Ctrl_S;
    QAction *exit_action;
    QAction *inspection_action;
    QAction *color_action;
    QAction *clear_color_action;
    QAction *border_panel_action;
    QAction *hide_border_action;
    QAction *change_view_point_action;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    //QVTKWidget *qvtk_widget_left;
    sqi::ui::QtVtkWidget *qvtk_widget_left;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menu_edit;
    QMenu *menu_View;
    QToolBar *toolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ModelView)
    {
        if (ModelView->objectName().isEmpty())
            ModelView->setObjectName(QString::fromUtf8("ModelView"));
        ModelView->resize(883, 672);
        QIcon icon;
        icon.addFile(QString::fromUtf8("./images/inspect.png"), QSize(), QIcon::Normal, QIcon::On);
        ModelView->setWindowIcon(icon);
        ModelView->setIconSize(QSize(22, 22));
        actionOpenFile = new QAction(ModelView);
        actionOpenFile->setObjectName(QString::fromUtf8("actionOpenFile"));
        actionOpenFile->setEnabled(true);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/Icons/fileopen.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpenFile->setIcon(icon1);
        actionExit = new QAction(ModelView);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(""), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon2);
        actionPrint = new QAction(ModelView);
        actionPrint->setObjectName(QString::fromUtf8("actionPrint"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/Icons/print.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPrint->setIcon(icon3);
        actionHelp = new QAction(ModelView);
        actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/Icons/help.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionHelp->setIcon(icon4);
        actionSave = new QAction(ModelView);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/Icons/filesave.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon5);
        edit_action = new QAction(ModelView);
        edit_action->setObjectName(QString::fromUtf8("edit_action"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8("../images/save.png"), QSize(), QIcon::Normal, QIcon::On);
        edit_action->setIcon(icon6);
        edit_action->setIconVisibleInMenu(true);
        actionMain_Panel = new QAction(ModelView);
        actionMain_Panel->setObjectName(QString::fromUtf8("actionMain_Panel"));
        about_action = new QAction(ModelView);
        about_action->setObjectName(QString::fromUtf8("about_action"));
        open_action = new QAction(ModelView);
        open_action->setObjectName(QString::fromUtf8("open_action"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8("../images/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        open_action->setIcon(icon7);
        action_Save_Ctrl_S = new QAction(ModelView);
        action_Save_Ctrl_S->setObjectName(QString::fromUtf8("action_Save_Ctrl_S"));
        exit_action = new QAction(ModelView);
        exit_action->setObjectName(QString::fromUtf8("exit_action"));
        inspection_action = new QAction(ModelView);
        inspection_action->setObjectName(QString::fromUtf8("inspection_action"));
        color_action = new QAction(ModelView);
        color_action->setObjectName(QString::fromUtf8("color_action"));
        clear_color_action = new QAction(ModelView);
        clear_color_action->setObjectName(QString::fromUtf8("clear_color_action"));
        border_panel_action = new QAction(ModelView);
        border_panel_action->setObjectName(QString::fromUtf8("border_panel_action"));
        hide_border_action = new QAction(ModelView);
        hide_border_action->setObjectName(QString::fromUtf8("hide_border_action"));
        change_view_point_action = new QAction(ModelView);
        change_view_point_action->setObjectName(QString::fromUtf8("change_view_point_action"));
        centralwidget = new QWidget(ModelView);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
       
        //qvtk_widget_left = new QVTKWidget(centralwidget);
        qvtk_widget_left = new sqi::ui::QtVtkWidget(centralwidget);
        qvtk_widget_left->setObjectName(QString::fromUtf8("qvtk_widget_left"));
        gridLayout->setContentsMargins(3, 5, 3, 0);
        gridLayout->addWidget(qvtk_widget_left, 0, 0, 1, 1);
        
//        qvtk_widget_right = new QVTKWidget(centralwidget);
//        qvtk_widget_right->setObjectName(QString::fromUtf8("qvtk_widget_right"));

       // gridLayout->addWidget(qvtk_widget_right, 0, 1, 1, 1);

        ModelView->setCentralWidget(centralwidget);
        menuBar = new QMenuBar(ModelView);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 883, 25));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menu_edit = new QMenu(menuBar);
        menu_edit->setObjectName(QString::fromUtf8("menu_edit"));
        menu_View = new QMenu(menuBar);
        menu_View->setObjectName(QString::fromUtf8("menu_View"));
        ModelView->setMenuBar(menuBar);
        toolBar = new QToolBar(ModelView);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        ModelView->addToolBar(Qt::TopToolBarArea, toolBar);
        statusBar = new QStatusBar(ModelView);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        ModelView->setStatusBar(statusBar);

        /* menuBar->addAction(menuFile->menuAction()); */
        /* menuBar->addAction(menu_edit->menuAction()); */
        /* menuBar->addAction(menu_View->menuAction()); */
        /* menuBar->addAction(menuHelp->menuAction()); */
        menuFile->addAction(edit_action);
        menuFile->addAction(open_action);
        menuFile->addAction(action_Save_Ctrl_S);
        menuFile->addSeparator();
        menuFile->addAction(exit_action);
        menuHelp->addAction(about_action);
        menu_edit->addAction(inspection_action);
        menu_edit->addAction(color_action);
        menu_edit->addAction(clear_color_action);
        menu_edit->addAction(change_view_point_action);
        menu_View->addAction(border_panel_action);
        menu_View->addAction(hide_border_action);

        retranslateUi(ModelView);

        QMetaObject::connectSlotsByName(ModelView);
    } // setupUi

    void retranslateUi(QMainWindow *ModelView)
    {
        ModelView->setWindowTitle(QApplication::translate("ModelView", "Surface Quality Inspection", 0, QApplication::UnicodeUTF8));
        actionOpenFile->setText(QApplication::translate("ModelView", "Open File...", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("ModelView", "Exit", 0, QApplication::UnicodeUTF8));
        actionPrint->setText(QApplication::translate("ModelView", "Print", 0, QApplication::UnicodeUTF8));
        actionHelp->setText(QApplication::translate("ModelView", "Help", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("ModelView", "Save", 0, QApplication::UnicodeUTF8));
        edit_action->setText(QApplication::translate("ModelView", "&Edit...        ", 0, QApplication::UnicodeUTF8));
        edit_action->setShortcut(QApplication::translate("ModelView", "Ctrl+E", 0, QApplication::UnicodeUTF8));
        actionMain_Panel->setText(QApplication::translate("ModelView", "Main Panel", 0, QApplication::UnicodeUTF8));
        about_action->setText(QApplication::translate("ModelView", "About", 0, QApplication::UnicodeUTF8));
        open_action->setText(QApplication::translate("ModelView", "&Open...", 0, QApplication::UnicodeUTF8));
        open_action->setShortcut(QApplication::translate("ModelView", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        action_Save_Ctrl_S->setText(QApplication::translate("ModelView", "&Save...", 0, QApplication::UnicodeUTF8));
        action_Save_Ctrl_S->setShortcut(QApplication::translate("ModelView", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        exit_action->setText(QApplication::translate("ModelView", "&Exit...", 0, QApplication::UnicodeUTF8));
        exit_action->setShortcut(QApplication::translate("ModelView", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        inspection_action->setText(QApplication::translate("ModelView", "&Inspection...", 0, QApplication::UnicodeUTF8));
        inspection_action->setShortcut(QApplication::translate("ModelView", "Ctrl+I", 0, QApplication::UnicodeUTF8));
        color_action->setText(QApplication::translate("ModelView", "&Load Color...", 0, QApplication::UnicodeUTF8));
        color_action->setShortcut(QApplication::translate("ModelView", "Ctrl+L", 0, QApplication::UnicodeUTF8));
        clear_color_action->setText(QApplication::translate("ModelView", "&Clear Color...", 0, QApplication::UnicodeUTF8));
        clear_color_action->setShortcut(QApplication::translate("ModelView", "Ctrl+C", 0, QApplication::UnicodeUTF8));
        border_panel_action->setText(QApplication::translate("ModelView", "&Show Border Panel", 0, QApplication::UnicodeUTF8));
        hide_border_action->setText(QApplication::translate("ModelView", "&Hide Border Panel", 0, QApplication::UnicodeUTF8));
        hide_border_action->setShortcut(QApplication::translate("ModelView", "Ctrl+H", 0, QApplication::UnicodeUTF8));
        change_view_point_action->setText(QApplication::translate("ModelView", "&Switch View Point", 0, QApplication::UnicodeUTF8));
        change_view_point_action->setShortcut(QApplication::translate("ModelView", "Ctrl+C", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("ModelView", "&File", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("ModelView", "&Help", 0, QApplication::UnicodeUTF8));
        menu_edit->setTitle(QApplication::translate("ModelView", "&Edit", 0, QApplication::UnicodeUTF8));
        menu_View->setTitle(QApplication::translate("ModelView", "&View", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("ModelView", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ModelView: public Ui_ModelView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODELVIEW_H
