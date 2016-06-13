#ifndef VIEW_POINT_OPERATE_H
#define VIEW_POINT_OPERATE_H
#include <vtk_data/vtk_model_data.h>

#include <vtkSphereWidget.h>
#include <vtkTextWidget.h>
namespace lq
{
  class viewpoint_callback;
  
  class view_point_operate
  {
 public:
    
    view_point_operate();
    void set_data_set(const vtkSmartPointer<vtk_model_data> &other_data_set);
    void display_view_point();
    
 private:

    vtkSmartPointer<vtk_model_data> data_set_;
    vtkSmartPointer<viewpoint_callback> view_point_callback_;
    void create_sphere(int radius);
    void create_text_widget();

  };
}
#endif
