#include <menu_operate/file_menu_operate.h>

#include <callback/interactor_style.h>
#include <callback/PointPickInteractorStyle.h>
#include <callback/AreaPickCallback.h>
#include <widget_operate/ModelNameTextActor.h>

#include <vtkCellArray.h>
#include <vtkTriangle.h>
#include <vtkLight.h>
#include <vtkProperty.h>

#include <vtkCellData.h>
#include <vtkCellArray.h>
#include <vtkDoubleArray.h>
#include <vtkRenderedAreaPicker.h>
#include <vtkIdFilter.h>
#include <vtkDataSetSurfaceFilter.h>

#include <string>

#include <ConsoleMessage.h>
#include <vtk_data/vtk_model_data.h>

#include <ImportNAS.h>
//#include <ImportOBJ.h>
#include <wrap/io_trimesh/import_obj.h>
#include <wrap/io_trimesh/export_obj.h>
#include <wrap/io_trimesh/import.h>
#include <vcg/complex/algorithms/smooth.h>

namespace lq {

using namespace sqi::meshdata;
using namespace sqi::WidgetOp;
using namespace std;

FileMenuOperate::FileMenuOperate()
{
  std::cout << "[#Info-Init]Init file menu operate." << std::endl;
  fout.open("operation_log.txt", fstream::out | fstream::app);
}

FileMenuOperate::~FileMenuOperate()
{
  
}

void FileMenuOperate::SetDataSet(const vtkSmartPointer<vtk_model_data> &other_data_set)
{
  this->data_set_ = other_data_set;
}

int FileMenuOperate::LoadFile(const std::string &file_name, const int flag)
{
  assert(mesh_data_.use_count() > 0);
  if(mesh_data_.use_count() == 0 || mesh_data_ == NULL)
  {
    
    return lq::kError;
  }
  size_t spliter_idx = file_name.find_last_of('.');
  std::string tmp_name = file_name.substr(spliter_idx+1);
  if(strcmp(tmp_name.c_str(), "obj") == 0)
  { 
    int mask;
    /*if(*/vcg::tri::io::ImporterOBJ<Mesh>::Open(mesh_data_->M(flag), file_name.c_str(), mask);/* != 0)*/
    //    {
    //      sqi::io::ConsoleMessage<string>::MessageFailImportFile(file_name, __FILE__, __LINE__);
    //      return kError;
    //    }
  }
  else
  {
    int mask;
    if(vcg::tri::io::ImporterNAS<Mesh>::Open(mesh_data_->M(flag), file_name.c_str(), mask) !=0)
    {
      sqi::io::ConsoleMessage<string>::MessageFailImportFile(file_name, __FILE__, __LINE__);
      return kError;
    }
  }
  mesh_data_->RebuildMesh(flag);
  mesh_data_->UpdateMesh(flag);
  if(mesh_data_->RepairVertexTopology(flag))
  {
    mesh_data_->RebuildMesh(flag);
    mesh_data_->UpdateMesh(flag);
  }
  if(mesh_data_->RepairFaceTopology(flag))
  {
    mesh_data_->RebuildMesh(flag);
    mesh_data_->UpdateMesh(flag);
  }
  sqi::io::ConsoleMessage<string>::SendMessage(file_name, sqi::io::kImportSuccess);
  std::cout << "edge num : " << mesh_data_->M(flag).edge.size() << std::endl;
  std::cout << "vertex num : " << mesh_data_->VNum(flag) << " face num : " << mesh_data_->FNum(flag) << std::endl;
  return lq::kOK;
}
//------------------------------------------------------------------------------

int FileMenuOperate::CreateRender(sqi::meshdata::MeshData<sqi::meshdata::Mesh>& mesh,
                                  lq::WindowFlag flag)
{
  if(!data_set_->IsHasMapAndActor(flag))
  {
    data_set_->IsHasMapAndActor(flag) = true;
    
    if(flag == kLeft)
    {
      data_set_->CM() = data_set_->R(flag)->GetActiveCamera();
      
      data_set_->InitialCamera()->DeepCopy(data_set_->CM());
      //reset right render windows camera when the left render windows load a 
      //new file
      if(mesh_data_->IsWinHas(kRight))
        data_set_->R(kRight)->SetActiveCamera(data_set_->CM());
      
    }
    else 
    {
      data_set_->R(flag)->SetActiveCamera(data_set_->CM());
    }
    
    data_set_->R(flag)->SetViewport(data_set_->render_view_port_[flag]);
    data_set_->Mapper(flag)->RemoveAllInputs();
#if VTK_MAJOR_VERSION <= 5
    data_set_->Mapper(flag)->SetInputConnection(data_set_->PD(flag)->GetProducerPort());
#else
    data_set_->Mapper(flag)->SetInputData(data_set_->PD(flag);
    #endif
        data_set_->Actor(flag)->SetMapper(data_set_->Mapper(flag));
    data_set_->R(flag)->AddActor(data_set_->Actor(flag));
    data_set_->R(flag)->SetBackground(0.0, 0.0, 0.0);
    data_set_->QTW()->GetRenderWindow()->AddRenderer(data_set_->R(flag));
    data_set_->R(flag)->ResetCamera();
    if(!data_set_->IsHasIterStyle())
    {
      data_set_->IsHasIterStyle() = true;
      vtkSmartPointer<interactor_style> style = 
          vtkSmartPointer<interactor_style>::New();
      style->set_data_set(data_set_);
      style->set_mesh_data(mesh_data_);
//      data_set_->QTW()->GetRenderWindow()->GetInteractor()->SetInteractorStyle(style);
    }
    if(!data_set_->IsHasPickIterStyle(flag))
    {
      vtkSmartPointer<vtkAreaPicker> pick = 
          vtkSmartPointer<vtkAreaPicker>::New();
      vtkSmartPointer<sqi::vtk::PointPickInteractorStyle> inter_style = 
          vtkSmartPointer<sqi::vtk::PointPickInteractorStyle>::New();
      inter_style->SetDataSet(data_set_);
      inter_style->SetMeshData(mesh_data_);
      inter_style->SetWinFlag(flag);
      inter_style->SetCurrentRenderer(data_set_->R(flag));
      data_set_->QTW()->GetRenderWindow()->GetInteractor()->SetPicker(pick);
      data_set_->QTW()->GetRenderWindow()->GetInteractor()->SetInteractorStyle(inter_style);
//      vtkSmartPointer<sqi::vtk::AreaPickCallback> area_pick =
//          vtkSmartPointer<sqi::vtk::AreaPickCallback>::New();
//      area_pick->SetDataSet(data_set_);
//      area_pick->SetMeshData(mesh_data_);
//      area_pick->SetWinFlag(flag);
//      data_set_->QTW()->GetRenderWindow()->GetInteractor()->AddObserver(vtkCommand::EndPickEvent,
//                                                                        area_pick);
//      vtkSmartPointer<vtkRenderedAreaPicker> pick2 = 
//          vtkSmartPointer<vtkRenderedAreaPicker>::New();

//      vtkSmartPointer<vtkInteractorStyleRubberBandPick> inter_style2 = 
//          vtkSmartPointer<vtkInteractorStyleRubberBandPick>::New();
      
//      data_set_->QTW()->GetRenderWindow()->GetInteractor()->SetPicker(pick2);
//      data_set_->QTW()->GetRenderWindow()->GetInteractor()->SetInteractorStyle(inter_style2);
      data_set_->IsHasPickIterStyle(flag) = true;
    }
    
    if(flag == kRight && !data_set_->IsHasAxis())
    {
      data_set_->IsHasAxis() = true;
      
      data_set_->AxesWidget()->SetOutlineColor(0.9, 0.5, 0.1);
      //set axes size
      data_set_->AxesActor()->SetTotalLength(0.5, 0.5, 0.5);
      data_set_->AxesWidget()->SetOrientationMarker(data_set_->AxesActor());
      data_set_->AxesWidget()->SetInteractor(data_set_->QTW()->GetRenderWindow()->GetInteractor());
      //set axes widget size
      
      data_set_->AxesWidget()->SetViewport(0.0, 0.0, 0.15, 0.15);
      data_set_->AxesWidget()->SetEnabled(1);
      data_set_->AxesWidget()->InteractiveOn();
      data_set_->AxesWidget()->GetOrientationMarker()->SetDragable(0);
      data_set_->AxesWidget()->GetOrientationMarker()->SetPickable(0);
      // data_set_->AxesWidget()->GetOrientationMarker()->SetVisibility(0);
    }
    
  }
  SetLightParameter(flag);
  tr1::shared_ptr<ModelNameTextActor<vtk_model_data> > text_actor;
  text_actor.reset(new ModelNameTextActor<vtk_model_data>(data_set_));
  text_actor->set_model_name_text_actor(flag);
  data_set_->QTW()->update();
  
  return kOK;
}

int FileMenuOperate::SetRenderData(sqi::meshdata::MeshData<Mesh> &mesh, WindowFlag flag)
{
  vtkSmartPointer<vtkPoints> vtk_points = vtkSmartPointer<vtkPoints>::New();
  for(Mesh::VertexIterator vi = mesh.M(flag).vert.begin(); vi != mesh.M(flag).vert.end(); ++vi)
  {
    vtk_points->InsertNextPoint(vi->P()[0], vi->P()[1], vi->P()[2]);
  }
  vtkSmartPointer<vtkCellArray> triangle_cell =
      vtkSmartPointer<vtkCellArray>::New();
  vtkIdType pts[3];
  for(Mesh::FaceIterator fi = mesh.M(flag).face.begin(); fi != mesh.M(flag).face.end(); ++fi)
  {
    pts[0] =  mesh.GetIndexVertex((*fi).V(0), flag);
    pts[1] =  mesh.GetIndexVertex((*fi).V(1), flag);
    pts[2] =  mesh.GetIndexVertex((*fi).V(2), flag);
    //    tmp_triangle->GetPointIds()->SetId(0, mesh.GetIndexVertex((*fi).V(0), flag));
    //    tmp_triangle->GetPointIds()->SetId(1, mesh.GetIndexVertex((*fi).V(1), flag));
    //    tmp_triangle->GetPointIds()->SetId(2, mesh.GetIndexVertex((*fi).V(2), flag));
    //    triangle_cell->InsertNextCell(tmp_triangle);
    triangle_cell->InsertNextCell(3, pts);
  }
  data_set_->PD(flag)->SetPoints(vtk_points);
  data_set_->PD(flag)->SetPolys(triangle_cell);
  
  //set two render in the same render windows
  return lq::kOK;
}

void FileMenuOperate::SetLightProperty(const lq::WindowFlag flag)
{
  data_set_->LightCtrl(flag)->SetIntensity(0.8);
  data_set_->LightCtrl(flag)->SetFocalPoint(data_set_->Actor(flag)->GetPosition());
  data_set_->LightCtrl(flag)->SetConeAngle(180);
  data_set_->LightCtrl(flag)->SetLightTypeToHeadlight();
  //data_set_->LightCtrl(flag)->SetLightTypeToCameraLight();
  //data_set_->LightCtrl(flag)->SetLightTypeToSceneLight();
  data_set_->R(flag)->AddLight(data_set_->LightCtrl(flag));
}

void FileMenuOperate::SetLightParameter(const WindowFlag win)
{
  int *tmp;
  if(win == lq::kLeft)
    tmp = data_set_->ActorColor(sqi::VtkData::kLeftActor);
  else
    tmp = data_set_->ActorColor(sqi::VtkData::kRightActor);
  int *back_color = data_set_->BackFaceActorColor();
  data_set_->Actor(win)->GetProperty()->SetColor(static_cast<double>(back_color[0] / 255.0),
      static_cast<double>(back_color[1] / 255.0), static_cast<double>(back_color[2] / 255.0));
  if(data_set_->Actor(win)->GetBackfaceProperty() == NULL)
  {
    vtkSmartPointer<vtkProperty> back_property = 
        vtkSmartPointer<vtkProperty>::New();
    back_property->SetColor(static_cast<double>(tmp[0] / 255.0),
        static_cast<double>(tmp[1] / 255.0), static_cast<double>(tmp[2] / 255.0));
    data_set_->Actor(win)->SetBackfaceProperty(back_property); 
  }
  else 
  {
    data_set_->Actor(win)->GetBackfaceProperty()->SetColor(static_cast<double>(tmp[0] / 255.0),
        static_cast<double>(tmp[1] / 255.0), static_cast<double>(tmp[2] / 255.0));
  }
  data_set_->Actor(win)->GetProperty()->SetSpecular(data_set_->Specular());
  data_set_->Actor(win)->GetProperty()->SetSpecularPower(data_set_->SpecularPower());
  data_set_->Actor(win)->GetProperty()->SetAmbient(data_set_->Ambient());
  data_set_->Actor(win)->GetBackfaceProperty()->SetSpecular(data_set_->Specular());
  data_set_->Actor(win)->GetBackfaceProperty()->SetAmbient(data_set_->Ambient());
  data_set_->Actor(win)->GetBackfaceProperty()->SetSpecularPower(data_set_->SpecularPower());
  //data_set_->R(win)->AddLight(data_set_->LightCtrl(win));  
  
}

}
