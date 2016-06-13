/*************************************************************************************
 *
 *COPYRIGHT NOTICE
 *Copyright (c) 2013, Zhe Jiang University
 *All rights reserved.
 *
 *@file edit_menu_operate
 *@author li quan
 *
 *version 2.0
 *data 2013-3-4
 *ingroup VTK/menu_operate
 *
 *@brief this file is to support the operate of menubar->View item. It include &Show Border Panel,
 *&Add Color Legend
 *
 *************************************************************************************/
#ifndef VIEW_MENU_OPERATE_H
#define VIEW_MENU_OPERATE_H

#include <vtkSmartPointer.h>

#include <MeshData.h>

class vtkLegendScaleActor;
class vtkScalarBarActor;
class vtkLookupTable;

namespace lq
{

class vtk_model_data;
    
  /***************************************************************************************
   *@class ViewMenuOperate
   *@brief  this class is to support the operate of menubar->View item. &Show Border Panel,
   *&Add Color Legend
   *
   *@detail
   *
   *@data 2013-1-14
   *@version 2.0
   ****************************************************************************************/
  class ViewMenuOperate
  {
    
 public:

    ViewMenuOperate();
    ~ViewMenuOperate();
    void SetDataSet(const vtkSmartPointer<vtk_model_data> &other_data_set);
    void SetMeshData(const sqi::meshdata::MeshDataPtr& other)
    {
      this->mesh_data_ = other;
    }
    void AddColorLegend(lq::WindowFlag win = lq::kRight);
    void AddClassifyColorLengend(lq::WindowFlag win = lq::kLeft);
    void ClearColorLegend(lq::WindowFlag win = lq::kRight);
    void ResizeColorLegend(lq::WindowFlag win = lq::kRight);
    void LinkCamera();
    void ResetScene();
    void ShowBackSide();
    void FlipModelNormal(lq::WindowFlag flag);
 private:

    vtkSmartPointer<vtk_model_data> data_set_;
    sqi::meshdata::MeshDataPtr mesh_data_;
    vtkSmartPointer<vtkLegendScaleActor> legend_scale_actor_;
    vtkSmartPointer<vtkLookupTable> lut_;
    vtkSmartPointer<vtkScalarBarActor> scalar_bar_;
    
  };

}


#endif
