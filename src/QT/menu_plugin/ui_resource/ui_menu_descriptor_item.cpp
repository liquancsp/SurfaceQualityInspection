#include <menu_plugin/ui_resource/ui_menu_descriptor_item.h>

namespace Ui {
ui_menu_descriptor_item::ui_menu_descriptor_item()
{
  
}

ui_menu_descriptor_item::~ui_menu_descriptor_item()
{
  
}

void ui_menu_descriptor_item::NewAction(const std::string &obj_name, 
                                        QActionGroup *action_group,
                                        bool checkable, bool enable, 
                                        QAction *action)
{
  action->setObjectName(QString::fromUtf8(obj_name.c_str()));
  action->setCheckable(checkable);
  action->setEnabled(enable);
  action->setActionGroup(action_group);
}

void ui_menu_descriptor_item::setup_ui(QMenu *menu_descriptor_item)
{
  if(menu_descriptor_item->objectName().isEmpty())
    menu_descriptor_item->setObjectName(QString::fromUtf8("menu_descriptor_item"));
  descriptor_action_group_ = new QActionGroup(menu_descriptor_item);
  mean_zerniker_action_ = new QAction(menu_descriptor_item);
  NewAction("mean_zerniker_action_", descriptor_action_group_,
            true, true, mean_zerniker_action_);
  
  gauss_zerniker_action_ = new QAction(menu_descriptor_item);
  NewAction("gauss_zerniker_action_", descriptor_action_group_,
            true, true, gauss_zerniker_action_);
  
  k1_zerniker_action_ = new QAction(menu_descriptor_item);
  NewAction("k1_zerniker_action_", descriptor_action_group_,
            true, true, k1_zerniker_action_);
  
  k2_zerniker_action_ = new QAction(menu_descriptor_item);
  NewAction("k2_zerniker_action_", descriptor_action_group_,
            true, true, k2_zerniker_action_);
  
  curvature_zerniker_action_ = new QAction(menu_descriptor_item);
  NewAction("curvature_zerniker_action_", descriptor_action_group_,
            true, true, curvature_zerniker_action_);
  
  curvature_descriptor_action_ = new QAction(menu_descriptor_item);
  NewAction("curvature_descriptor_action_", descriptor_action_group_,
            true, true, curvature_descriptor_action_);
  
  curvature_histogram_action_ = new QAction(menu_descriptor_item);
  NewAction("curvature_histogram_action_", descriptor_action_group_,
            true, true, curvature_histogram_action_);
  smooth_curvature_diff_action_ = new QAction(menu_descriptor_item);
  NewAction("smooth_curvature_diff_action_", descriptor_action_group_,
            true, true, smooth_curvature_diff_action_);
  
  train_predict_action_ = new QAction(menu_descriptor_item);
  NewAction("train_predict_action_", descriptor_action_group_,
            true, true, train_predict_action_);
  
  menu_descriptor_item->addAction(mean_zerniker_action_);
  menu_descriptor_item->addAction(gauss_zerniker_action_);
  menu_descriptor_item->addAction(k1_zerniker_action_);
  menu_descriptor_item->addAction(k2_zerniker_action_);
  menu_descriptor_item->addAction(curvature_zerniker_action_);
  menu_descriptor_item->addAction(curvature_descriptor_action_);
  menu_descriptor_item->addAction(curvature_histogram_action_);
  menu_descriptor_item->addAction(smooth_curvature_diff_action_);
  menu_descriptor_item->addSeparator();
  menu_descriptor_item->addAction(train_predict_action_);
  retranslate_ui(menu_descriptor_item);
  QMetaObject::connectSlotsByName(menu_descriptor_item);
}

void ui_menu_descriptor_item::retranslate_ui(QMenu *menu_descriptor_item)
{
  menu_descriptor_item->setTitle(QApplication::translate("menu_descriptor_item", "&Descriptor",
                                                         0, QApplication::UnicodeUTF8));
  mean_zerniker_action_->setText(QApplication::translate("mean_zerniker_action_",
                                                         "&MeanCurveZerniker",
                                                         0, QApplication::UnicodeUTF8));
  gauss_zerniker_action_->setText(QApplication::translate("gauss_zerniker_action_",
                                                          "&GaussCurveZerniker",
                                                          0, QApplication::UnicodeUTF8));
  k1_zerniker_action_->setText(QApplication::translate("k1_zerniker_action_",
                                                       "&K1CurveZerniker",
                                                       0, QApplication::UnicodeUTF8));
  k2_zerniker_action_->setText(QApplication::translate("k2_zerniker_action_",
                                                       "&K2CurveZerniker",
                                                       0, QApplication::UnicodeUTF8));
  curvature_zerniker_action_->setText(QApplication::translate("curvature_zerniker_action_",
                                                              "&CurveZerniker",
                                                              0, QApplication::UnicodeUTF8));
  curvature_descriptor_action_->setText(QApplication::translate("curvature_descriptor_action_",
                                                                "&CurvatureDescriptor",
                                                                0, QApplication::UnicodeUTF8));
  curvature_histogram_action_->setText(QApplication::translate("curvature_histogram_action_",
                                                               "&CurvatureHistogram",
                                                               0, QApplication::UnicodeUTF8));
  smooth_curvature_diff_action_->setText(QApplication::translate("smooth_curvature_diff_action_",
                                                                 "&SmoothCurvatureDiff",
                                                                 0, QApplication::UnicodeUTF8));
  train_predict_action_->setText(QApplication::translate("train_predict_action_",
                                                         "&Train", 0,
                                                         QApplication::UnicodeUTF8));
}

}
