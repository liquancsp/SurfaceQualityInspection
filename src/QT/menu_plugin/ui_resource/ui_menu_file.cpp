#include "ui_menu_file.h"
#include <iostream>
namespace Ui
{

ui_menu_file_item::ui_menu_file_item()
{
  std::cout << "[#Info-Init]Init ui menu file item." << std::endl;
}

ui_menu_file_item::~ui_menu_file_item()
{
  
}//end of ~ui_menu_file_item

void  ui_menu_file_item::setup_ui(QMenu *menu_file_item)
{
  if (menu_file_item->objectName().isEmpty())
  {
    menu_file_item->setObjectName(QString::fromUtf8("menu_file_item"));
  }
  open_action_ = new QAction(menu_file_item);
  open_action_->setObjectName(QString::fromUtf8("open_action_"));
  
  new_action_ = new QAction(menu_file_item);
  new_action_->setObjectName(QString::fromUtf8("new_action_"));
  
  import_training_label_action_ = new QAction(menu_file_item);
  import_training_label_action_->setObjectName(QString::fromUtf8("import_training_label_action_"));
  
  export_training_label_action_ = new QAction(menu_file_item);
  import_training_label_action_->setObjectName(QString::fromUtf8("import_training_label_action_"));
    
  export_action_ = new QAction(menu_file_item);
  export_action_->setObjectName(QString::fromUtf8("export_action_"));
  
  import_action_ = new QAction(menu_file_item);
  import_action_->setObjectName(QString::fromUtf8("import_action_"));
  
  exit_action_ = new QAction(menu_file_item);
  exit_action_->setObjectName(QString::fromUtf8("exit_action_"));
  
  menu_file_item->addAction(open_action_);
  menu_file_item->addAction(new_action_);
  menu_file_item->addSeparator();
  menu_file_item->addAction(import_training_label_action_);
  menu_file_item->addAction(export_training_label_action_);
  menu_file_item->addAction(export_action_);
  menu_file_item->addAction(import_action_);
  menu_file_item->addSeparator();
  menu_file_item->addAction(exit_action_);
  retranslate_ui(menu_file_item);

  QMetaObject::connectSlotsByName(menu_file_item);
}// end of setuo_ui

void  ui_menu_file_item::retranslate_ui(QMenu *menu_file_item)
{
  menu_file_item->setTitle(QApplication::translate("menu_file_item", "&File",
                                                   0, QApplication::UnicodeUTF8));
  open_action_->setText(QApplication::translate("menu_file_item", "&Open",
                                               0, QApplication::UnicodeUTF8));
  open_action_->setShortcut(QApplication::translate("menu_file_item", "Ctrl+O",
                                                   0, QApplication::UnicodeUTF8));
  open_action_->setIcon(QIcon("./images/open.png"));
  
  new_action_->setText(QApplication::translate("menu_file_item", "&New",
                                               0, QApplication::UnicodeUTF8));
  new_action_->setShortcut(QApplication::translate("menu_file_item", "Ctrl+N",
                                                   0, QApplication::UnicodeUTF8));
  
  import_training_label_action_->setText(QApplication::translate("menu_file_item", "&Import Training Label",
                                                                 0, QApplication::UnicodeUTF8));
  export_training_label_action_->setText(QApplication::translate("menu_file_item", "&Export Training Label",
                                                                 0, QApplication::UnicodeUTF8));
  
  export_action_->setText(QApplication::translate("menu_file_item", "&Export",
                                                  0, QApplication::UnicodeUTF8));
  
  import_action_->setText(QApplication::translate("menu_file_item", "&Import",
                                                   0, QApplication::UnicodeUTF8));
  
  exit_action_->setText(QApplication::translate("menu_file_item", "&Exit",
                                               0, QApplication::UnicodeUTF8));
  exit_action_->setShortcut(QApplication::translate("menu_file_item", "Ctrl+E",
                                                       0, QApplication::UnicodeUTF8));
}//end  of retranslate_ui

}
