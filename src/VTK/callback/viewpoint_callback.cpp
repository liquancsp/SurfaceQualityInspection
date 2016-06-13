#include "viewpoint_callback.h"
#include <vtkSphereWidget.h>

namespace lq
{

viewpoint_callback::viewpoint_callback()
{
  
}

void viewpoint_callback::set_data_set(const vtkSmartPointer<vtk_model_data> &other_data_set)
{
  this->data_set_ = other_data_set;
  //edit_operate_.set_data_set(data_set_);
}


void viewpoint_callback::Execute(vtkObject *obj, unsigned long, void *cell_data)
{
//  std::cout << "[#info]Execute view point callback" << " file: " << __FILE__
//            << " line: " << __LINE__ << std::endl;
//  vtkSphereWidget *sphereWidget = 
//      reinterpret_cast<vtkSphereWidget*>(obj);

//  double center[3];
//  char view_point_ch[VIEW_POINT_CHAR_SIZE];
//  sphereWidget->GetCenter(center);
//  point view_point(center);
//  data_set_->ViewPoint() = view_point;
//  edit_operate_.ComputeError(kReflectionMap);
//  sphereWidget->GetCenter(center);
//  sprintf(view_point_ch, "View Point is:(%.2lf, %.2lf, %.2lf )", center[0], center[1], center[2]);
//  data_set_->ViewPosTextActor()->SetInput(view_point_ch);

//  if(edit_operate_.init_color_list() == kColorInitFail)
//  {
//    return ;
//  }
//	edit_operate_.map_error_color();
//	edit_operate_.clear_poly_color();
//	edit_operate_.set_color_on_poly();
//	data_set_->QTW()->update();
//  //edit_menu_handle_->load_color_action->setEnabled(true);
//  std::cout << "Center: " << center[0] << " " << center[1] << " " << center[2] << std::endl;
}

}
