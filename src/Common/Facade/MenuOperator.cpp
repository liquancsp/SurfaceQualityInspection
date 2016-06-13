#include <Facade/MenuOperator.h>
#include <vtk_data/vtk_model_data.h>
#include <menu_operate/file_menu_operate.h>
#include <menu_operate/edit_menu_operate.h>
#include <menu_operate/view_menu_operate.h>

namespace sqi {
namespace common{

using namespace sqi::meshdata;

MenuOperator::MenuOperator(const sqi::meshdata::MeshDataPtr mesh, 
                           const vtkSmartPointer<lq::vtk_model_data>& data) : 
  mesh_data_(mesh), data_set_(data),
  file_operator_(new lq::FileMenuOperate()),
  edit_operator_(new lq::EditMenuOperate()),
  view_operator_(new lq::ViewMenuOperate())
{
  cout << "[#Infp-Init]Init menu operator." << endl;
  file_operator_->SetDataSet(data_set_);
  file_operator_->SetMeshData(mesh_data_);
  edit_operator_->SetDataSet(data_set_);
  edit_operator_->SetMeshData(mesh_data_);
  view_operator_->SetDataSet(data_set_);
  view_operator_->SetMeshData(mesh_data_);
}

void MenuOperator::LoadModelFile(const std::string &file_name, lq::WindowFlag win)
{
  //mesh_data_->IsWinHas(win) = true;
  file_operator_->LoadFile(file_name, win);
}

void MenuOperator::SetRenderData(lq::WindowFlag win)
{
  file_operator_->SetRenderData(*mesh_data_.get(), win);
}

void MenuOperator::CreateVtkRender(lq::WindowFlag win)
{
  file_operator_->CreateRender(*mesh_data_.get(), win);
  if(win == lq::kRight)
  {
    data_set_->R(win)->ResetCamera();
    data_set_->CM() = data_set_->R(kLeft)->GetActiveCamera();
  }
}

void MenuOperator::SetSelectClassifyColor(lq::WindowFlag win)
{
  this->CleanPolyColor(win);
  this->InitClassifyColor();
  edit_operator_->SetSelectClassifyColor(win);
  this->GenerateClassifyMapColor(win);
  this->MapColorOnVertex(win);
  this->UpdateClassifyColorLengend(win);
}

void MenuOperator::CreateScene(const std::string &file_name, lq::WindowFlag win)
{
  LoadModelFile(file_name, win);
  CreateVtkRender(win);
  mesh_data_->IsWinHas(win) = true;
  if(win == lq::kRight)
  {
    data_set_->R(win)->ResetCamera();
    data_set_->CM() = data_set_->R(kLeft)->GetActiveCamera();
  }
}

lq::ReturnValue MenuOperator::UpdateColorScalar(ColorScalarType scalar_type, 
                                                double max_scalar, double min_scalar)
{
  if(mesh_data_->AlorithmModel() == lq::kCalTwoDiff)
  {
    UpdateColorScalar(scalar_type, max_scalar, min_scalar, lq::kRight);
    UpdateColorLegend(lq::kRight);
    return lq::kOK;
  }
  else if(mesh_data_->AlorithmModel() == lq::kCalSingleProperty)
  {
   
    UpdateColorScalar(scalar_type, max_scalar, min_scalar, lq::kLeft);
    UpdateColorScalar(scalar_type, max_scalar, min_scalar, lq::kRight);
    UpdateColorLegend(lq::kLeft);
    UpdateColorLegend(lq::kRight);
    return lq::kOK;
  }
  else 
    return lq::kError;
}


void MenuOperator::UpdateColorScalar(meshdata::ColorScalarType scalar_type, 
                                     double max_scalar, double min_scalar, 
                                     lq::WindowFlag win)
{
  mesh_data_->ScalarType() = scalar_type;
  if(scalar_type == sqi::meshdata::kUniform)
  {
    mesh_data_->MaxColor(win) = max_scalar;
    mesh_data_->MinColor(win) = 0 - max_scalar;
    mesh_data_->MinThreshold() = min_scalar;
  }
  if(data_set_->IsFirstColorScalar(win))
  {
    edit_operator_->MapErrorColor(win);
    edit_operator_->SetColorOnPoly(win);
  }
  InitColor();  
}

void MenuOperator::UpdateClassifyColor(lq::WindowFlag win)
{
  edit_operator_->MapClassifyColor(win);
  edit_operator_->SetColorOnVertex(win);
}

void MenuOperator::InitParamters()
{
  mesh_data_->ScalarType() = sqi::meshdata::kIndependent;
}


void MenuOperator::ComputeError(const lq::InspectFlag flag,
                                const lq::AlgType type, 
                                const lq::WindowFlag win)
{
  std::cout << "In MenuOperator " << std::endl;
  edit_operator_->ComputeError(flag, type, win);
}

void MenuOperator::ComputeDescriptor(const lq::InspectFlag flag, 
                                     const lq::WindowFlag win)
{
  edit_operator_->ComputeDescriptor(flag, win);
}

void MenuOperator::InitColor()
{
  edit_operator_->InitColorList();
}

void MenuOperator::InitClassifyColor()
{
  edit_operator_->InitClassifyColorList();
}

void MenuOperator::GenerateMapColor(lq::WindowFlag win)
{
  edit_operator_->MapErrorColor(win);
}

void MenuOperator::GenerateClassifyMapColor(lq::WindowFlag win)
{
  edit_operator_->MapClassifyColor(win);
}

void MenuOperator::MapColorOnModel(lq::WindowFlag win)
{
  edit_operator_->SetColorOnPoly(win);
}

void MenuOperator::MapColorOnVertex(lq::WindowFlag win)
{
  edit_operator_->SetColorOnVertex(win);
}

void MenuOperator::SetParameters(lq::WindowFlag win)
{
  data_set_->IsFirstColorScalar(win) = true;
}

void MenuOperator::UpdateColorLegend(lq::WindowFlag win)
{
  view_operator_->AddColorLegend(win);
}

void MenuOperator::UpdateClassifyColorLengend(lq::WindowFlag win)
{
  view_operator_->AddClassifyColorLengend(win);
}

void MenuOperator::CleanPolyColor(lq::WindowFlag win)
{
  edit_operator_->ClearPolyColor(win);
}

void MenuOperator::ResetLightParameter(lq::WindowFlag win)
{
  file_operator_->SetLightParameter(win);
}

void MenuOperator::ResizeColorLegend()
{
  ResizeColorLegend(lq::kLeft);
  //if(mesh_data_->AlorithmModel() == lq::kCalSingleProperty)
  ResizeColorLegend(lq::kRight);
}

void MenuOperator::ResizeColorLegend(lq::WindowFlag win)
{
  view_operator_->ResizeColorLegend(win);
}


void MenuOperator::FlipNormalOperator(lq::WindowFlag win)
{
  view_operator_->FlipModelNormal(win);
}

void MenuOperator::ClearSceneColorItem(lq::WindowFlag win)
{
  data_set_->ResetColorBar(win);
  if(mesh_data_->IsWinHas(win))
  {
    CleanPolyColor(win);
    ResetLightParameter(win);
  }
}

}
}
