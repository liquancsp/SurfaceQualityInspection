/********************************************************************************
** Form generated from reading UI file 'menu_file.ui'
**
** Created: Wed Feb 27 19:39:12 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MENU_FILE_H
#define UI_MENU_FILE_H

#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QMenu>

QT_BEGIN_NAMESPACE
namespace Ui
{
  class ui_menu_file_item
  {
  
 public:
  
    ui_menu_file_item();
    ~ui_menu_file_item();
    QAction* const OpenAction()                  const  {return open_action_;}
    QAction* const NewAction()                   const  {return new_action_;}
    QAction* const ImportTrainingLabelAction()   const  {return import_training_label_action_;}
    QAction* const ExportTrainingLabelAction()   const  {return export_training_label_action_;}
    QAction* const ExportAction()                const  {return export_action_;}
    QAction* const ImportAction()                const  {return import_action_;}
    QAction* const ExitAction()                  const  {return exit_action_;}
    void setup_ui(QMenu *menu_file_item);

  protected:
    QAction* open_action_;
    QAction* new_action_; 
    QAction* import_training_label_action_;
    QAction* export_training_label_action_;
    QAction* export_action_;
    QAction* import_action_;
    QAction* exit_action_;
 
    
 private :
    void retranslate_ui(QMenu *menu_file_item);
    
  };
   
}// namespace Ui
QT_END_NAMESPACE

#endif // UI_MENU_FILE_H
