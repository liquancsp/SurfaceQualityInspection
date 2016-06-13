#ifndef UI_MENU_CLASSIFICATION_H
#define UI_MENU_CLASSIFICATION_H

#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QMenu>
QT_BEGIN_NAMESPACE
namespace Ui
{

class ui_menu_classification
{
public:
   
    ui_menu_classification();
    ~ui_menu_classification();
    QAction* const TrainPredictAction()         const   {return train_predict_action_;}
    QAction* const ClassifyAction()             const   {return classify_action_;}
    QActionGroup* const ClassifyActionGroup()   const   {return classify_action_group_;}
    
    void SetActionEnable(bool flag, QAction* const action)  
    {action->setEnabled(flag);}

    void setup_ui(QMenu *menu_classify_item);
    void ResetInspectCheckable()
    {
      QAction* tmp = classify_action_group_->checkedAction();
      if(tmp != NULL)
        tmp->setChecked(false);
    }
    
    
protected:
    QAction *train_predict_action_;
    QAction *classify_action_;
    QActionGroup* classify_action_group_;
    
    
private:
    void NewAction(const std::string& obj_name,
                   QActionGroup* action_group, bool checkable,
                   bool enable, QAction* action);
    void retranslate_ui(QMenu *menu_classify_item);
};

}// namespace Ui
QT_END_NAMESPACE
#endif // UI_MENU_CLASSIFICATION_H
