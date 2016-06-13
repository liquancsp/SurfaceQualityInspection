#ifndef MENU_CLASSIFY_ITEM_H
#define MENU_CLASSIFY_ITEM_H

#include <menu_plugin/ui_resource/ui_menu_classification.h>
#include <MeshData.h>
#include <vtkSmartPointer.h>


namespace sqi {
namespace thread {
class InspectAlgThread;
}
}

namespace sqi {
namespace ui {

}
}

namespace lq {
class vtk_model_data;

class menu_classify_item : public QMenu {
  Q_OBJECT
  
public:
  
  menu_classify_item(sqi::meshdata::MeshDataPtr mesh,
                     vtkSmartPointer<lq::vtk_model_data> data, QWidget* PARENT = 0);
  
  ~menu_classify_item();
  void SetDataSet(const vtkSmartPointer<lq::vtk_model_data>& other)
  {
    this->data_set_ = other;
  }
  
  void SetMeshData(const sqi::meshdata::MeshDataPtr& other)
  {
    this->mesh_data_ = other;
  }
  
  Ui::ui_menu_classification* GetUiHandle();

public slots:

  void SlotTrainAndPredictAction();
  void SlotClassifyAction();
signals:
  void SignalProgressRange(int begin, int end);
  void SignalBeginAlgAction();
private :
  
  void CreateAction();
  void ExecuteAlg();
  std::tr1::shared_ptr<Ui::ui_menu_classification> classify_menu_handle_;
  sqi::meshdata::MeshDataPtr mesh_data_; 
  vtkSmartPointer<lq::vtk_model_data> data_set_;
  std::tr1::shared_ptr<sqi::thread::InspectAlgThread> inspect_thread_ptr_;
  lq::InspectFlag alg_type_;
  
};

}



#endif // MENU_CLASSIFY_ITEM_H
