#include <menu_plugin/ui_action/menu_inspect_item.h>
#include <configure/configure.h>
#include <thread/InspectAlgThread.h>
#include <vtk_data/vtk_model_data.h>
#include <menu_operate/edit_menu_operate.h>
#include <widget_plugin/ui_action/AlignRadiusInputWidget.h>
#include <widget_plugin/ui_action/HeightMapRadiusRatioWidget.h>
#include <QtGui>


namespace lq {

using namespace lq;
using namespace sqi::meshdata;
using namespace sqi::common;
using namespace sqi::thread;
using namespace sqi::ui;
//#if defined(_MSC_VER)
//menu_inspect_item::menu_inspect_item(sqi::meshdata::MeshDataPtr mesh,
//                                     vtkSmartPointer<lq::vtk_model_data> data,
//                                     QWidget* PARENT) : mesh_data_(mesh),
//  data_set_(data), inspect_director_ptr_(new InspectActionDirector(mesh, data))
//#else if defined (__GNUC__)
menu_inspect_item::menu_inspect_item(sqi::meshdata::MeshDataPtr mesh, 
                                     vtkSmartPointer<lq::vtk_model_data> data,
                                     QWidget* PARENT) :
  radius_flag_(kNull),
  inspect_menu_handle_(new Ui::ui_menu_inspect_item()),
  radius_input_widget_handle_(new AlignRadiusInputWidget()),
  ratio_input_widget_handle_(new HeightMapRadiusRatioWidget()),
  mesh_data_(mesh),  data_set_(data),
  inspect_thread_ptr_(new InspectAlgThread(mesh, data))
//#endif
{
  this->inspect_menu_handle_->setup_ui(this);
  create_action();
  qRegisterMetaType<lq::InspectFlag>("lq::InspectFlag");
}

menu_inspect_item::~menu_inspect_item()
{
  
}

Ui::ui_menu_inspect_item* menu_inspect_item::GetUiHandle()
{
  return inspect_menu_handle_.get();
}

void menu_inspect_item::slot_algorithm_model_action()
{
  mesh_data_->AlorithmModel() = static_cast<lq::AlgType>
      (static_cast<int>(mesh_data_->AlorithmModel()) ^ 
       static_cast<int>(lq::kCalTwoDiff));
  slot_reset_checkable_action();
  inspect_thread_ptr_->SetThreadType(InspectAlgThread::kSwitchModel);
  inspect_thread_ptr_->start();
  inspect_thread_ptr_->exit();
  inspect_menu_handle_->SetInspectActionEable(mesh_data_->AlorithmModel());
}

void menu_inspect_item::slot_reflection_map_action()
{
  ExecuteAlg(kReflectionMap, mesh_data_->AlorithmModel());
}

void menu_inspect_item::slot_integrate_reflection_map_action()
{
  ExecuteAlg(kIntegrateReflection, mesh_data_->AlorithmModel());
}

void menu_inspect_item::slot_global_align_integral_refl_action()
{
  
  ExecuteAlg(kGlobalAlignInteRefl, mesh_data_->AlorithmModel());
}

void menu_inspect_item::slot_global_align_normal_component_action()
{
  ExecuteAlg(kGlobalAlignNormalComponent, mesh_data_->AlorithmModel());
}

void menu_inspect_item::slot_local_align_integral_refl_action()
{
  radius_flag_ = kLocalAlignIntegralRefl;
  radius_input_widget_handle_->exec();
}

void menu_inspect_item::slot_local_align_normal_division_action()
{
  radius_flag_ = kLocalAlignNormalDivision;
  radius_input_widget_handle_->exec();
}

void menu_inspect_item::slot_exec_local_align_alg_action(double radius)
{
  if(mesh_data_->IsFirstInputRadius())
  {
    mesh_data_->LocalRadius() = radius;
    mesh_data_->IsFirstInputRadius() = false;
  }
  else 
  {
    if(!equal(mesh_data_->PreLocalRadius(), radius, EPS_E7))
      mesh_data_->PreLocalRadius() = mesh_data_->LocalRadius();
    mesh_data_->LocalRadius() = radius;
    
  }
  ExecuteAlg(radius_flag_, mesh_data_->AlorithmModel());
  radius_input_widget_handle_->hide();
}

void menu_inspect_item::slot_least_square_integral_refl_action()
{
  ExecuteAlg(kGlobalLSIntegralRefl, mesh_data_->AlorithmModel());
}

void menu_inspect_item::slot_least_square_normal_component_action()
{
  ExecuteAlg(kGlobalLSNormalComponent, mesh_data_->AlorithmModel());
}

void menu_inspect_item::slot_convex_diff_action()
{
  ExecuteAlg(kConvexDiff, mesh_data_->AlorithmModel());
}

void menu_inspect_item::slot_mix_refl_convex_action()
{
  ExecuteAlg(kMixReflConvex, mesh_data_->AlorithmModel());
}

void menu_inspect_item::slot_mix_inte_convex_action()
{
  ExecuteAlg(kMixInteConvex, mesh_data_->AlorithmModel());
} 

void menu_inspect_item::slot_mean_curvature_action()
{
  ExecuteAlg(kMeanCurvature, mesh_data_->AlorithmModel());
}

void menu_inspect_item::slot_gaussian_curvature_action()
{
  ExecuteAlg(kGaussianCurvature, mesh_data_->AlorithmModel());
}

void menu_inspect_item::slot_principal_k1_action()
{
  ExecuteAlg(kPrincipalK1, mesh_data_->AlorithmModel());
}

void menu_inspect_item::slot_principal_k2_action()
{
  ExecuteAlg(kPrincipalK2, mesh_data_->AlorithmModel());
}

void menu_inspect_item::slot_distance_different_action()
{
  ExecuteAlg(kDistanceDifferent, mesh_data_->AlorithmModel());
}

void menu_inspect_item::slot_normal_vector_angle_action()
{
  ExecuteAlg(kNormalVectorAngle, mesh_data_->AlorithmModel());
}

void menu_inspect_item::slot_mix_mean_inte_action()
{
  ExecuteAlg(kMixMeanInte, mesh_data_->AlorithmModel());
}

void menu_inspect_item::slot_mix_gauss_inte_action()
{
  ExecuteAlg(kMixGaussInte, mesh_data_->AlorithmModel());
}

void menu_inspect_item::slot_mix_k1_inte_action()
{
  ExecuteAlg(kMixK1Inte, mesh_data_->AlorithmModel());
}

void menu_inspect_item::slot_mix_k2_inte_action()
{
  ExecuteAlg(kMixK2Inte, mesh_data_->AlorithmModel());
}

void menu_inspect_item::slot_defect_fitting_action()
{
  ExecuteAlg(kDefectFitting, mesh_data_->AlorithmModel());
}

void menu_inspect_item::slot_propagate_action()
{
  ExecuteAlg(kZernikePropagation, mesh_data_->AlorithmModel());
}

void menu_inspect_item::slot_exec_zerniker_action(double multipiler,
                                                  double ratio, int step)
{
  mesh_data_->RadiusRatio() = ratio;
  mesh_data_->HeightRadiusMulti() = multipiler;
  mesh_data_->SmoothStep() = step;
  cout << "[#Info]Multipiler : " << multipiler << " Ratio : " << ratio << endl;
  ExecuteAlg(radius_flag_, mesh_data_->AlorithmModel());
  ratio_input_widget_handle_->hide();
}

void menu_inspect_item::slot_zerniker_coeff_action()
{
  radius_flag_ = kCalcuateZernikerCoefficient;
  ratio_input_widget_handle_->exec();
}

void menu_inspect_item::slot_zerniker_classify_action()
{
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
  ExecuteAlg(kZernikerClassify, mesh_data_->AlorithmModel());
}

void menu_inspect_item::slot_execute_alg()
{
//  if(data_set_.GetPointer() != NULL)
//  {
//    lq::InspectFlag flag = data_set_->AlgType();
//    if(flag != lq::kNull)
//    {
//      inspect_director_ptr_->Execute(flag);
//    }
//  }
}

void menu_inspect_item::slot_reset_checkable_action()
{
  this->inspect_menu_handle_->ResetInspectCheckable();
}

void menu_inspect_item::SlotCompleteAligorithm(lq::InspectFlag flag)
{
  emit SignalCompleteInspect();
  emit SignalActionDone(COMPLETE_ALGORITHM[flag]);
}

void menu_inspect_item::ExecuteAlg(const InspectFlag alg_type, const AlgType alg_model)
{
  emit SignalProgressRange(0, 0);
  emit SignalBeginAlgAction();
  inspect_thread_ptr_->SetThreadType(InspectAlgThread::kAlg);
  inspect_thread_ptr_->SetAlgType(alg_type);
  inspect_thread_ptr_->SetAlgModel(alg_model);
  inspect_thread_ptr_->start();
}

//end of slots
//------------------------------------------------------------------------------
void menu_inspect_item::create_action()
{
  
  connect(this->inspect_menu_handle_->AlgorithmModel(), SIGNAL(triggered()),
          this, SLOT(slot_algorithm_model_action()));
  
  connect(this->inspect_menu_handle_->ReflectionMapAction(), SIGNAL(triggered()),
          this, SLOT(slot_reflection_map_action()));
  connect(this->inspect_menu_handle_->IntegrateReflectionAction(), SIGNAL(triggered()),
          this, SLOT(slot_integrate_reflection_map_action()));
  
  connect(this->inspect_menu_handle_->GlobalAlignInteMapAction(), SIGNAL(triggered()),
          this, SLOT(slot_global_align_integral_refl_action()));
  connect(this->inspect_menu_handle_->GlobalAlignNormalComAction(), SIGNAL(triggered()),
          this, SLOT(slot_global_align_normal_component_action()));
  
  connect(this->inspect_menu_handle_->LocalAlignIntegralReflAction(), SIGNAL(triggered()),
          this, SLOT(slot_local_align_integral_refl_action()));
  connect(this->inspect_menu_handle_->LocalAlignNormalDivisionAction(), SIGNAL(triggered()),
          this, SLOT(slot_local_align_normal_division_action()));
  connect(this->radius_input_widget_handle_.get(), SIGNAL(SignalAlignRadius(double)),
          this, SLOT(slot_exec_local_align_alg_action(double)));
  
  connect(this->inspect_menu_handle_->LeastSquareIntegralMapAction(), SIGNAL(triggered()),
          this, SLOT(slot_least_square_integral_refl_action()));
  connect(this->inspect_menu_handle_->LeastSquareNormalComponentAction(), SIGNAL(triggered()),
          this, SLOT(slot_least_square_normal_component_action()));
  
  connect(this->inspect_menu_handle_->ConvexDiffAction(), SIGNAL(triggered()),
          this, SLOT(slot_convex_diff_action()));
  connect(this->inspect_menu_handle_->MixReflConvexAction(), SIGNAL(triggered()),
          this, SLOT(slot_mix_refl_convex_action()));
  connect(this->inspect_menu_handle_->MixInteConvexAction(), SIGNAL(triggered()),
          this, SLOT(slot_mix_inte_convex_action()));
  
  
  connect(this->inspect_menu_handle_->MeanCurvatureAction(), SIGNAL(triggered()),
          this, SLOT(slot_mean_curvature_action()));
  connect(this->inspect_menu_handle_->GaussCurvatureAction(), SIGNAL(triggered()),
          this, SLOT(slot_gaussian_curvature_action()));
  connect(this->inspect_menu_handle_->PrincipalK1Action(), SIGNAL(triggered()),
          this, SLOT(slot_principal_k1_action()));
  connect(this->inspect_menu_handle_->PrincipalK2Action(), SIGNAL(triggered()),
          this, SLOT(slot_principal_k2_action()));
  
  connect(this->inspect_menu_handle_->NormalVectorAngleAction(), SIGNAL(triggered()),
          this, SLOT(slot_normal_vector_angle_action()));
  connect(this->inspect_menu_handle_->DistanceDiffAction(), SIGNAL(triggered()),
          this, SLOT(slot_distance_different_action()));
  
  connect(this->inspect_menu_handle_->MixMeanInteAction(), SIGNAL(triggered()),
          this, SLOT(slot_mix_mean_inte_action()));
  connect(this->inspect_menu_handle_->MixGaussInteAction(), SIGNAL(triggered()),
          this, SLOT(slot_mix_gauss_inte_action()));
  connect(this->inspect_menu_handle_->MixK1InteAction(), SIGNAL(triggered()),
          this, SLOT(slot_mix_k1_inte_action()));
  connect(this->inspect_menu_handle_->MixK2InteAction(), SIGNAL(triggered()),
          this, SLOT(slot_mix_k2_inte_action()));
  connect(this->inspect_menu_handle_->DefectFittingAction(), SIGNAL(triggered()), 
          this, SLOT(slot_defect_fitting_action()));
  connect(this->inspect_menu_handle_->PropagationAction(), SIGNAL(triggered()),
          this, SLOT(slot_propagate_action()));
  //cal zerniker coefficient
  connect(this->ratio_input_widget_handle_.get(), SIGNAL(SignalRadiusRatio(double, double, int)),
          this, SLOT(slot_exec_zerniker_action(double, double, int)));
  connect(this->inspect_menu_handle_->CalculateZernikerCoeffAction(), SIGNAL(triggered()),
          this, SLOT(slot_zerniker_coeff_action()));
//  connect(this->inspect_menu_handle_->ZernikerClassiryAction(), SIGNAL(triggered()),
//          this, SLOT(slot_zerniker_classify_action()));
  
  connect(this->inspect_thread_ptr_.get(), SIGNAL(SignalCompleteAlgorithm(lq::InspectFlag)),
          this, SLOT(SlotCompleteAligorithm(lq::InspectFlag)));
}



}
