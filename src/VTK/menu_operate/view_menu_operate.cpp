#include <menu_operate/view_menu_operate.h>
#include <vtk_data/vtk_model_data.h>

#include <vtkCellArray.h>
#include <vtkLegendScaleActor.h>
#include <vtkScalarBarActor.h>
#include <vtkLookupTable.h>

namespace lq
{
using namespace sqi::meshdata;

ViewMenuOperate::ViewMenuOperate() : legend_scale_actor_(vtkSmartPointer<vtkLegendScaleActor>::New())
{
  legend_scale_actor_ = vtkSmartPointer<vtkLegendScaleActor>::New();
  lut_ = vtkSmartPointer<vtkLookupTable>::New();
  scalar_bar_ = vtkSmartPointer<vtkScalarBarActor>::New();
}

ViewMenuOperate::~ViewMenuOperate()
{
  
}

void ViewMenuOperate::SetDataSet(const vtkSmartPointer<vtk_model_data> &other_data_set)
{
  this->data_set_ = other_data_set;
}

void ViewMenuOperate::AddColorLegend(lq::WindowFlag win)
{
  
  assert(mesh_data_.use_count() > 0);
  double M;
  if(lq::is_larger(fabs(mesh_data_->MaxE(win)), fabs(mesh_data_->MinE(win))))
    M = fabs(mesh_data_->MaxE(win));
  else
    M = fabs(mesh_data_->MinE(win));
  if(mesh_data_->ScalarType() == sqi::meshdata::kIndependent)
    data_set_->LookupTable(win)->SetTableRange(0 - M, M);
  else if(mesh_data_->ScalarType() == sqi::meshdata::kUniform)
    data_set_->LookupTable(win)->SetTableRange(mesh_data_->MinColor(), mesh_data_->MaxColor());
  data_set_->LookupTable(win)->SetNumberOfTableValues(mesh_data_->ColorNum());
  
  for(size_t i = 0; i < mesh_data_->ColorNum(); ++i)
  {
    data_set_->LookupTable(win)->SetTableValue(i, double(mesh_data_->StdColor()[i].R) / 255.0f,
                                               double(mesh_data_->StdColor()[i].G) / 255.0f,
                                               double(mesh_data_->StdColor()[i].B) / 255.0f,
                                               mesh_data_->StdColor()[i].Scalar);
  }
  ViewMenuOperate::ResizeColorLegend(win);
  if(!data_set_->IsHasScalarBar(win))
  {
    data_set_->ScalarActor(win)->SetTitle("Color Legend");
    data_set_->ScalarActor(win)->SetNumberOfLabels(7);
    data_set_->ScalarActor(win)->SetOrientationToHorizontal();
  //data_set_->ScalarActor()->SetOrientationToVertical();
//  data_set_->ScalarWidget()->SetScalarBarActor(data_set_->ScalarActor());
//  data_set_->ScalarWidget()->ResizableOff();
//  data_set_->ScalarWidget()->SetInteractor(data_set_->QTW()->GetInteractor());
//  data_set_->ScalarWidget()->On();
  
    data_set_->ScalarActor(win)->SetLookupTable(data_set_->LookupTable(win));
    data_set_->R(win)->AddActor(data_set_->ScalarActor(win));
    data_set_->IsHasScalarBar(win) = true;
  }
}

void ViewMenuOperate::AddClassifyColorLengend(WindowFlag win)
{
  assert(mesh_data_.use_count() > 0);
  data_set_->LookupTable(win)->SetTableRange(1, mesh_data_->StdColor().size());
  data_set_->LookupTable(win)->SetNumberOfTableValues(mesh_data_->ColorNum());
  for(size_t i = 0; i < mesh_data_->ColorNum(); ++i)
  {
    data_set_->LookupTable(win)->SetTableValue(i, double(mesh_data_->StdColor()[i].R) / 255.0f,
                                               double(mesh_data_->StdColor()[i].G) / 255.0f,
                                               double(mesh_data_->StdColor()[i].B) / 255.0f,
                                               mesh_data_->StdColor()[i].Scalar);
  }
  ViewMenuOperate::ResizeColorLegend(win);
  //  if(!data_set_->IsHasScalarBar(win))
  //  {
  data_set_->ScalarActor(win)->SetTitle("Color Legend");
  data_set_->ScalarActor(win)->SetNumberOfLabels(mesh_data_->StdColor().size());
  data_set_->ScalarActor(win)->SetOrientationToHorizontal();
  
  data_set_->ScalarActor(win)->SetLookupTable(data_set_->LookupTable(win));
  data_set_->R(win)->AddActor(data_set_->ScalarActor(win));
  data_set_->IsHasScalarBar(win) = true;
  //  }
}

void ViewMenuOperate::ClearColorLegend(lq::WindowFlag win)
{
  if(data_set_->IsHasScalarBar(win))
  {
    data_set_->LookupTable(win)->SetNumberOfTableValues(mesh_data_->ColorNum());
    data_set_->LookupTable(win)->SetTableRange(0, 0);
    for(size_t i = 0; i < mesh_data_->ColorNum(); ++i)
    {
      data_set_->LookupTable(win)->SetTableValue(i, 1.0f, 0.0f, 1.0f, 1.0f);
    }
    data_set_->ScalarActor(win)->SetTitle("Color Legend");
    data_set_->ScalarActor(win)->SetNumberOfLabels(6);
    ViewMenuOperate::ResizeColorLegend(win);
  }
}

//set color legend in the middle of the render windows with 20% distance to the margin
void ViewMenuOperate::ResizeColorLegend(lq::WindowFlag win)
{
  int* win_size = data_set_->RenderWinSize();
  int color_bar_max_width = (win_size[0] / 2 - win_size[0] * 0.1);
  data_set_->ScalarActor(win)->SetMaximumWidthInPixels(color_bar_max_width);
  data_set_->ScalarActor(win)->SetMaximumHeightInPixels(50);
  data_set_->ScalarActor(win)->SetDisplayPosition(static_cast<int>(win_size[0] * (win + 0.1) / 2), 0);
  double color_item_width = static_cast<double>(static_cast<double>(color_bar_max_width) / 257.00);
  data_set_->ScalarActor(win)->SetWidth(color_item_width);
  data_set_->QTW()->update();
}

void ViewMenuOperate::LinkCamera()
{
  
}

void ViewMenuOperate::ResetScene()
{
  data_set_->CM()->DeepCopy(data_set_->InitialCamera());
  for(size_t i = 0; i < 2; ++i)
    data_set_->R(i)->ResetCamera();
}


void ViewMenuOperate::ShowBackSide()
{
//  ViewMenuOperate::ResetScene();
//  data_set_->Actor(0)->RotateX(180.0);
//  data_set_->Actor(1)->RotateX(180.0);
//  ViewMenuOperate::ResetScene();
//  data_set_->QTW()->update();
//  for(size_t i = 0; i < 2; ++i)
//    mesh_data_->IsBackSide(i)() = !mesh_data_->IsBackSide(i)();
  
}


void ViewMenuOperate::FlipModelNormal(lq::WindowFlag flag)
{
  vtkIdType pts[3];
  Mesh::FaceIterator fi = mesh_data_->FBeginIter(flag);
  vtkSmartPointer<vtkCellArray> triangle_cell =
      vtkSmartPointer<vtkCellArray>::New();
  data_set_->PD(flag)->GetPolys()->Initialize();
  for(fi = mesh_data_->M(flag).face.begin(); fi != mesh_data_->M(flag).face.end(); ++fi)
  {
    std::swap((*fi).V(1), (*fi).V(2));
    pts[0] =  mesh_data_->GetIndexVertex((*fi).V(0), flag);
    pts[1] =  mesh_data_->GetIndexVertex((*fi).V(1), flag);
    pts[2] =  mesh_data_->GetIndexVertex((*fi).V(2), flag);
    triangle_cell->InsertNextCell(3, pts);
  }
  data_set_->PD(flag)->SetPolys(triangle_cell);
  data_set_->QTW()->update();
  mesh_data_->UpdateMesh(flag);
  mesh_data_->ResetFlip(flag);
}


}
