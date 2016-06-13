#include <menu_classify_item.h>
#include <vtk_data/vtk_model_data.h>
#include <thread/InspectAlgThread.h>
#include <configure/configure.h>

#include <QtGui>

namespace lq {
using namespace std;
using namespace sqi::meshdata;
using namespace sqi::ui;
using namespace sqi::thread;

menu_classify_item::menu_classify_item(MeshDataPtr mesh, 
                                       vtkSmartPointer<vtk_model_data> data, 
                                       QWidget *PARENT) : 
  classify_menu_handle_(new Ui::ui_menu_classification()),
  mesh_data_(mesh), data_set_(data),
  inspect_thread_ptr_(new InspectAlgThread(mesh, data)),
  alg_type_(lq::kNull)
{
  this->classify_menu_handle_->setup_ui(this);
  CreateAction();
  qRegisterMetaType<lq::InspectFlag>("lq::InspectFlag");
}

menu_classify_item::~menu_classify_item()
{
  
}

Ui::ui_menu_classification* menu_classify_item::GetUiHandle()
{
  return classify_menu_handle_.get();
}

void menu_classify_item::SlotTrainAndPredictAction()
{
  alg_type_ = lq::kTrainAndPredict;
  if(mesh_data_->IsWinHas(kLeft) && mesh_data_->IsWinHas(kRight))
  {
    int ret = QMessageBox::warning(this, tr(WARNNING_TITLE),
                                   tr(NO_WIN_DISPLAY),
                                   QMessageBox::Cancel);
    return ;
  }
  QString file_name = QFileDialog::getOpenFileName(this,
                                                   tr("Open File"), ".",
                                                   tr("Python file (*.py)"));
  
  mesh_data_->PythonFilePath() = qPrintable(file_name);
  ExecuteAlg();
}

void menu_classify_item::SlotClassifyAction()
{
  alg_type_ = lq::kClassify;
  if(mesh_data_->IsWinHas(kLeft) && mesh_data_->IsWinHas(kRight))
  {
    int ret = QMessageBox::warning(this, tr(WARNNING_TITLE),
                                   tr(NO_WIN_DISPLAY),
                                   QMessageBox::Cancel);
    return ;
  }
  QString file_name = QFileDialog::getOpenFileName(this,
                                                   tr("Open File"), ".",
                                                   tr("Predict files (*.txt *.pred)"));
  
  mesh_data_->PredictFile() = qPrintable(file_name);
  ExecuteAlg();
}

void menu_classify_item::CreateAction()
{
  connect(this->classify_menu_handle_->TrainPredictAction(), SIGNAL(triggered()),
          this, SLOT(SlotTrainAndPredictAction()));
  connect(this->classify_menu_handle_->ClassifyAction(), SIGNAL(triggered()),
          this, SLOT(SlotClassifyAction()));
}

void menu_classify_item::ExecuteAlg()
{
  emit SignalProgressRange(0, 0);
  emit SignalBeginAlgAction();
  inspect_thread_ptr_->SetAlgModel(mesh_data_->AlorithmModel());
  inspect_thread_ptr_->SetAlgType(alg_type_);
  inspect_thread_ptr_->SetThreadType(InspectAlgThread::kClassify);
  inspect_thread_ptr_->start();
}

}
