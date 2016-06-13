#ifndef MODELNAMETEXTACTOR_H
#define MODELNAMETEXTACTOR_H

#include <vtk_data/vtk_model_data.h>
#include <data_type.h>

namespace sqi {

namespace WidgetOp {

template<typename DataSet>
class ModelNameTextActor 
{
public:
  
  ModelNameTextActor(){}
  ModelNameTextActor(const vtkSmartPointer<DataSet> &other): 
    data_set_(other)
  {}
  ~ModelNameTextActor(){}
  
  void set_data_set(const vtkSmartPointer<DataSet>& other)
  {
   this->data_set_ = other;
  }
  
  void update_model_name_text_actor()
  {
    set_model_name_text_actor(lq::kLeft);
    set_model_name_text_actor(lq::kRight);
  }
  
  void set_model_name_text_actor(int flag)
  {
    int* pos = data_set_->RenderWinSize();
    data_set_->ModelNameTextActor(flag)->SetPosition(0, pos[1] - 20);
    data_set_->ModelNameTextActor(flag)->SetWidth(150);
    data_set_->ModelNameTextActor(flag)->SetHeight(20);
    std::string tmp_name;
    lq::get_model_name(data_set_->ModelName(flag), tmp_name);
    data_set_->ModelNameTextActor(flag)->SetInput(tmp_name.c_str());
    data_set_->ModelNameTextActor(flag)->VisibilityOn();
    data_set_->R(flag)->AddActor(data_set_->ModelNameTextActor(flag));
  }
  
  void clear_two_text_actor()
  {
    clear_model_name_text_actor(lq::kLeft);
    clear_model_name_text_actor(lq::kRight);
  }
  
  void clear_model_name_text_actor(lq::WindowFlag flag)
  {
    //data_set_->ModelNameTextActor(flag)->SetInput(" ");
    data_set_->ModelNameTextActor(flag)->VisibilityOff();
  }
  
private:
  
  vtkSmartPointer<DataSet> data_set_;
  
};

}

}

#endif // MODELNAMETEXTACTOR_H
