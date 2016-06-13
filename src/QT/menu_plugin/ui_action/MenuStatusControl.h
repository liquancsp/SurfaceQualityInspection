#ifndef MENUSTATUSCONTROL_H
#define MENUSTATUSCONTROL_H

#include <MeshData.h>
#include <vtkSmartPointer.h>

namespace lq{

class vtk_model_data;
class menu_inspect_item;
class menu_view_item;

class MenuStatusControl {
public:
  MenuStatusControl(){}
 
  ~MenuStatusControl(){}
  
  void SetInspectMenu(std::tr1::shared_ptr<menu_inspect_item> inspectg_item)
  {
    this->inspect_item_handle_ = inspectg_item;
  }
  
  void SetViewMenu(std::tr1::shared_ptr<menu_view_item> view_item)
  {
    this->view_item_handle_ = view_item;
  }
  
  
  void InvalidInspectMenu();
  void ResumeInspectMenu(bool flag);
  void InvalidViewMenu();
  void ResumeViewMenu();

  void InvalidMenuStatus();
  void ResumeMenuStatus(bool flag);
  
private:
  
  std::tr1::shared_ptr<menu_inspect_item> inspect_item_handle_;
  std::tr1::shared_ptr<menu_view_item> view_item_handle_;
  
};

}

#endif // MENUSTATUSCONTROL_H
