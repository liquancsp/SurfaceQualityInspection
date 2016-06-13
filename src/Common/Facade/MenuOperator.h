#ifndef MENU_OPERATOR_H
#define MENU_OPERATOR_H

#include <MeshData.h>
#include <vtkSmartPointer.h>

namespace lq{
class vtk_model_data;
class FileMenuOperate;
class EditMenuOperate;
class ViewMenuOperate;

}

namespace sqi {
namespace common {

class MenuOperator {
  
public :
  MenuOperator(const sqi::meshdata::MeshDataPtr mesh, 
               const vtkSmartPointer<lq::vtk_model_data>& data);
  
  
  void LoadModelFile(const std::string& file_name, lq::WindowFlag win);
  
  void SetRenderData(lq::WindowFlag win);
  
  void CreateVtkRender(lq::WindowFlag win);
  
  void SetSelectClassifyColor(lq::WindowFlag win);
  
  void CreateScene(const std::string& file_name, lq::WindowFlag win);
  
  
  lq::ReturnValue UpdateColorScalar(meshdata::ColorScalarType scalar_type,
                                    double max_scalar, double min_scalar);
 
  void UpdateColorScalar(sqi::meshdata::ColorScalarType scalar_type,
                         double max_scalar, double min_scalar, lq::WindowFlag win);
  
  void UpdateClassifyColor(lq::WindowFlag win = lq::kLeft);
  
  void InitParamters();
  
  void ComputeError(const lq::InspectFlag flag,
                    const lq::AlgType type = lq::kCalTwoDiff,
                    const lq::WindowFlag win = lq::kRight);
  
  void ComputeDescriptor(const lq::InspectFlag flag, 
                         const lq::WindowFlag win = lq::kLeft);
  
  void InitColor();
  
  void InitClassifyColor();
  
  void GenerateMapColor(lq::WindowFlag win);
  
  void GenerateClassifyMapColor(lq::WindowFlag win);
  
  void MapColorOnModel(lq::WindowFlag win);
  
  void MapColorOnVertex(lq::WindowFlag win);
  
  void SetParameters(lq::WindowFlag win);
  
  void UpdateColorLegend(lq::WindowFlag win = lq::kRight);
  
  void UpdateClassifyColorLengend(lq::WindowFlag win = lq::kLeft);
  
  void CleanPolyColor(lq::WindowFlag win = lq::kRight);
  
  void ResetLightParameter(lq::WindowFlag win);
  
  void ResizeColorLegend();
  void ResizeColorLegend(lq::WindowFlag win);
  
  
  void UpdatePolyColor(lq::WindowFlag win);
  
  void FlipNormalOperator(lq::WindowFlag win);
  
  void ClearSceneColorItem(lq::WindowFlag win);
  
private:
  
  MenuOperator(){}
  sqi::meshdata::MeshDataPtr mesh_data_;
  vtkSmartPointer<lq::vtk_model_data> data_set_;
  std::tr1::shared_ptr<lq::FileMenuOperate> file_operator_;
  std::tr1::shared_ptr<lq::EditMenuOperate> edit_operator_;
  std::tr1::shared_ptr<lq::ViewMenuOperate> view_operator_;
  
};

}

}

#endif
