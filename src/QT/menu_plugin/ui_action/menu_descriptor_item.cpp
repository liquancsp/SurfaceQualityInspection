#include <menu_descriptor_item.h>
#include <vtk_data/vtk_model_data.h>
#include <widget_plugin/ui_action/HeightMapRadiusRatioWidget.h>
#include <thread/InspectAlgThread.h>

#include <QtGui>

namespace lq {

using namespace std;
using namespace sqi::meshdata;
using namespace sqi::thread;
using namespace sqi::ui;

menu_descriptor_item::menu_descriptor_item(MeshDataPtr mesh, 
                                           vtkSmartPointer<vtk_model_data> data,
                                           QWidget *PARENT) :
  descriptor_menu_handle_(new Ui::ui_menu_descriptor_item()),
  ratio_input_widget_handle_(new HeightMapRadiusRatioWidget()),
  mesh_data_(mesh), data_set_(data), 
  inspect_thread_ptr_(new InspectAlgThread(mesh, data)),
  alg_type_(lq::kNull)
{
  this->descriptor_menu_handle_->setup_ui(this);
  CreateAction();
  qRegisterMetaType<lq::InspectFlag>("lq::InspectFlag");
}

menu_descriptor_item::~menu_descriptor_item()
{
  
}

Ui::ui_menu_descriptor_item* menu_descriptor_item::GetUiHandle()
{
  return descriptor_menu_handle_.get();
}

void menu_descriptor_item::SlotAwakeMeanZernikerAction()
{
  alg_type_ = lq::kCalMeanZernikerCoeff;
  ratio_input_widget_handle_->exec();
}

void menu_descriptor_item::SlotAwakeGaussZernikerAction()
{
  alg_type_ = lq::kCalGaussZernikerCoeff;
  ratio_input_widget_handle_->exec();
}

void menu_descriptor_item::SlotAwakeK1ZernikerAction()
{
  alg_type_ = lq::kCalK1ZernikerCoeff;
  ratio_input_widget_handle_->exec();
}

void menu_descriptor_item::SlotAwakeK2ZernikerAction()
{
  alg_type_ = lq::kCalK2ZernikerCoeff;
  ratio_input_widget_handle_->exec();
}

void menu_descriptor_item::SlotAwakeCurvatureZernikerAction()
{
  alg_type_ = lq::kCalCurvatureZernikerCoeff;
  ratio_input_widget_handle_->exec();
}

void menu_descriptor_item::SlotAwakeCurvatureDescriptorAction()
{
  alg_type_ = lq::kCalCurvatureDescriptor;
  ratio_input_widget_handle_->exec();
}

void menu_descriptor_item::SlotAwakeCurvatureHistogramAction()
{
  alg_type_ = lq::kCalCurvatureHistogram;
  ratio_input_widget_handle_->exec();
}

void menu_descriptor_item::SlotAwakeSmoothCurvatureDiffAction()
{
  alg_type_ = lq::kCalSmoothCurvatureDiff;
  ratio_input_widget_handle_->exec();
}

void menu_descriptor_item::SlotMeanZernikerAction(double multiplies, double ratio, int step)
{
  mesh_data_->RadiusRatio() = ratio;
  mesh_data_->HeightRadiusMulti() = multiplies;
  mesh_data_->SmoothStep() = step;
  cout << "[#Info]Multipiler : " << multiplies << " Ratio : " << ratio << 
          " Step : " << step << endl;
  ExecuteAlg();
  ratio_input_widget_handle_->hide();
}

void menu_descriptor_item::SlotCompleteAligorithm(InspectFlag flag)
{
  emit SignalCompleteInspect();
  emit SignalActionDone(COMPLETE_ALGORITHM[flag]);
}

void menu_descriptor_item::ExecuteAlg()
{
  emit SignalProgressRange(0, 0);
  emit SignalBeginAlgAction();
  inspect_thread_ptr_->SetThreadType(InspectAlgThread::kDescriptor);
  inspect_thread_ptr_->SetAlgType(alg_type_);
  inspect_thread_ptr_->start();
}


void menu_descriptor_item::CreateAction()
{
  
  connect(this->descriptor_menu_handle_->MeanZernikerAction(), SIGNAL(triggered()),
          this, SLOT(SlotAwakeMeanZernikerAction()));
  connect(this->descriptor_menu_handle_->GaussZernikerAction(), SIGNAL(triggered()),
          this, SLOT(SlotAwakeGaussZernikerAction()));
  connect(this->descriptor_menu_handle_->K1ZernikerAction(), SIGNAL(triggered()),
          this, SLOT(SlotAwakeK1ZernikerAction()));
  connect(this->descriptor_menu_handle_->K2ZernikerAction(), SIGNAL(triggered()),
          this, SLOT(SlotAwakeK2ZernikerAction()));
  connect(this->descriptor_menu_handle_->CurvatureZernikerAction(), SIGNAL(triggered()),
          this, SLOT(SlotAwakeCurvatureZernikerAction()));
  connect(this->descriptor_menu_handle_->CurvatureDescriptorAction(), SIGNAL(triggered()),
          this, SLOT(SlotAwakeCurvatureDescriptorAction()));
  connect(this->descriptor_menu_handle_->CurvatureHistogramAction(), SIGNAL(triggered()),
          this, SLOT(SlotAwakeCurvatureHistogramAction()));
  connect(this->descriptor_menu_handle_->SmoothCurvatureDiffAction(), SIGNAL(triggered()),
          this, SLOT(SlotAwakeSmoothCurvatureDiffAction()));
  
  connect(this->ratio_input_widget_handle_.get(), SIGNAL(SignalRadiusRatio(double, double, int)),
          this, SLOT(SlotMeanZernikerAction(double, double, int)));

  
  connect(this->inspect_thread_ptr_.get(), SIGNAL(SignalCompleteAlgorithm(lq::InspectFlag)),
          this, SLOT(SlotCompleteAligorithm(lq::InspectFlag)));
}



}

