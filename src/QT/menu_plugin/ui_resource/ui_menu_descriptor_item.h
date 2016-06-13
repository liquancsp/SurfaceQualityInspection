#ifndef UI_MENU_DESCRITOR_ITEM_H
#define UI_MENU_DESCRITOR_ITEM_H

#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QMenu>
QT_BEGIN_NAMESPACE
namespace Ui{
class ui_menu_descriptor_item{
  
public:
  
  ui_menu_descriptor_item();
  ~ui_menu_descriptor_item();
  
  QAction* const MeanZernikerAction()                       const {return mean_zerniker_action_;}
  QAction* const GaussZernikerAction()                      const {return gauss_zerniker_action_;}
  QAction* const K1ZernikerAction()                         const {return k1_zerniker_action_;}
  QAction* const K2ZernikerAction()                         const {return k2_zerniker_action_;}
  QAction* const CurvatureZernikerAction()                  const {return curvature_zerniker_action_;}
  QAction* const CurvatureDescriptorAction()                const {return curvature_descriptor_action_;}
  QAction* const CurvatureHistogramAction()                 const {return curvature_histogram_action_;}
  QAction* const SmoothCurvatureDiffAction()                const {return smooth_curvature_diff_action_;}
  QAction* const TrainAndPredictAction()                    const {return train_predict_action_;}
  
  QActionGroup*  const DescriptorActionGroup()              const {return descriptor_action_group_;}
  
  void SetActionEnable(bool flag, QAction* const action)  
  {action->setEnabled(flag);}
  void setup_ui(QMenu *menu_descriptor_item);
  void ResetInspectCheckable()
  {
    QAction* tmp = descriptor_action_group_->checkedAction();
    if(tmp != NULL)
      tmp->setChecked(false);
  }
protected:
  QAction *mean_zerniker_action_;
  QAction *gauss_zerniker_action_;
  QAction *k1_zerniker_action_;
  QAction *k2_zerniker_action_;
  QAction *curvature_zerniker_action_;
  QAction *curvature_descriptor_action_;
  QAction *curvature_histogram_action_;
  QAction *smooth_curvature_diff_action_;
  QAction *train_predict_action_;
  QActionGroup *descriptor_action_group_;
  
private:
  void NewAction(const std::string& obj_name,
                 QActionGroup* action_group, bool checkable,
                 bool enable, QAction* action);
  void retranslate_ui(QMenu *menu_descriptor_item);
};

}

#endif // UI_MENU_DESCRITOR_ITEM_H
