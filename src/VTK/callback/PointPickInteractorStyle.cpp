#include <PointPickInteractorStyle.h>
#include <ColorOperate.h>

#include <vtkAreaPicker.h>
#include <vtkExtractGeometry.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkProperty.h>
#include <vtkPlanes.h>
#include <vtkIdTypeArray.h>
#include <vtkPointData.h>
#include <vtkCellArray.h>
#include <vtkIdFilter.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkExtractSelectedPolyDataIds.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkHardwareSelector.h>
#include <vtkExtractSelection.h>

#include <vtkSelectVisiblePoints.h>
#include <vtkRendererCollection.h>
#include <vtkCellArray.h>
#include <vtkCellType.h>
#include <vtkCellData.h>
#include <vtkAssemblyPath.h>

#include <QtGui>
#include <QtCore>
#include <QString>
#include <QFile>
#include <QtXml/QDomDocument>
#include <QXmlStreamWriter>
#include <QObject>

using namespace std;
using namespace lq;
namespace sqi{
namespace vtk{

PointPickInteractorStyle::PointPickInteractorStyle() : vtkInteractorStyleRubberBandPick(),
  data_set_(NULL),
  win_flag_(lq::kLeft),
  inspect_director_ptr_(new sqi::common::InspectActionDirector())
{
}

PointPickInteractorStyle::PointPickInteractorStyle(meshdata::MeshDataPtr mesh, 
                                                   vtkSmartPointer<lq::vtk_model_data> &data)
  : vtkInteractorStyleRubberBandPick(),
    data_set_(data), mesh_data_(mesh), win_flag_(lq::kLeft),
    inspect_director_ptr_ (new sqi::common::InspectActionDirector(mesh, data))
{
}

PointPickInteractorStyle::~PointPickInteractorStyle()
{
  
}

void PointPickInteractorStyle::SetDataSet(const vtkSmartPointer<lq::vtk_model_data> &data)
{
  this->data_set_ = data;
  inspect_director_ptr_->SetDataSet(data_set_);
}

void PointPickInteractorStyle::SetMeshData(const meshdata::MeshDataPtr &mesh)
{
  this->mesh_data_ = mesh;
  inspect_director_ptr_->SetMeshData(mesh);
}

void PointPickInteractorStyle::SetWinFlag(const lq::WindowFlag &flag)
{
  this->win_flag_ = flag;
}

int PointPickInteractorStyle::GetRepresentation()
{
  if(rwi_ == NULL)
    rwi_ = this->Interactor;
  vtkActorCollection *ac;
  vtkActor *anActor, *aPart;
  vtkAssemblyPath *path;
  vtkRenderer* render;
  /*it must judge whether the render windows have vtkrender, if there is no vtkrender
    return -1. if not the program will collapse when user press the right button of mouse*/
  render = this->CurrentRenderer;
  if(render == NULL)
    return -1;
  this->FindPokedRenderer(rwi_->GetEventPosition()[0],
                          rwi_->GetEventPosition()[1]);
  ac = this->CurrentRenderer->GetActors();
  vtkCollectionSimpleIterator ait;
  for (ac->InitTraversal(ait); (anActor = ac->GetNextActor(ait)); ) 
  {
    for (anActor->InitPathTraversal(); (path = anActor->GetNextPath()); ) 
    {
      aPart=static_cast<vtkActor *>(path->GetLastNode()->GetViewProp());
      if(aPart->GetProperty()->GetEdgeVisibility())
        return kSurfaceWithEdge;
      else 
        return aPart->GetProperty()->GetRepresentation();
    }
  }
  return 0;
}

void PointPickInteractorStyle::SetPointRepresentation()
{
  for(size_t i = 0; i < 2; ++i)
    data_set_->Actor(i)->GetProperty()->SetRepresentationToPoints();
  rwi_->Render();
}

void PointPickInteractorStyle::SetFaceRepresentation()
{
  //set the two model in the same represent
  for(size_t i = 0; i < 2; ++i)
  {
    data_set_->Actor(i)->GetProperty()->EdgeVisibilityOff();
    data_set_->Actor(i)->GetProperty()->SetRepresentationToSurface();
  }
  rwi_->Render();
}

void PointPickInteractorStyle::SetWireframeRepresentation()
{
  //set the two model in the same represent
  for(size_t i = 0; i < 2; ++i)
    data_set_->Actor(i)->GetProperty()->SetRepresentationToWireframe();
  rwi_->Render();
}

void PointPickInteractorStyle::SetSurfaceWithEdgeRepresentation()
{
  //set the two model in the same represent
  for(size_t i = 0; i < 2; ++i)
  {
    data_set_->Actor(i)->GetProperty()->SetRepresentationToSurface();
    data_set_->Actor(i)->GetProperty()->EdgeVisibilityOn();
  }
  rwi_->Render();
}

void PointPickInteractorStyle::OnKeyRelease()
{
  std::string key = this->Interactor->GetKeySym();
  std::cout << "[#Info]Key : " <<  key << " has been released!" << std::endl;
  int n = index_list_.size();
  std::cout << "[#Info]The number of selected patch is : " << n << endl;
  if(key.compare("r") == 0 || key.compare("R") == 0 )//change pick type
  {
    data_set_->AreaPickType() = !data_set_->AreaPickType();
    sqi::color::ColorOperate<sqi::meshdata::MeshDataPtr>::InitClassifyColor(mesh_data_);
    std::cout << "[#Info]Area pick type : " << data_set_->AreaPickType() << std::endl;
  }
  else if(key.compare("c") == 0 || key.compare("C") == 0)// clean selected area
  {
    for(size_t i = 0; i < selected_mapper_list_.size(); ++i)
      selected_mapper_list_[i]->RemoveAllInputs();
    selected_mapper_list_.clear();
    selected_actor_list_.clear();
    index_list_.clear();
    data_set_->QTW()->update();
  }
  else if(key.compare("u") == 0 || key.compare("U") == 0)
  {
    
  }
  else if(key.compare("z") == 0 ||key.compare("Z") == 0) //undo the last selection
  {
    if(selected_mapper_list_.size() != 0)
    {
      selected_mapper_list_[selected_mapper_list_.size() - 1]->RemoveAllInputs();
      selected_mapper_list_.pop_back();
      selected_actor_list_.pop_back();
      index_list_.pop_back();
    }
    
  }
  else if(key.compare("o") == 0 || key.compare("O") == 0)//export the index of selected points
  {
    QString file_name = QFileDialog::getSaveFileName(NULL, QObject::tr("Save File"),
                               "untitled.select",
                               QObject::tr("Select Files(*.select)"));
    std::set<int> index_set;
    ifstream fin(qPrintable(file_name));
    if(!fin.fail())
    {
      string str;
      while(getline(fin, str) != NULL)
        index_set.insert(atoi(str.c_str()));
      fin.close();
    }
    for(size_t i = 0; i < index_list_.size(); ++i)
    {
//      for(size_t j = 0; j < index_list_[i].size(); ++j)
//      {
////        sqi::meshdata::FaceIterator fi = mesh_data_->FBeginIter(win_flag_) + index_list_[i][j];
////        for(int k = 0; k < 3; ++k)
////        {
//          int vertex_idx = mesh_data_->GetIndexVertex(fi->V(k), win_flag_);
      index_set.insert(index_list_[i].begin(), index_list_[i].end());
//        }
//      }
    }
    ofstream fout(qPrintable(file_name));
    for(set<int>::iterator iter = index_set.begin(); iter != 
        index_set.end(); ++iter)
      fout << *iter << endl;
    fout.close();
  }
  else if(key >= "1" && key <= "4")
  {
    if(selected_actor_list_.size() == 0)
      return ;
    if(mesh_data_->SelectFileList().size() == 0)
      mesh_data_->SelectIdxList().resize(3);
    cout << "[#Info]Selected actor number is : " << n << endl;
    if(key.compare("1") == 0)
    {
      UpdateClassifyIndex(index_list_[n - 1], 1);
      cout << "std color size : " << mesh_data_->StdColor().size() << endl;
      selected_actor_list_[n - 1]->GetProperty()->SetColor(mesh_data_->StdColor()[0].R,
          mesh_data_->StdColor()[0].G, mesh_data_->StdColor()[0].B);
    }
    else if(key.compare("2") == 0)
    {
      UpdateClassifyIndex(index_list_[n - 1], 2);
      selected_actor_list_[n - 1]->GetProperty()->SetColor(mesh_data_->StdColor()[1].R,
          mesh_data_->StdColor()[1].G, mesh_data_->StdColor()[1].B);
    }
    else if(key.compare("4") == 0)
    {
      UpdateClassifyIndex(index_list_[n - 1], 3);
      selected_actor_list_[n - 1]->GetProperty()->SetColor(mesh_data_->StdColor()[2].R,
          mesh_data_->StdColor()[2].G, mesh_data_->StdColor()[2].B);
    }
  }
  else if(key.compare("d") == 0)
  {
    UpdateClassifyIndex(index_list_[n - 1], 5);
    selected_actor_list_[n - 1]->GetProperty()->SetColor(mesh_data_->StdColor()[4].R,
        mesh_data_->StdColor()[4].G, mesh_data_->StdColor()[4].B);
  }
  data_set_->QTW()->update();
}

void PointPickInteractorStyle::OnLeftButtonUp()
{
  // Forward events
  RecalError();
  vtkInteractorStyleRubberBandPick::OnLeftButtonUp();
  if(data_set_->AreaPickType())
  {
    vtkSmartPointer<vtkIdFilter> id_filter =
        vtkSmartPointer<vtkIdFilter>::New();
    id_filter->SetInput(data_set_->PD(win_flag_));
    id_filter->SetIdsArrayName("PolyData");
    id_filter->Update();
    vtkSmartPointer<vtkDataSetSurfaceFilter> surface_filter =
        vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
    surface_filter->SetInputConnection(id_filter->GetOutputPort());
    surface_filter->PassThroughCellIdsOn();
    surface_filter->Update();
    
    vtkSmartPointer<vtkHardwareSelector> select =
        vtkSmartPointer<vtkHardwareSelector>::New();
    select->SetRenderer(this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
    unsigned int rect[4];
    rect[0] = this->StartPosition[0];
    rect[1] = this->EndPosition[1];
    rect[2] = this->EndPosition[0];
    rect[3] = this->StartPosition[1];
    select->SetArea(rect);
    select->SetFieldAssociation(vtkDataObject::FIELD_ASSOCIATION_CELLS);
    vtkSmartPointer<vtkSelection> sel_res =
        vtkSmartPointer<vtkSelection>::New();
    sel_res = select->Select();
    if(!sel_res)
    {
      std::cerr << "[#Error]Selection not support" << std::endl;
      return;
    }
    cout << "[#Info]The number of selected nodes: " << sel_res->GetNumberOfNodes() << endl;
    vtkSmartPointer<vtkSelectionNode> cell_ids = 
        vtkSmartPointer<vtkSelectionNode>::New();
    cell_ids = sel_res->GetNode(0);
    if(cell_ids)
    {
      cell_ids->SetFieldType(vtkSelectionNode::CELL);
      cell_ids->SetContentType(vtkSelectionNode::INDICES);
      vtkSmartPointer<vtkExtractSelectedPolyDataIds> extr = 
          vtkSmartPointer<vtkExtractSelectedPolyDataIds>::New();
      extr->SetInput(0, surface_filter->GetOutput());
      vtkSmartPointer<vtkSelection> temp = 
          vtkSmartPointer<vtkSelection>::New();
      temp->AddNode(cell_ids);
      extr->SetInput(1, temp);
      extr->Update();
      vtkSmartPointer<vtkDataSetMapper> selected_mapper = 
          vtkSmartPointer<vtkDataSetMapper>::New();
      selected_mapper->SetInput(extr->GetOutput());
      selected_mapper->ScalarVisibilityOff();
      selected_mapper_list_.push_back(selected_mapper);
      vtkSmartPointer<vtkDataSetSurfaceFilter> temp_sf =
          vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
      temp_sf->SetInputConnection(extr->GetOutputPort());
      temp_sf->PassThroughCellIdsOn();
      temp_sf->Update();
      vtkPolyData* selected = temp_sf->GetOutput();
      std::cout << "Selected " << selected->GetNumberOfPoints() << " points." << std::endl;
      std::cout << "Selected " << selected->GetNumberOfCells() << " cells." << std::endl;
      vtkIdTypeArray* ids = vtkIdTypeArray::SafeDownCast(selected->GetCellData()->GetArray("PolyData"));
      std::set<int> tmp_set;
      //get cell index, it must be converted to vertex index
      for(vtkIdType i = 0; i < ids->GetNumberOfTuples(); ++i)
      {
        int face_idx = ids->GetValue(i);
        sqi::meshdata::FaceIterator fi = mesh_data_->FBeginIter(win_flag_) + face_idx;
        tmp_set.insert(mesh_data_->GetIndexVertex((*fi).V(0), win_flag_));
        tmp_set.insert(mesh_data_->GetIndexVertex((*fi).V(1), win_flag_));
        tmp_set.insert(mesh_data_->GetIndexVertex((*fi).V(2), win_flag_));
      }
      std::vector<int> tmp_vec(tmp_set.begin(), tmp_set.end());
      cout << "[#Info]tmp_vec size : " << tmp_vec.size() << endl;
      index_list_.push_back(tmp_vec);
      vtkSmartPointer<vtkActor> selected_actor =
          vtkSmartPointer<vtkActor>::New();
      selected_actor->SetMapper(selected_mapper);
      selected_actor_list_.push_back(selected_actor);
      SetLightParameter(lq::kLeft, selected_actor);
//      selected_actor->GetProperty()->EdgeVisibilityOn();
//      selected_actor->GetProperty()->SetEdgeColor(0, 0, 1);
//      selected_actor->GetProperty()->SetLineWidth(2.0);
      selected_actor->GetProperty()->SetColor(1, 0.84, 0);
      selected_actor->GetProperty()->SetPointSize(4.0);
      selected_actor->GetProperty()->SetRepresentationToPoints();
      selected_actor->SetMapper(selected_mapper);
      data_set_->R(win_flag_)->AddActor(selected_actor);
    }
    else 
    {
      
    }
    data_set_->QTW()->update();
  }
}

void PointPickInteractorStyle::OnRightButtonDown()
{
  vtkInteractorStyleRubberBandPick::OnRightButtonDown();
}

void PointPickInteractorStyle::OnMouseMove()
{
//  RecalError();
  vtkInteractorStyleRubberBandPick::OnMouseMove();
}

void PointPickInteractorStyle::OnMouseWheelForward()
{
  RecalError();
  vtkInteractorStyleRubberBandPick::OnMouseWheelForward();
}

void PointPickInteractorStyle::OnMouseWheelBackward()
{
  RecalError();
  vtkInteractorStyleRubberBandPick::OnMouseWheelBackward();
}


void PointPickInteractorStyle::SetLightParameter(const lq::WindowFlag flag, 
                                                 vtkSmartPointer<vtkActor> actor)
{
  int *tmp;
  if(flag == lq::kLeft)
    tmp = data_set_->ActorColor(sqi::VtkData::kLeftActor);
  else
    tmp = data_set_->ActorColor(sqi::VtkData::kRightActor);
  int *back_color = data_set_->BackFaceActorColor();
  actor->GetProperty()->SetColor(static_cast<double>(back_color[0] / 255.0),
      static_cast<double>(back_color[1] / 255.0), static_cast<double>(back_color[2] / 255.0));
  if(actor->GetBackfaceProperty() == NULL)
  {
    vtkSmartPointer<vtkProperty> back_property = 
        vtkSmartPointer<vtkProperty>::New();
    back_property->SetColor(static_cast<double>(tmp[0] / 255.0),
        static_cast<double>(tmp[1] / 255.0), static_cast<double>(tmp[2] / 255.0));
    actor->SetBackfaceProperty(back_property); 
  }
  else 
  {
    actor->GetBackfaceProperty()->SetColor(static_cast<double>(tmp[0] / 255.0),
        static_cast<double>(tmp[1] / 255.0), static_cast<double>(tmp[2] / 255.0));
  }
  actor->GetProperty()->SetSpecular(data_set_->Specular());
  actor->GetProperty()->SetSpecularPower(data_set_->SpecularPower());
  actor->GetProperty()->SetAmbient(data_set_->Ambient());
  actor->GetBackfaceProperty()->SetSpecular(data_set_->Specular());
  actor->GetBackfaceProperty()->SetAmbient(data_set_->Ambient());
  actor->GetBackfaceProperty()->SetSpecularPower(data_set_->SpecularPower());
}

void PointPickInteractorStyle::UpdateClassifyIndex(std::vector<int> &list, int label)
{
  std::cout << "[#Info]Begin update classify index." << endl;
  cout << "[#Info]list size : " << list.size() << endl;
  for(size_t i = 0; i < mesh_data_->SelectIdxList().size(); ++i)
  {
    if((i + 1) != label || label == 5)
    {
      for(size_t j = 0; j < list.size(); ++j)
        mesh_data_->SelectIdxList()[i].erase(list[j]);
    }
    else 
    {
      for(size_t j = 0; j < list.size(); ++j)
        mesh_data_->SelectIdxList()[i].insert(list[j]);
    }
  }
  std::cout << "[#Info]update classify index completed." << endl;
}

void PointPickInteractorStyle::RecalError()
{
  assert(mesh_data_.use_count() > 0);
  if(data_set_->AlgType() == lq::kReflectionMap)
    inspect_director_ptr_->Execute(lq::kReflectionMap, lq::kCalTwoDiff);
  if(data_set_->AlgType() == lq::kMixReflConvex)
    inspect_director_ptr_->Execute(lq::kMixReflConvex, lq::kCalTwoDiff);
}

}

}
