#include "view_point_operate.h"
#include <callback/viewpoint_callback.h>
#include <vtkTextProperty.h>
#include <vtkTextRepresentation.h>
namespace lq
{
view_point_operate::view_point_operate()
{
  std::cout << "[#info]Create view_point_operate" << std::endl;
  view_point_callback_ = vtkSmartPointer<viewpoint_callback>::New();
}

void view_point_operate::set_data_set(const vtkSmartPointer<vtk_model_data> &other_data_set)
{
  this->data_set_ = other_data_set;
  view_point_callback_->set_data_set(data_set_);
}

void view_point_operate::display_view_point()
{
  //std::cout << "[#info] Begin display_view_point" << std::endl;
  //create_sphere(5.0);
  create_text_widget();
  data_set_->QTW()->update();
}

void view_point_operate::create_text_widget()
{
  std::cout << "[#info]Begin create_text_widget." << std::endl;

  point view;
  char view_point_ch[VIEW_POINT_CHAR_SIZE];
  view = data_set_->ViewPoint();
  sprintf(view_point_ch, "View Point is:(%.2lf, %.2lf, %.2lf)",view.x, view.y, view.z);
  data_set_->ViewPosTextActor()->SetInput(view_point_ch);
  //if view point text widget is not created, create it. Otherwise just display it
  if(!data_set_->IsHasViewTextWidget())
  {
    data_set_->ViewPosTextActor()->GetTextProperty()->SetColor(1.0, 1.0, 1.0);
    vtkSmartPointer<vtkTextRepresentation> text_representation =
        vtkSmartPointer<vtkTextRepresentation>::New();
    text_representation->GetPositionCoordinate()->SetValue(.10, .10);
    text_representation->GetPosition2Coordinate()->SetValue(.5, .1);
    text_representation->SetMaximumSize(100, 40);
    text_representation->ProportionalResizeOff();
    data_set_->ViewTextWidget()->SetRepresentation(text_representation);
    data_set_->ViewTextWidget()->ResizableOff();
    data_set_->ViewTextWidget()->SetInteractor(data_set_->QTW()->GetInteractor());
    data_set_->ViewTextWidget()->SetTextActor(data_set_->ViewPosTextActor());    
    data_set_->ViewTextWidget()->SelectableOff();
    data_set_->IsHasViewTextWidget() = true;
  }
  data_set_->ViewTextWidget()->On();
  std::cout << "[#info]Completed create_text_widget." << std::endl;
}

void view_point_operate::create_sphere(int radius)
{
  point center;
  center = data_set_->ViewPoint();
  if(!data_set_->IsHasSphereWidget())
  {
    data_set_->ViewSphereWidget() = vtkSmartPointer<vtkSphereWidget>::New();
    //data_set_->view_point_sphere_widget_->SetEnabled(false);
    data_set_->ViewSphereWidget()->SetInteractor(data_set_->QTW()->GetInteractor());
    data_set_->ViewSphereWidget()->SetRepresentationToSurface();
    data_set_->ViewSphereWidget()->SetCenter(center.x, center.y, center.z);
    data_set_->ViewSphereWidget()->SetRadius(radius);
    data_set_->ViewSphereWidget()->AddObserver(vtkCommand::InteractionEvent,view_point_callback_);
    data_set_->IsHasSphereWidget() = true;
  }
  //if the view point sphere widget have been created, just reset the position of view point sphere widget
  data_set_->ViewSphereWidget()->SetCenter(center.x, center.y, center.z);
  data_set_->ViewSphereWidget()->SetRadius(radius);
  data_set_->ViewSphereWidget()->On();
}

}
