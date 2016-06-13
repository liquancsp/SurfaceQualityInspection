#include <menu_plugin/ui_resource/ui_menu_classification.h>
namespace Ui{

ui_menu_classification::ui_menu_classification() {
  
}


ui_menu_classification::~ui_menu_classification(){
  
}

void ui_menu_classification::setup_ui(QMenu *menu_classify_item)
{
  if(menu_classify_item->objectName().isEmpty())
    menu_classify_item->setObjectName(QString::fromUtf8("menu_classify_item"));
  classify_action_group_ = new QActionGroup(menu_classify_item);
  train_predict_action_ = new QAction(menu_classify_item);
  NewAction("train_predict_action_", classify_action_group_,
            true, true, train_predict_action_);
  classify_action_ = new QAction(menu_classify_item);
  NewAction("classify_action_", classify_action_group_,
            true, true, classify_action_);
  menu_classify_item->addAction(train_predict_action_);
  menu_classify_item->addAction(classify_action_);
  retranslate_ui(menu_classify_item);
  QMetaObject::connectSlotsByName(menu_classify_item);
}

void ui_menu_classification::retranslate_ui(QMenu *menu_classify_item)
{
  menu_classify_item->setTitle(QApplication::translate("menu_classify_item", "&Classification",
                                                       0, QApplication::UnicodeUTF8));
  train_predict_action_->setText(QApplication::translate("menu_classify_item", "&Train and Predict",
                                                         0, QApplication::UnicodeUTF8));
  classify_action_->setText(QApplication::translate("menu_classify_item", "&Defect Classification",
                                                    0, QApplication::UnicodeUTF8));
  
}

void ui_menu_classification::NewAction(const std::string &obj_name,
                                       QActionGroup *action_group, 
                                       bool checkable, bool enable, 
                                       QAction *action)
{
  action->setObjectName(QString::fromUtf8(obj_name.c_str()));
  action->setCheckable(checkable);
  action->setEnabled(enable);
  action->setActionGroup(action_group);
}

}
