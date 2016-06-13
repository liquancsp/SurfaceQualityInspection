#include <menu_plugin/ui_resource/ui_menu_inspect.h>
#include <iostream>
namespace Ui {

ui_menu_inspect_item::ui_menu_inspect_item()
{
}

ui_menu_inspect_item::~ui_menu_inspect_item()
{}

void ui_menu_inspect_item::NewAction(const std::string &obj_name, 
                                     QActionGroup *action_group, 
                                     bool checkable, bool enable, QAction* action)
{
  action->setObjectName(QString::fromUtf8(obj_name.c_str()));
  action->setCheckable(checkable);
  action->setEnabled(enable);
  action->setActionGroup(action_group);
}

void ui_menu_inspect_item::setup_ui(QMenu *menu_inspect_item)
{
  if(menu_inspect_item->objectName().isEmpty())
  {
    menu_inspect_item->setObjectName(QString::fromUtf8("menu_insect_item"));
  }
  //&inspect
  
  inspect_action_group_ = new QActionGroup(menu_inspect_item);
  algorithm_model_action_ = new QAction(menu_inspect_item);
  algorithm_model_action_->setObjectName(QString::fromUtf8("algorithm_model_action_"));
  algorithm_model_action_->setCheckable(true);
  
  reflection_map_action_ = new QAction(menu_inspect_item);
  NewAction("reflection_map_action_", inspect_action_group_,
            true, false, reflection_map_action_);

  integrate_reflection_map_action_ = new QAction(menu_inspect_item);
  NewAction("integrate_reflection_map_action_", 
            inspect_action_group_, true, false, integrate_reflection_map_action_);

  global_align_inte_map_action_ = new QAction(menu_inspect_item);
  NewAction("global_align_inte_map_action_", inspect_action_group_, 
            true, false, global_align_inte_map_action_);
  
  global_align_normal_component_action_ = new QAction(menu_inspect_item);
  NewAction("global_align_normal_component_action_", inspect_action_group_,
            true, false, global_align_normal_component_action_);
  
  local_align_integral_map_action_ = new QAction(menu_inspect_item);
  NewAction("local_align_integral_map_action_",
            inspect_action_group_, true, false, local_align_integral_map_action_);
  
  local_align_normal_division_action_ = new QAction(menu_inspect_item);
  NewAction("local_align_normal_division_action_", inspect_action_group_,
            true, false, local_align_normal_division_action_);
  
  least_square_integral_map_action_ = new QAction(menu_inspect_item);
  NewAction("least_square_integral_map_action_", inspect_action_group_,
            true, false, least_square_integral_map_action_);
  
  least_square_normal_component_action_ = new QAction(menu_inspect_item);
  NewAction("least_square_normal_component_action_", inspect_action_group_,
            true, false, least_square_normal_component_action_);
  
  convex_diff_action_ = new QAction(menu_inspect_item);
  NewAction("convex_diff_action_", inspect_action_group_,
            true, true, convex_diff_action_);
  
  mix_refl_convex_action_ = new QAction(menu_inspect_item);
  NewAction("mix_refl_convex_action_", inspect_action_group_,
            true, false, mix_refl_convex_action_);

  mix_inte_convex_action_ = new QAction(menu_inspect_item);
  NewAction("mix_inte_convex_action_", inspect_action_group_,
            true, false, mix_inte_convex_action_);
  
  mean_curvature_action_ = new QAction(menu_inspect_item);
  NewAction("mean_curvature_action_", inspect_action_group_,
            true, true, mean_curvature_action_);
  
  gaussian_curvature_action_ = new QAction(menu_inspect_item);
  NewAction("gaussian_curvature_action_", inspect_action_group_,
            true, true, gaussian_curvature_action_);
  
  principal_curvature_k1_action_ = new QAction(menu_inspect_item);
  NewAction("principal_curvatrue_k1_action_", inspect_action_group_,
            true, true, principal_curvature_k1_action_);
  
  principal_curvature_k2_action_ = new QAction(menu_inspect_item);
  NewAction("principal_curvature_k2_action_", inspect_action_group_,
            true, true, principal_curvature_k2_action_);
  
  normal_vector_angle_action_ = new QAction(menu_inspect_item);
  NewAction("normal_vector_angle_action_", inspect_action_group_,
            true, false, normal_vector_angle_action_);
  
  distance_different_action_ = new QAction(menu_inspect_item);
  NewAction("distance_different_action_", inspect_action_group_,
            true, false, distance_different_action_);
  
  mix_mean_inte_action_ = new QAction(menu_inspect_item);
  NewAction("mix_mean_inte_action_", inspect_action_group_,
            true, false, mix_mean_inte_action_);
  
  mix_gauss_inte_action_ = new QAction(menu_inspect_item);
  NewAction("mix_gauss_inte_action_", inspect_action_group_,
            true, false, mix_gauss_inte_action_);

  mix_k1_inte_action_ = new QAction(menu_inspect_item);
  NewAction("mix_k1_inte_action_", inspect_action_group_,
            true, false, mix_k1_inte_action_);

  mix_k2_inte_action_ = new QAction(menu_inspect_item);
  NewAction("mix_k2_inte_action_", inspect_action_group_,
            true, false, mix_k2_inte_action_);
  
  defect_fitting_action_ = new QAction(menu_inspect_item);
  NewAction("defect_fitting_action_", inspect_action_group_,
            true, true, defect_fitting_action_);
  
  propagate_action_ = new QAction(menu_inspect_item);
  NewAction("propagate_action_", inspect_action_group_,
            true, true, propagate_action_);
  
  cal_zerniker_coeff_action_ = new QAction(menu_inspect_item);
  NewAction("cal_zerniker_coeff_action_", inspect_action_group_,
            true, true, cal_zerniker_coeff_action_);
  
//  zerniker_classify_action_ = new QAction(menu_inspect_item);
//  NewAction("zerniker_classify_action_", inspect_action_group_,
//            true, true, zerniker_classify_action_);
  
  menu_inspect_item->addAction(algorithm_model_action_);
  menu_inspect_item->addSeparator();
  menu_inspect_item->addAction(reflection_map_action_);
  menu_inspect_item->addAction(integrate_reflection_map_action_);
  
  global_align_menu_ = menu_inspect_item->addMenu("Global Align");  
  global_align_menu_->addAction(global_align_inte_map_action_);
  global_align_menu_->addAction(global_align_normal_component_action_);
  local_align_menu_ = menu_inspect_item->addMenu("Local Align");  
  local_align_menu_->addAction(local_align_integral_map_action_);
  local_align_menu_->addAction(local_align_normal_division_action_);
  least_square_menu_ = menu_inspect_item->addMenu("Least Square");
  least_square_menu_->addAction(least_square_integral_map_action_);
  least_square_menu_->addAction(least_square_normal_component_action_);
  menu_inspect_item->addSeparator();
  menu_inspect_item->addAction(convex_diff_action_);
  menu_inspect_item->addAction(mix_refl_convex_action_);
  menu_inspect_item->addAction(mix_inte_convex_action_);
  menu_inspect_item->addSeparator();
  menu_inspect_item->addAction(mean_curvature_action_);
  menu_inspect_item->addAction(gaussian_curvature_action_);
  menu_inspect_item->addAction(principal_curvature_k1_action_);
  menu_inspect_item->addAction(principal_curvature_k2_action_);
  menu_inspect_item->addSeparator();
  menu_inspect_item->addAction(normal_vector_angle_action_);
  menu_inspect_item->addAction(distance_different_action_);
  menu_inspect_item->addSeparator();
  menu_inspect_item->addAction(mix_mean_inte_action_);
  menu_inspect_item->addAction(mix_gauss_inte_action_);
  menu_inspect_item->addAction(mix_k1_inte_action_);
  menu_inspect_item->addAction(mix_k2_inte_action_);
  menu_inspect_item->addSeparator();
  menu_inspect_item->addAction(defect_fitting_action_);
  menu_inspect_item->addSeparator();
  menu_inspect_item->addAction(propagate_action_);
  menu_inspect_item->addAction(cal_zerniker_coeff_action_);
//  menu_inspect_item->addAction(zerniker_classify_action_);
  retranslate_ui(menu_inspect_item);
  QMetaObject::connectSlotsByName(menu_inspect_item);
}

void ui_menu_inspect_item::retranslate_ui(QMenu *menu_inspect_item)
{
  menu_inspect_item->setTitle(QApplication::translate("menu_inspect_item", "&Inspect",
                                                      0, QApplication::UnicodeUTF8));
  algorithm_model_action_->setText(QApplication::translate("algorithm_model_action_",
                                                           "&Algorithm Model", 0,
                                                           QApplication::UnicodeUTF8));
  
  reflection_map_action_->setText(QApplication::translate("reflection_map_action_",
                                                          "&Reflection map", 0,
                                                          QApplication::UnicodeUTF8));
  integrate_reflection_map_action_->setText(QApplication::translate("integrate_reflection_map_action_",
                                                                    "&Integrate Reflection", 0,
                                                                    QApplication::UnicodeUTF8));
  global_align_inte_map_action_->setText(QApplication::translate("global_align_inte_map_action_",
                                                                 "&Global Align Integral", 0,
                                                                 QApplication::UnicodeUTF8));
  
  global_align_normal_component_action_->setText(QApplication::translate("global_align_normal_component_action_",
                                                                         "&Global Align Normal Component", 0,
                                                                         QApplication::UnicodeUTF8));
  
  local_align_integral_map_action_->setText(QApplication::translate("local_align_integral_map_action_",
                                                                    "&Local Align Integral", 0,
                                                                    QApplication::UnicodeUTF8));
  
  local_align_normal_division_action_->setText(QApplication::translate("local_align_normal_division_action_",
                                                                       "&Local Align Normal Component", 0,
                                                                       QApplication::UnicodeUTF8));
  
  least_square_integral_map_action_->setText(QApplication::translate("least_square_integral_map_action_",
                                                                     "&Integral Reflection", 0,
                                                                     QApplication::UnicodeUTF8));
  
  least_square_normal_component_action_->setText(QApplication::translate("least_square_normal_component_action_",
                                                                         "&Normal Component", 0,
                                                                         QApplication::UnicodeUTF8));
  
  convex_diff_action_->setText(QApplication::translate("convex_diff_action_",
                                                       "&Convex Different", 0,
                                                       QApplication::UnicodeUTF8));
  
  mix_refl_convex_action_->setText(QApplication::translate("mix_refl_convex_action_",
                                                           "&Reflection X Convex", 0,
                                                           QApplication::UnicodeUTF8));
  
  mix_inte_convex_action_->setText(QApplication::translate("mix_inte_convex_action_",
                                                            "Integral X Covex", 0,
                                                            QApplication::UnicodeUTF8));
  
  mean_curvature_action_->setText(QApplication::translate("mean_curvature_action", 
                                                          "&Mean Curvature",
                                                          0, QApplication::UnicodeUTF8));
  
  gaussian_curvature_action_->setText(QApplication::translate("gaussian_curvature_action", 
                                                              "&Gaussian Curvature",
                                                              0, QApplication::UnicodeUTF8));
  
  principal_curvature_k1_action_->setText(QApplication::translate("principal_curvature_k1_action_",
                                                                  "&Principal K1",
                                                                  0, QApplication::UnicodeUTF8));
  
  principal_curvature_k2_action_->setText(QApplication::translate("principal_curvature_k2_action_", 
                                                                  "&Principal K2",
                                                                  0, QApplication::UnicodeUTF8));
  
  normal_vector_angle_action_->setText(QApplication::translate("normal_vector_angle_action_", 
                                                               "&Normal Vector Angle",
                                                               0, QApplication::UnicodeUTF8));
  
  distance_different_action_->setText(QApplication::translate("distance_different_action_", 
                                                              "&Distance Different",
                                                              0, QApplication::UnicodeUTF8));
  mix_mean_inte_action_->setText(QApplication::translate("mix_mean_inte_action_",
                                                         "&Mean X Integral",
                                                         0, QApplication::UnicodeUTF8));
  
  mix_gauss_inte_action_->setText(QApplication::translate("mix_gauss_inte_action_",
                                                         "&Gauss X Integral",
                                                         0, QApplication::UnicodeUTF8));
  
  mix_k1_inte_action_->setText(QApplication::translate("mix_k1_inte_action_",
                                                         "&K1 X Integral",
                                                         0, QApplication::UnicodeUTF8));
  
  mix_k2_inte_action_->setText(QApplication::translate("mix_k2_inte_action_",
                                                         "&K2 X Integral",
                                                         0, QApplication::UnicodeUTF8));
  defect_fitting_action_->setText(QApplication::translate("defect_fitting_action_",
                                                          "&Defect Fitting",
                                                          0, QApplication::UnicodeUTF8));
  
  propagate_action_->setText(QApplication::translate("propagate_action_",
                                                     "&Propagation",
                                                     0, QApplication::UnicodeUTF8));
  cal_zerniker_coeff_action_->setText(QApplication::translate("cal_zerniker_coeff_action_",
                                                              "&HeightMapZerniker",
                                                              0, QApplication::UnicodeUTF8));
  
//  zerniker_classify_action_->setText(QApplication::translate("zerniker_classify_action_",
//                                                             "&Zerniker Classify",
//                                                             0, QApplication::UnicodeUTF8));
}

}
