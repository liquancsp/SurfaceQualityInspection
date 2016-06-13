/***********************************************************************************************
 *
 *COPYRIGHT NOTICE
 *Copyright (c) 2013, Zhe Jiang University
 *All rights reserved
 *
 *@file vtk_model_data.h
 *@author li quan
 *
 *version 2.0
 *data 2013-7
 *ingroup VTK/menu_operate
 *
 *@brief this file is to manager all the datas of models and the windows handel of QT
 *
 *
 ***********************************************************************************************/
#ifndef VTK_MODEL_DATA_H
#define VTK_MODEL_DATA_H
#include <data_type.h>
#include <widget_plugin/ui_action/QtVtkWidget.h>
#include <vtk_data/VtkPropManage.h>

#include <vtkRenderer.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkScalarBarActor.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkTextActor.h>
#include <vtkTextWidget.h>
#include <vtkSphereWidget.h>
#include <vtkLookupTable.h>
#include <vtkScalarBarWidget.h>
#include <vtkCamera.h>
#include <vtkPolyDataMapper.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkAxesActor.h>
#include <vtkRenderWindow.h>
#include <vtkLight.h>
#include <vtkMatrix4x4.h>
#include <vtkKdTreePointLocator.h>
#include <vtkOBBTree.h>
#include <vtkModifiedBSPTree.h>

#include <vector>


namespace lq {

/********************************************************************************************
   *
   *@class vtk_model_data
   *@ingroup VTK/menu_operate
   *@brief  this file is to manager all the datas of models and the windows handel of QT
   *
   *@detail
   *it public member variable include
   * -render_[2]  Save render of two QVTKWidget windows
   * -vertex_list_[2]  Save the two models vertex information
   * -normal_list_[2] Save two models normal vector information
   * -triangle_[2] Save two models triangle information
   * -QVTKWidget* qvtk_widget_[2] Save the pointer to two QVTKwidget
   * -win_flag_[2] Save the status of two QVTKWidget windows
   *     -false  available
   *     -true   busy
   * -triangle_error Save the error between two model
   * -vertex_error Save ther error between two model
   *
   ********************************************************************************************/

class vtk_model_data : public vtkObjectBase
{
    
public :
    
    static vtk_model_data* New()
    {
        return new vtk_model_data;
    }
    
    static double render_view_port_[2][4];
    
    vtk_model_data();
    ~vtk_model_data();
    void Reset(lq::WindowFlag flag);
    void ResetColorBar(lq::WindowFlag win);
    //Access some flag
    inline bool& IsFirstColorScalar(lq::WindowFlag win = lq::kRight)   {return is_first_set_color_scalar_[win];}
    inline bool& IsHasAxis()                                           {return is_has_axis_;}
    inline bool& IsHasIterStyle()                                      {return is_has_iter_style_;}
    inline bool& IsHasPickIterStyle(lq::WindowFlag win = lq::kRight)   {return is_has_pick_iter_style_[win];} 
    inline bool& IsHasMapAndActor(int flag)                            {return is_has_mapper_and_actor_[flag];}
    inline bool& IsHasSphereWidget()                                   {return is_sphere_widget_create_;}
    inline bool& IsHasViewTextWidget()                                 {return is_view_point_text_widget_create_;}
    inline bool& IsHasScalarBar(lq::WindowFlag win)                    {return is_has_scalar_bar_[win];}
    inline bool& IsBuildKdTree()                                       {return is_build_kdtree_;}
    inline bool& IsBuildOBBTree()                                      {return is_build_obbtree_;}
    inline bool& IsBuildBSPTree()                                      {return is_build_bsptree_;}
    inline bool& AreaPickType()                                        {return area_pick_type_;}
    inline size_t& ScalarType()                                        {return color_scalar_type_;}
    inline lq::InspectFlag& AlgType()                                  {return algorithm_type_;}
    
    inline int* RenderWinSize()                                        {return this->qvtk_->GetRenderWindow()->GetActualSize();}
    inline int& RenderWinX()                                           {return this->qvtk_->GetRenderWindow()->GetActualSize()[0];}
    inline int& RenderWinY()                                           {return this->qvtk_->GetRenderWindow()->GetActualSize()[1];}
    
    inline double& ScalarValue()                                       {return color_scalar_value_;}
    inline std::string& ModelName(int flag)                            {return model_name_[flag];}
    inline point& ViewPoint()                                          {return single_view_point_;}
    
    //Access the VTK data
    inline sqi::ui::QtVtkWidget* &QTW() {return qvtk_;}
    inline vtkSmartPointer<vtkRenderer> &R(int flag)                   {return render_[flag];}
    inline vtkSmartPointer<vtkPolyData>& PD(int flag)                  {return poly_data_[flag];}
    inline vtkSmartPointer<vtkCamera>& CM()                            {return camera_;}
    inline vtkSmartPointer<vtkCamera>& InitialCamera()                 {return init_camera_;}
    inline vtkSmartPointer<vtkPolyDataMapper>& Mapper(int flag)        {return mapper_[flag];}
    inline vtkSmartPointer<vtkActor>& Actor(int flag)                  {return actor_[flag];}
    inline vtkSmartPointer<vtkTextActor>& ViewPosTextActor()           {return view_point_text_actor_;}
    inline vtkSmartPointer<vtkTextActor>& ModelNameTextActor(int flag) {return model_name_text_actor_[flag];}
    inline vtkSmartPointer<vtkAxesActor>& AxesActor()                  {return axes_;}
    inline vtkSmartPointer<vtkScalarBarActor>& ScalarActor(lq::WindowFlag win = lq::kRight)
    {return scalar_bar_[win];}
    inline vtkSmartPointer<vtkLookupTable>& LookupTable(lq::WindowFlag win = lq::kRight)
    {return legend_lut_[win];}
    inline vtkSmartPointer<vtkScalarBarWidget>& ScalarWidget()         {return scalar_bar_widget_;}
    
