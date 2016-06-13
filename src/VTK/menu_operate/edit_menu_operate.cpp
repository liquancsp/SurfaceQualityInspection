#include <menu_operate/edit_menu_operate.h>
#include <callback/viewpoint_callback.h>
#include <vtkUnsignedCharArray.h>
#include <vtkCellData.h>
#include <vtkPolyDataMapper.h>
#include <vtkSphereSource.h>
#include <vtkProperty.h>
#include <vtkPointData.h>

#include <ColorOperate.h>
#include <ConsoleMessage.h>
#include <Strategy/AlgorithmStrategy.h>
#include <Strategy/DescriptorStrategy.h>
#include <vtk_data/vtk_model_data.h>
#include <menu_operate/LocalAlign.h>

#include <string>

//#define MEAN_CURVATURE
namespace lq
{

using namespace sqi::alg;
using namespace sqi::color;
using namespace sqi::meshdata;
using namespace sqi::common;
using namespace std;
EditMenuOperate::EditMenuOperate() : 
  alg_strategy_ptr_(new AlgorithmStrategy()), 
  descriptor_strategy_ptr_(new DescriptorStrategy())
{
  cout << "[#Info-Init]Init edit menu operate." << endl;
  fout.open("operation_log.txt", fstream::out | fstream::app);
}

EditMenuOperate::~EditMenuOperate()
{
  
}

/*************************************************************************************************/
//begin public function

void EditMenuOperate::SetDataSet(const vtkSmartPointer<vtk_model_data> &other_data_set)
{
  this->data_set_ = other_data_set;
  this->alg_strategy_ptr_->SetDataSet(data_set_);
  this->descriptor_strategy_ptr_->SetDataSet(data_set_);
}

void EditMenuOperate::SetMeshData(const sqi::meshdata::MeshDataPtr &other)
{
  this->mesh_data_ = other;
  this->alg_strategy_ptr_->SetMeshData(other);
  this->descriptor_strategy_ptr_->SetMeshData(other);
}

void EditMenuOperate::ComputeError(const lq::InspectFlag flag, 
                                     const lq::AlgType type,
                                     const lq::WindowFlag win)
{
  //set LocalAlign class's data_set and build KdTree
  std::cout << "at begin in edit_menu_operate" << std::endl;
  LocalAlign::SetDataSet(data_set_);
  ZernikerPropagation::SetDataSet(data_set_);
//  data_set_->BuildKdTree();
  alg_strategy_ptr_->SetAlgType(flag);
  cout << "in edit_menu_operate" << std::endl;
  if(type == lq::kCalTwoDiff)
  {
    data_set_->BuildKdTree();
    alg_strategy_ptr_->CalTwoModelDiff();
  }
  else if(type == lq::kCalSingleProperty)
    alg_strategy_ptr_->CalOneModelProperty(win);
  //#ifdef My_Debug
  //  std::ofstream fout("edit_meanu_operate_error.txt");
  //  for(size_t i = 0; i < data_set_->triangle_error_.size(); ++i)
  //  {
  //    fout << "data_set_ " << data_set_->triangle_error_[i] << std::endl;
  //  }
  //#endif
}

void EditMenuOperate::ComputeDescriptor(const InspectFlag flag, const WindowFlag win)
{
  cout << "[#Info]Compute Descriptor in edit menu operate." << endl;
  descriptor_strategy_ptr_->SetAlgType(flag);
  descriptor_strategy_ptr_->Execute(win);
}

void EditMenuOperate::ComputeSingleProperty(const InspectFlag flag,
                                              const WindowFlag win)
{
  alg_strategy_ptr_->SetAlgType(flag);
  alg_strategy_ptr_->CalOneModelProperty(win);
}

int EditMenuOperate::InitColorList()
{
//  if(!mesh_data_->IsInitColor())
//  {
//    if(ColorOperate<MeshDataPtr>::InitBilinearColor(mesh_data_)
//       /*ColorOperate<MeshDataPtr>::InitStdColor(mesh_data_)*/
//       /*ColorOperate<MeshDataPtr>::InitHSLColor(mesh_data_)*/ == kError)
//    {
//      return kColorInitFail;
//    }
//    else
//    {
//      mesh_data_->IsInitColor() = true;
//      return kOK;
//    }
//  }
//  else
//    return kColorAlreadyInit;
  if(ColorOperate<MeshDataPtr>::InitBilinearColor(mesh_data_)
     /*ColorOperate<MeshDataPtr>::InitStdColor(mesh_data_)*/
     /*ColorOperate<MeshDataPtr>::InitHSLColor(mesh_data_)*/ == kError)
  {
    return kColorInitFail;
  }
  else
  {
//    mesh_data_->IsInitColor() = true;
    return kOK;
  }
}

int EditMenuOperate::InitClassifyColorList()
{
  if(ColorOperate<MeshDataPtr>::InitClassifyColor(mesh_data_) == kError)
    return kColorInitFail;
  else 
    return kOK;
}

int EditMenuOperate::MapErrorColor(lq::WindowFlag win)
{
  sqi::io::ConsoleMessage<string>::MessageText("Begin mapping error color.", 
                                               sqi::io::kInfoColor);
  //clear the light just 
  data_set_->Actor(win)->GetProperty()->SetSpecular(0.0);
  data_set_->Actor(win)->GetProperty()->SetSpecularPower(0.0);
  data_set_->Actor(win)->GetProperty()->SetAmbient(0.0);
  data_set_->Actor(win)->GetBackfaceProperty()->SetSpecular(0.0);
  data_set_->Actor(win)->GetBackfaceProperty()->SetSpecularPower(0.0);
  data_set_->Actor(win)->GetBackfaceProperty()->SetAmbient(0.0);
  //it can use both of follow function to map the error color
  //  ColorOperate<MeshDataPtr>::UpdateColor(mesh_data_, mesh_data_->MinE(),
  //                                         mesh_data_->MaxE());
  //ColorOperate<MeshDataPtr>::UpdateColor(mesh_data_);
  //ColorOperate<MeshDataPtr>::UpdateHSLColor(mesh_data_);
  ColorOperate<MeshDataPtr>::UpdateRainbowColor(mesh_data_, win);
  return 0;
}

int EditMenuOperate::MapClassifyColor(WindowFlag win)
{
  sqi::io::ConsoleMessage<string>::MessageText("Begin mapping error color.", 
                                               sqi::io::kInfoColor);
  //clear the light just 
  data_set_->Actor(win)->GetProperty()->SetSpecular(0.0);
  data_set_->Actor(win)->GetProperty()->SetSpecularPower(0.0);
  data_set_->Actor(win)->GetProperty()->SetAmbient(0.0);
  data_set_->Actor(win)->GetBackfaceProperty()->SetSpecular(0.0);
  data_set_->Actor(win)->GetBackfaceProperty()->SetSpecularPower(0.0);
  data_set_->Actor(win)->GetBackfaceProperty()->SetAmbient(0.0);
  ColorOperate<MeshDataPtr>::UpdateClassifyColor(mesh_data_);
  return 0;
}


void EditMenuOperate::SetColorOnPoly(lq::WindowFlag win)
{
  sqi::io::ConsoleMessage<string>::MessageText("Set color on the model.",
                                               sqi::io::kInfoColor);
  vtkSmartPointer<vtkUnsignedCharArray> cell_color = 
      vtkSmartPointer<vtkUnsignedCharArray>::New();
  ClearPolyColor(win);
  cell_color->Reset();
  cell_color->Initialize();
  cell_color->SetNumberOfComponents(3);
  cell_color->SetName("error_color");
  //set color bar's range
  double M;
  data_set_->ScalarActor(win)->VisibilityOn();
  if(lq::is_larger(fabs(mesh_data_->MaxE(win)), fabs(mesh_data_->MinE(win))))
    M = fabs(mesh_data_->MaxE(win));
  else
    M = fabs(mesh_data_->MinE(win));
  if(mesh_data_->ScalarType() == kIndependent)
    data_set_->LookupTable(win)->SetTableRange (0 - M, M);
  else if(mesh_data_->ScalarType() = kUniform)
    data_set_->LookupTable(win)->SetTableRange(mesh_data_->MinColor(win), 
                                               mesh_data_->MaxColor(win));
  for(FaceIterator fi = mesh_data_->FBeginIter(win); 
      fi != mesh_data_->FEndIter(win); ++fi)
  {
    unsigned char tmp_color[3];
    tmp_color[0] = (*fi).C()[0];
    tmp_color[1] = (*fi).C()[1];
    tmp_color[2] = (*fi).C()[2];
    cell_color->InsertNextTupleValue(tmp_color);
  }
  data_set_->PD(win)->GetCellData()->SetScalars(cell_color);
  data_set_->QTW()->update();
}

void EditMenuOperate::SetColorOnVertex(WindowFlag win)
{
  sqi::io::ConsoleMessage<string>::MessageText("Set color on the model's vertex.",
                                               sqi::io::kInfoColor);
  vtkSmartPointer<vtkUnsignedCharArray> vertex_color = 
      vtkSmartPointer<vtkUnsignedCharArray>::New();
  ClearPolyColor(win);
  vertex_color->Reset();
  vertex_color->Initialize();
  vertex_color->SetNumberOfComponents(3);
  vertex_color->SetName("error_color");
  //set color bar's range
  double M;
  data_set_->ScalarActor(win)->VisibilityOn();
  if(lq::is_larger(fabs(mesh_data_->MaxE(win)), fabs(mesh_data_->MinE(win))))
    M = fabs(mesh_data_->MaxE(win));
  else
    M = fabs(mesh_data_->MinE(win));
  if(mesh_data_->ScalarType() == kIndependent)
    data_set_->LookupTable(win)->SetTableRange (0 - M, M);
  else if(mesh_data_->ScalarType() = kUniform)
    data_set_->LookupTable(win)->SetTableRange(mesh_data_->MinColor(win), 
                                               mesh_data_->MaxColor(win));
  for(VertexIterator vi = mesh_data_->VBeginIter(win); 
      vi != mesh_data_->VEndIter(win); ++vi)
  {
    unsigned char tmp_color[3];
    tmp_color[0] = (*vi).C()[0];
    tmp_color[1] = (*vi).C()[1];
    tmp_color[2] = (*vi).C()[2];
//    cout << "tmp _color : " << static_cast<int>(tmp_color[0]) << " " << static_cast<int>(tmp_color[1])
//        << " " << static_cast<int>(tmp_color[2]) << endl;
    vertex_color->InsertNextTupleValue(tmp_color);
  }
  data_set_->PD(win)->GetPointData()->SetScalars(vertex_color);
  data_set_->QTW()->update();
}

void EditMenuOperate::ClearPolyColor(lq::WindowFlag win)
{
//  sqi::io::ConsoleMessage<string>::MessageText("Clear color on model.",
//                                               sqi::io::kInfoColor);
//  vtkSmartPointer<vtkUnsignedCharArray> cell_color = 
//      vtkSmartPointer<vtkUnsignedCharArray>::New();
//  cell_color->Reset();
//  cell_color->Initialize();
//  cell_color->SetNumberOfComponents(3);
//  cell_color->SetName("default_color");
//  unsigned char tmp_color[3] = {192, 192, 192};
//  for(size_t i = 0; i < mesh_data_->FNum(win); ++i)
//  {
//    cell_color->InsertNextTupleValue(tmp_color);
//  }
//  data_set_->PD(win)->GetCellData()->SetScalars(cell_color);
  data_set_->PD(win)->GetPointData()->Initialize();
  data_set_->PD(win)->GetCellData()->Initialize();
  data_set_->QTW()->update();
}

void EditMenuOperate::SetSelectClassifyColor(WindowFlag win)
{
  VertexIterator vi;
  for(size_t i = 0; i < mesh_data_->VNum(win); ++i)
  {
    vi = mesh_data_->VBeginIter(win) + i;
    vi->Q() = 5.0f;
  }
  for(size_t i = 0; i < mesh_data_->SelectIdxList().size(); ++i)
  {
    for(set<int>::iterator iter = mesh_data_->SelectIdxList()[i].begin();
        iter !=  mesh_data_->SelectIdxList()[i].end(); ++iter)
    {
      vi = mesh_data_->VBeginIter(win) + *iter;
      vi->Q() = static_cast<double>(i + 1);
    }
  }
}

}//end of namespace
