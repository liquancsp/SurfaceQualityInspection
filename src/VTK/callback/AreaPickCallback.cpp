#include <AreaPickCallback.h>

#include <vtkDataSetSurfaceFilter.h>
#include <vtkIdFilter.h>
#include <vtkHardwareSelector.h>
#include <vtkSelection.h>
#include <vtkSelectionNode.h>
#include <vtkExtractSelectedPolyDataIds.h>
#include <vtkProperty.h>

using namespace std;
namespace sqi{
namespace vtk{

AreaPickCallback::AreaPickCallback() : data_set_(NULL), win_flag_(lq::kLeft),
  selected_actor_(vtkSmartPointer<vtkActor>::New()),
  selected_mapper_(vtkSmartPointer<vtkDataSetMapper>::New())
{
  
}

AreaPickCallback::~AreaPickCallback(){
  
}

void AreaPickCallback::SetDataSet(const vtkSmartPointer<lq::vtk_model_data> &data)
{
  this->data_set_ = data;
}

void AreaPickCallback::SetMeshData(const meshdata::MeshDataPtr &mesh)
{
  this->mesh_data_ = mesh;
}

void AreaPickCallback::SetWinFlag(const lq::WindowFlag &flag)
{
  this->win_flag_ = flag;
}

void AreaPickCallback::Execute(vtkObject *obj, unsigned long, void *cell_data)
{
  std::cout << "------2" << std::endl;
//  vtkSmartPointer<vtkIdFilter> id_filter =
//      vtkSmartPointer<vtkIdFilter>::New();
//  id_filter->SetInput(data_set_->PD(win_flag_));
//  id_filter->SetIdsArrayName("PolyData");
//  id_filter->Update();
//  vtkSmartPointer<vtkDataSetSurfaceFilter> surface_filter =
//      vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
//  surface_filter->SetInputConnection(id_filter->GetOutputPort());
//  surface_filter->PassThroughCellIdsOn();
//  surface_filter->Update();
  
//  vtkSmartPointer<vtkHardwareSelector> select =
//      vtkSmartPointer<vtkHardwareSelector>::New();
//  select->SetRenderer(data_set_->R(win_flag_));
  double rect[4];
  rect[0] = data_set_->R(win_flag_)->GetPickX1();
  rect[1] = data_set_->R(win_flag_)->GetPickY1();
  rect[2] = data_set_->R(win_flag_)->GetPickX2();
  rect[3] = data_set_->R(win_flag_)->GetPickY2();
  std::cout << "2----- : " << rect[0] << " " << rect[1] << " " <<
               rect[2] << " " << rect[3] << std::endl;
//  select->SetArea(static_cast<int>(rect[0]), static_cast<int>(rect[1]),
//      static_cast<int>(rect[2]), static_cast<int>(rect[3]));
//  select->SetFieldAssociation(vtkDataObject::FIELD_ASSOCIATION_CELLS);
//  vtkSmartPointer<vtkSelection> sel_res =
//      vtkSmartPointer<vtkSelection>::New();
//  sel_res = select->Select();
//  if(!sel_res)
//  {
//    std::cerr << "[#Error]Selection not support" << std::endl;
//    return;
//  }
//  cout << "[#Info]The number of selected nodes: " << sel_res->GetNumberOfNodes() << endl;
//  vtkSmartPointer<vtkSelectionNode> cell_ids = 
//      vtkSmartPointer<vtkSelectionNode>::New();
//  cell_ids = sel_res->GetNode(0);
//  if(cell_ids)
//  {
//    cell_ids->SetFieldType(vtkSelectionNode::CELL);
//    cell_ids->SetContentType(vtkSelectionNode::INDICES);
//    vtkSmartPointer<vtkExtractSelectedPolyDataIds> extr = 
//        vtkSmartPointer<vtkExtractSelectedPolyDataIds>::New();
//    extr->SetInput(0, surface_filter->GetOutput());
//    vtkSmartPointer<vtkSelection> temp = 
//        vtkSmartPointer<vtkSelection>::New();
//    temp->AddNode(cell_ids);
//    extr->SetInput(1, temp);
//    extr->Update();
//    selected_mapper_->SetInput(extr->GetOutput());
//    selected_mapper_->ScalarVisibilityOff();
//    vtkSmartPointer<vtkDataSetSurfaceFilter> temp_sf =
//        vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
//    temp_sf->SetInputConnection(extr->GetOutputPort());
//    temp_sf->PassThroughCellIdsOn();
//    temp_sf->Update();
//    vtkPolyData* selected = temp_sf->GetOutput();
//    std::cout << "Selected " << selected->GetNumberOfPoints() << " points." << std::endl;
//    std::cout << "Selected " << selected->GetNumberOfCells() << " cells." << std::endl;
//    selected_actor_->SetMapper(selected_mapper_);
//    selected_actor_->GetProperty()->EdgeVisibilityOn();
//    selected_actor_->GetProperty()->SetEdgeColor(0, 0, 1);
//    selected_actor_->GetProperty()->SetLineWidth(3);
//    data_set_->R(win_flag_)->AddActor(selected_actor_);
//  }
//  else 
//  {
    
//  }
}

}
}