    inline vtkSmartPointer<vtkOrientationMarkerWidget>& AxesWidget()   {return axes_widget_;}
    inline vtkSmartPointer<vtkTextWidget>& ViewTextWidget()            {return view_point_text_widget_;}
    inline vtkSmartPointer<vtkSphereWidget>& ViewSphereWidget()        {return view_point_sphere_widget_;}
    inline vtkSmartPointer<vtkLight>& LightCtrl(lq::WindowFlag flag)   {return light_[flag];} 
    //inline sqi::VtkData::VtkPropManage* VtkProp()  {return vtk_prop_.get();}
    inline vtkSmartPointer<vtkKdTreePointLocator>& KdTree()            {return kd_tree_;}
    inline vtkSmartPointer<vtkOBBTree>& OBBTree()                      {return obb_tree_;}
    inline vtkSmartPointer<vtkModifiedBSPTree>& BSPTree()              {return bsp_tree_;}
    
    void BuildKdTree(lq::WindowFlag win = lq::kRight);
    void BuildOBBTree(lq::WindowFlag win = lq::kLeft);
    void BuildBSPTree(lq::WindowFlag win = lq::kLeft);
    
    inline int* BackgroundColor()                                      {return sqi::VtkData::VtkPropManage::BackGround();}
    inline int* CellEdgeColor()                                        {return sqi::VtkData::VtkPropManage::CellEdge();}
    inline int* CellColr()                                             {return sqi::VtkData::VtkPropManage::Cell();}
    inline int* ActorColor(sqi::VtkData::ColorType flag)               {return sqi::VtkData::VtkPropManage::ActorColor(flag);}
    inline int* BackFaceActorColor()                                   {return sqi::VtkData::VtkPropManage::BackFaceColor();}
    
    inline double& Diffuse()                                           {return sqi::VtkData::VtkPropManage::DiffuseValue();}
    inline double& Ambient()                                           {return sqi::VtkData::VtkPropManage::AmbientValue();}
    inline double& Specular()                                          {return sqi::VtkData::VtkPropManage::SpecularValue();}
    inline double& SpecularPower()                                     {return sqi::VtkData::VtkPropManage::SpecularPower();}
    inline sqi::VtkData::Light& LightSt()                              {return sqi::VtkData::VtkPropManage::LightStruct();}
    
    
protected:
    
    sqi::ui::QtVtkWidget* qvtk_;
    
    vtkSmartPointer<vtkCamera> camera_;
    vtkSmartPointer<vtkCamera> init_camera_;
    vtkSmartPointer<vtkActor> actor_[2];
    vtkSmartPointer<vtkTextActor> view_point_text_actor_;
    vtkSmartPointer<vtkTextActor> model_name_text_actor_[2];
    vtkSmartPointer<vtkAxesActor> axes_;
    vtkSmartPointer<vtkScalarBarActor> scalar_bar_[2];
    
    vtkSmartPointer<vtkRenderer> render_[2];
    vtkSmartPointer<vtkPolyData> poly_data_[2];
    vtkSmartPointer<vtkPolyDataMapper> mapper_[2];
    vtkSmartPointer<vtkLookupTable> legend_lut_[2];
    vtkSmartPointer<vtkScalarBarWidget> scalar_bar_widget_;
    
    vtkSmartPointer<vtkOrientationMarkerWidget> axes_widget_;
    vtkSmartPointer<vtkTextWidget> view_point_text_widget_;
    vtkSmartPointer<vtkSphereWidget> view_point_sphere_widget_;
    vtkSmartPointer<vtkLight> light_[2];
    
    vtkSmartPointer<vtkKdTreePointLocator> kd_tree_;
    vtkSmartPointer<vtkOBBTree> obb_tree_;
    vtkSmartPointer<vtkModifiedBSPTree> bsp_tree_;
    
    bool is_first_set_color_scalar_[2];
    bool is_has_axis_;
    bool is_has_iter_style_;
    bool is_has_pick_iter_style_[2];
    bool is_has_mapper_and_actor_[2];
    bool is_sphere_widget_create_;
    bool is_view_point_text_widget_create_;
    bool is_has_scalar_bar_[2];
    bool is_build_kdtree_;
    bool is_build_obbtree_;
    bool is_build_bsptree_;
    bool area_pick_type_;
    size_t color_scalar_type_;
    lq::InspectFlag algorithm_type_;
    
    double color_scalar_value_;
    std::string model_name_[2]; 
    point single_view_point_;
    
    //std::tr1::shared_ptr<sqi::VtkData::VtkPropManage> vtk_prop_;
    
private:
    //flag wether it is the first time user set color scalar. default is false 
    vtk_model_data(const vtk_model_data &other);
    vtk_model_data& operator = (const vtk_model_data &other);
    
};


}

#endif
