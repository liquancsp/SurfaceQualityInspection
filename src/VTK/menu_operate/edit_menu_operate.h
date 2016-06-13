/*************************************************************************************
 *
 *COPYRIGHT NOTICE
 *Copyright (c) 2013, Zhe Jiang University
 *All rights reserved.
 *
 *@file EditMenuOperate
 *@author li quan
 *
 *version 2.0
 *data 2013-3-3
 *ingroup VTK/menu_operate
 *
 *@brief this file is to support the operate of menubar->Edit item. It include &Inspect,
 *&Load Color, &Clear Color
 *
 *************************************************************************************/
#ifndef EDIT_MENU_OPERATE_H
#define EDIT_MENU_OPERATE_H
#include <data_type.h>
#include <MeshData.h>

#include <vtkSmartPointer.h>

#if defined(_MSC_VER)
#include <memory>
#endif

#if defined(__GNUC__)
#include <tr1/memory>
#endif

namespace sqi{
namespace common{
class AlgorithmStrategy;
class DescriptorStrategy;
}
}
namespace sqi{
namespace alg{
class BaseAlgorithm;
}
}

namespace lq
{

class vtk_model_data;

  /***************************************************************************************
   *@class edit_menu_operate
   *@brief  this class is to support the operate of menubar->Edit item. It include &Inspect,
   *&Load Color, &Clear Color
   *
   *@detail
   *
   *@data 2013-11-4
   *@version 2.0
   ****************************************************************************************/
 
  class EditMenuOperate
  {

 public:
    EditMenuOperate();
    
    ~EditMenuOperate();
    void SetDataSet(const vtkSmartPointer<vtk_model_data> &other_data_set);
    void SetMeshData(const sqi::meshdata::MeshDataPtr& other);
    
    
    void ComputeError(const lq::InspectFlag flag, const lq::AlgType type = lq::kCalTwoDiff, 
                       const lq::WindowFlag win = lq::kRight);
    
    void ComputeDescriptor(const lq::InspectFlag flag, const lq::WindowFlag win = lq::kLeft);
    
    void ComputeSingleProperty(const lq::InspectFlag flag, 
                               const lq::WindowFlag win = lq::kRight);
    int InitColorList();
    int InitClassifyColorList();
    int MapErrorColor(lq::WindowFlag win = lq::kRight);
    int MapClassifyColor(lq::WindowFlag win = lq::kLeft);
    void SetColorOnPoly(lq::WindowFlag win = lq::kRight);
    void SetColorOnVertex(lq::WindowFlag win = lq::kRight);
    void SetSelectClassifyColor(lq::WindowFlag win = lq::kLeft);
    void ClearPolyColor(lq::WindowFlag win = lq::kRight);
    // void display_view_point();
    
 private:
    
    EditMenuOperate(const EditMenuOperate &other);
    EditMenuOperate& operator = (const EditMenuOperate &other);
//    vtkSmartPointer<vtkActor> view_point_actor_;
//    vtkSmartPointer<vtkSphereWidget> view_point_sphere_;
    //void create_sphere(int radius);
   
    vtkSmartPointer<vtk_model_data> data_set_;
    sqi::meshdata::MeshDataPtr mesh_data_;
    std::tr1::shared_ptr<sqi::common::AlgorithmStrategy> alg_strategy_ptr_;
    std::tr1::shared_ptr<sqi::common::DescriptorStrategy> descriptor_strategy_ptr_;
    fstream fout;
    
  };

}//end of namespace

#endif
