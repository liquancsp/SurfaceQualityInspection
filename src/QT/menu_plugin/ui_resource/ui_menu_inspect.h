#pragma once
#ifndef UI_MENU_INSPECT_H
#define UI_MENU_INSPECT_H

#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QMenu>
QT_BEGIN_NAMESPACE
namespace Ui
{

class ui_menu_inspect_item
{
public:
   
    ui_menu_inspect_item();
    ~ui_menu_inspect_item();
    QAction* const AlgorithmModel()             const   {return algorithm_model_action_;}
    QAction* const ReflectionMapAction()        const   {return reflection_map_action_;}
    QAction* const IntegrateReflectionAction()  const   {return integrate_reflection_map_action_;}
    QAction* const GlobalAlignInteMapAction()   const   {return global_align_inte_map_action_;}
    QAction* const GlobalAlignNormalComAction() const   {return global_align_normal_component_action_;} 
    QAction* const LocalAlignIntegralReflAction()     const  {return local_align_integral_map_action_;}
    QAction* const LocalAlignNormalDivisionAction()   const  {return local_align_normal_division_action_;}
    QAction* const LeastSquareIntegralMapAction()     const  {return least_square_integral_map_action_;}
    QAction* const LeastSquareNormalComponentAction() const  {return least_square_normal_component_action_;}
    QAction* const ConvexDiffAction()           const   {return convex_diff_action_;}
    QAction* const MixReflConvexAction()        const   {return mix_refl_convex_action_;}
    QAction* const MixInteConvexAction()        const   {return mix_inte_convex_action_;}
    QAction* const MeanCurvatureAction()        const   {return mean_curvature_action_;}
    QAction* const GaussCurvatureAction()       const   {return gaussian_curvature_action_;}
    QAction* const PrincipalK1Action()          const   {return principal_curvature_k1_action_;}
    QAction* const PrincipalK2Action()          const   {return principal_curvature_k2_action_;}
    QAction* const NormalVectorAngleAction()    const   {return normal_vector_angle_action_;}
    QAction* const DistanceDiffAction()         const   {return distance_different_action_;}
    QAction* const MixMeanInteAction()          const   {return mix_mean_inte_action_;}
    QAction* const MixGaussInteAction()         const   {return mix_gauss_inte_action_;}
    QAction* const MixK1InteAction()            const   {return mix_k1_inte_action_;}
    QAction* const MixK2InteAction()            const   {return mix_k2_inte_action_;}
    QAction* const DefectFittingAction()        const   {return defect_fitting_action_;}
    QAction* const PropagationAction()          const   {return propagate_action_;} 
    QAction* const CalculateZernikerCoeffAction() const {return cal_zerniker_coeff_action_;}
//    QAction* const ZernikerClassiryAction()     const   {return zerniker_classify_action_;}
    QActionGroup* const InspectActionGroup()    const   {return inspect_action_group_;}
    
    void SetActionEnable(bool flag, QAction* const action)  
    {action->setEnabled(flag);}
    void SetInspectActionEable(bool flag)
    {
      SetActionEnable(flag, reflection_map_action_);
      SetActionEnable(flag, integrate_reflection_map_action_);
      SetActionEnable(flag, global_align_inte_map_action_);
      SetActionEnable(flag, global_align_normal_component_action_);
      SetActionEnable(flag, local_align_integral_map_action_);
      SetActionEnable(flag, local_align_normal_division_action_);
      SetActionEnable(flag, least_square_integral_map_action_);
      SetActionEnable(flag, least_square_normal_component_action_);
      SetActionEnable(flag, mix_refl_convex_action_);
      SetActionEnable(flag, mix_inte_convex_action_);
      SetActionEnable(flag, normal_vector_angle_action_);
      SetActionEnable(flag, distance_different_action_);
      SetActionEnable(flag, mix_mean_inte_action_);
      SetActionEnable(flag, mix_gauss_inte_action_);
      SetActionEnable(flag, mix_k1_inte_action_);
      SetActionEnable(flag, mix_k2_inte_action_);
    }
    
    void SetSinglePropertyActionEnable(bool flag)
    {
      SetActionEnable(flag, convex_diff_action_);
      SetActionEnable(flag, mean_curvature_action_);
      SetActionEnable(flag, gaussian_curvature_action_);
      SetActionEnable(flag, principal_curvature_k1_action_);
      SetActionEnable(flag, principal_curvature_k2_action_);
      SetActionEnable(flag, defect_fitting_action_);
      SetActionEnable(flag, propagate_action_);
      SetActionEnable(flag, cal_zerniker_coeff_action_);
//      SetActionEnable(flag, zerniker_classify_action_);
    }

    void setup_ui(QMenu *menu_inspect_item);
    void ResetInspectCheckable()
    {
      QAction* tmp = inspect_action_group_->checkedAction();
      if(tmp != NULL)
        tmp->setChecked(false);
    }
    
    
protected:
    QAction *algorithm_model_action_;
    QAction *reflection_map_action_;
    QAction *integrate_reflection_map_action_;
    QAction *global_align_inte_map_action_;
    QAction *global_align_normal_component_action_;
    QAction *local_align_integral_map_action_;
    QAction *local_align_normal_division_action_;
    QAction *least_square_integral_map_action_;
    QAction *least_square_normal_component_action_;
    QAction *convex_diff_action_;
    QAction *mix_refl_convex_action_;
    QAction *mix_inte_convex_action_;
    QAction *mean_curvature_action_;
    QAction *gaussian_curvature_action_;
    QAction *principal_curvature_k1_action_;
    QAction *principal_curvature_k2_action_;
    QAction *normal_vector_angle_action_;
    QAction *distance_different_action_;
    QAction *mix_mean_inte_action_;
    QAction *mix_gauss_inte_action_;
    QAction *mix_k1_inte_action_;
    QAction *mix_k2_inte_action_;
    QAction *defect_fitting_action_;
    QAction *propagate_action_;
    QAction *cal_zerniker_coeff_action_;
//    QAction *zerniker_classify_action_;
    QActionGroup* inspect_action_group_;
    
    QMenu *global_align_menu_;
    QMenu *local_align_menu_;
    QMenu *least_square_menu_;
    
private:
    void NewAction(const std::string& obj_name,
                   QActionGroup* action_group, bool checkable,
                   bool enable, QAction* action);
    void retranslate_ui(QMenu *menu_inspect_item);
};

}// namespace Ui
QT_END_NAMESPACE
#endif // UI_MENU_INSPECT_H
