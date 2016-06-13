#ifndef MENU_DESCRIPTOR_ITEM_H
#define MENU_DESCRIPTOR_ITEM_H
#include <menu_plugin/ui_resource/ui_menu_descriptor_item.h>
#include <configure/configure.h>

#include <MeshData.h>
#include <vtkSmartPointer.h>

namespace sqi {
namespace thread{
class InspectAlgThread;
}
}

namespace sqi{
namespace ui{
class HeightMapRadiusRatioWidget;
}
}

namespace lq {
class vtk_model_data;
class EditMenuOperate;



class menu_descriptor_item : public QMenu{
  Q_OBJECT
public:
  
  menu_descriptor_item(sqi::meshdata::MeshDataPtr mesh,
                       vtkSmartPointer<lq::vtk_model_data> data, QWidget* PARENT = 0);
  ~menu_descriptor_item();
  
  void SetDataSet(const vtkSmartPointer<lq::vtk_model_data>& other)
  {
    this->data_set_ = other;
  }
  
  void SetMeshData(const sqi::meshdata::MeshDataPtr& other)
  {
    this->mesh_data_ = other;
  }
  
  Ui::ui_menu_descriptor_item* GetUiHandle();
  
signals:
  
  void SignalProgressRange(int begin, int end);
  void SignalBeginAlgAction();
  void SignalCompleteInspect();
  void SignalActionDone(const char* str);
  
public slots:
  
  void SlotAwakeMeanZernikerAction();
  void SlotAwakeGaussZernikerAction();
  void SlotAwakeK1ZernikerAction();
  void SlotAwakeK2ZernikerAction();
  void SlotAwakeCurvatureZernikerAction();
  void SlotAwakeCurvatureDescriptorAction();
  void SlotAwakeCurvatureHistogramAction();
  void SlotAwakeSmoothCurvatureDiffAction();
  
  void SlotMeanZernikerAction(double multiplies, double ratio, int step);
//  void Slot
  void SlotCompleteAligorithm(lq::InspectFlag flag);
  
private:
  
  void CreateAction();
  void ExecuteAlg();
  std::tr1::shared_ptr<Ui::ui_menu_descriptor_item> descriptor_menu_handle_;
  std::tr1::shared_ptr<sqi::ui::HeightMapRadiusRatioWidget> ratio_input_widget_handle_;
  sqi::meshdata::MeshDataPtr mesh_data_; 
  vtkSmartPointer<lq::vtk_model_data> data_set_;
  std::tr1::shared_ptr<sqi::thread::InspectAlgThread> inspect_thread_ptr_;
  lq::InspectFlag alg_type_;
  
};
}

#endif // MENU_DESCRIPTOR_ITEM_H
