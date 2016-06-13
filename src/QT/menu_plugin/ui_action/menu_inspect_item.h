/*************************************************************************************
 *
 *COPYRIGHT NOTICE
 *Copyright (c) 2013, Zhe Jiang University
 *All rights reserved.
 *
 *@file menu_edititem.h
 *@author li quan
 *
 *version 2.0
 *data 2013-7
 *ingroup QT/menu_plugin/ui_action
 *
 *@brief this file is to support the function of menubar->Inspect item. It include all
 *algoritms to inpsect the different between two model
 *
 *
 *************************************************************************************/
#pragma once
#ifndef MENU_INSPECT_ITEM_H
#define MENU_INSPECT_ITEM_H

#include <menu_plugin/ui_resource/ui_menu_inspect.h>

#include <MeshData.h>
#include <vtkSmartPointer.h>


namespace sqi {
namespace thread{
class InspectAlgThread;
}
}

namespace sqi {
namespace ui{
class HeightMapRadiusRatioWidget;
}

}
namespace lq {

class vtk_model_data;
class EditMenuOperate;
class AlignRadiusInputWidget;


class menu_inspect_item : public QMenu
{
    Q_OBJECT
    
public:
    menu_inspect_item(sqi::meshdata::MeshDataPtr mesh,
                      vtkSmartPointer<lq::vtk_model_data> data, QWidget* PARENT = 0);
    ~menu_inspect_item();
    void set_data_set(const vtkSmartPointer<lq::vtk_model_data> &other_data_set)
    {
      this->data_set_ = other_data_set;
    }
    
    void set_mesh_data(const sqi::meshdata::MeshDataPtr& other)
    {
      this->mesh_data_ = other;
    }
    Ui::ui_menu_inspect_item* GetUiHandle();
    
signals:
    
    void SignalCompleteInspect();
    void SignalProgressRange(int begin, int end);
    void SignalActionDone(const char* str);
    void SignalBeginAlgAction();
    
public slots:
    
    void slot_algorithm_model_action();
    void slot_reflection_map_action();
    void slot_integrate_reflection_map_action();
    void slot_global_align_integral_refl_action();
    void slot_global_align_normal_component_action();
    void slot_local_align_integral_refl_action();
    void slot_local_align_normal_division_action();
    void slot_least_square_integral_refl_action();
    void slot_least_square_normal_component_action();
    void slot_exec_local_align_alg_action(double radius);
    void slot_convex_diff_action();
    void slot_mix_refl_convex_action();
    void slot_mix_inte_convex_action();
    void slot_mean_curvature_action();
    void slot_gaussian_curvature_action();
    void slot_principal_k1_action();
    void slot_principal_k2_action();
    void slot_normal_vector_angle_action();
    void slot_distance_different_action();
    void slot_mix_mean_inte_action();
    void slot_mix_gauss_inte_action();
    void slot_mix_k1_inte_action();
    void slot_mix_k2_inte_action();
    void slot_defect_fitting_action();
    void slot_exec_zerniker_action(double multipiler, double ratio, int step);
    void slot_propagate_action();
    void slot_zerniker_coeff_action();
    void slot_zerniker_classify_action();
    void slot_execute_alg();
    void slot_reset_checkable_action();
    void SlotCompleteAligorithm(lq::InspectFlag flag);
private:
    
    //void inspect_action(const lq::InspectFlag flag);
    void create_action();
    lq::InspectFlag radius_flag_;
    std::tr1::shared_ptr<Ui::ui_menu_inspect_item> inspect_menu_handle_;  
    std::tr1::shared_ptr<AlignRadiusInputWidget> radius_input_widget_handle_;
    std::tr1::shared_ptr<sqi::ui::HeightMapRadiusRatioWidget> ratio_input_widget_handle_;
    sqi::meshdata::MeshDataPtr mesh_data_; 
    vtkSmartPointer<lq::vtk_model_data> data_set_;
    void ExecuteAlg(const lq::InspectFlag alg_type, 
                    const lq::AlgType alg_model);
    std::tr1::shared_ptr<sqi::thread::InspectAlgThread> inspect_thread_ptr_;
    
};



}

#endif // MENU_INSPECT_ITEM_H
