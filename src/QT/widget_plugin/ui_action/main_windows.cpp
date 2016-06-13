#include <widget_plugin/ui_action/main_windows.h>
#include <widget_plugin/ui_resource/ui_ModelView.h>
#include <menu_plugin/ui_action/menu_file_item.h>
#include <menu_plugin/ui_action/menu_view_item.h>
#include <menu_plugin/ui_action/menu_inspect_item.h>
#include <menu_plugin/ui_action/menu_descriptor_item.h>
#include <menu_plugin/ui_action/menu_classify_item.h>
#include <menu_plugin/ui_action/MenuStatusControl.h>
#include <widget_plugin/ui_action/StatusBar.h>

#include <configure/configure.h>
#include <vtk_data/VtkPropManage.h>
#include <QComboBox>

#include <vtkRenderWindow.h>
#include <fstream>

namespace lq {

using namespace sqi::WidgetOp;
using namespace sqi::VtkData;
using namespace sqi::qt;
using namespace std;

main_windows::main_windows() : 
  data_set_(vtkSmartPointer<vtk_model_data>::New()),
  mesh_data_(new sqi::meshdata::MeshData<sqi::meshdata::Mesh>),
  model_name_text_actor_(new sqi::WidgetOp::ModelNameTextActor<vtk_model_data>),
  ui_main_panel_(new Ui_ModelView()),
  ui_left_region_(new left_region_win()),
  ui_file_item_(new menu_file_item(mesh_data_, data_set_, this)),
  ui_inspect_item_(new menu_inspect_item(mesh_data_, data_set_, this)),
  ui_descriptor_item_(new menu_descriptor_item(mesh_data_, data_set_, this)),
  ui_classify_item_(new menu_classify_item(mesh_data_, data_set_, this)),
  ui_view_item_(new menu_view_item(mesh_data_, data_set_, this)),
  menu_status_control_(new MenuStatusControl()),
  ui_status_bar_(new StatusBar(this))
{
  VtkPropManage::open_config_file();
  this->ui_main_panel_->setupUi(this);
  this->model_name_text_actor_->set_data_set(data_set_);
  this->ui_left_region_->set_data_set(data_set_);
  init();
  // initializes the menu bar
  this->ui_file_item_->set_data_set(data_set_);
  this->ui_file_item_->set_mesh_data(mesh_data_);
//  this->ui_edit_item_.reset(new menu_edit_item(this));
//  this->ui_edit_item_->set_data_set(data_set_);
//  this->ui_edit_item_->set_mesh_data(mesh_data_);
  
  this->ui_view_item_->set_data_set(data_set_);
  this->ui_view_item_->set_mesh_data(mesh_data_);
  this->ui_main_panel_->toolBar->close();
  
  this->menu_status_control_->SetInspectMenu(ui_inspect_item_);
  this->menu_status_control_->SetViewMenu(ui_view_item_);
  //add item to tool bar
//  this->ui_main_panel_->toolBar = addToolBar(tr("&File"));
//  this->ui_main_panel_->toolBar->addAction(this->ui_file_item_->
//                                           file_menu_handle_->OpenAction());
  this->ui_main_panel_->toolBar->addAction(this->ui_file_item_->GetUiHandle()->OpenAction());
  //this->addToolBar(Qt::RightToolBarArea, this->ui_main_panel_->toolBar);
  menuBar()->addMenu(ui_file_item_.get());
  menuBar()->addMenu(ui_inspect_item_.get());
  menuBar()->addMenu(ui_descriptor_item_.get());
  menuBar()->addMenu(ui_classify_item_.get());
//  menuBar()->addMenu(ui_edit_item_.get());
  menuBar()->addMenu(ui_view_item_.get());
  menuBar()->setVisible(true);
  this->setStatusBar(ui_status_bar_.get());
  create_action();
}

//I don't know why don't need to delete the pointer, if delete the 
//pointer it will be crash when you exit the main windows
main_windows::~main_windows()
{
  //save the configure file in current directory
  VtkPropManage::save_config_file(); 
}

/***************************************************************************************/
//begin public slots

void main_windows::slot_enabled_color_bar()
{
 // ui_view_item_->view_menu_handle_->add_color_legend_action->setEnabled(true);
}

void main_windows::slot_add_tree_leaf(const std::string &file_name)
{ 
  this->ui_left_region_->add_tree_leaf(file_name);
}

void main_windows::slot_new_action()
{
  slot_clear_two_actor();
  this->ui_file_item_->new_two_render();
}

void main_windows::slot_show_left_region(int flag)
{
  if(flag == kChecked)
  {
    Qt::WindowFlags flags = Qt::SubWindow;
    this->ui_left_region_->setWindowFlags(flags);
    this->ui_left_region_->setFixedWidth(256);
    this->addDockWidget(Qt::LeftDockWidgetArea, this->ui_left_region_.get());
    this->ui_left_region_->show();
  }
  else
  {
    this->ui_left_region_->hide();
  }
}

void main_windows::slot_delete_render_model(const QString &qt_file_name)
{
  std::string file_name(qPrintable(qt_file_name));
  if(strcmp(file_name.c_str(), data_set_->ModelName(kLeft).c_str()) == 0)
  {
    data_set_->Reset(kLeft);
    mesh_data_->Reset(kLeft);
  }
  else
  {
    data_set_->Reset(kRight);
    mesh_data_->Reset(kRight);
  }
}

void main_windows::slot_delete_render_model(lq::WindowFlag flag)
{
  if(data_set_ != NULL)
    data_set_->Reset(flag);
  if(mesh_data_.get() != NULL)
    mesh_data_->Reset(flag);
}

void main_windows::slot_update_text_actor()
{
  model_name_text_actor_->update_model_name_text_actor();
}

void main_windows::slot_clear_text_actor(lq::WindowFlag flag)
{
  model_name_text_actor_->clear_model_name_text_actor(flag);
}

void main_windows::slot_clear_two_actor()
{
  model_name_text_actor_->clear_two_text_actor();
//  this->ui_left_region_->slot_delete_tree_item(kLeft);
//  this->ui_left_region_->slot_delete_tree_item(kRight);
}

void main_windows::SlotMenuStatusChange()
{
  menu_status_control_->InvalidMenuStatus();
}

void main_windows::SlotMenuStatusResume()
{
  menu_status_control_->ResumeMenuStatus(mesh_data_->AlorithmModel());
}

//end of slots

/***************************************************************************************/
//begin private member function

void main_windows::init()
{
  this->ui_main_panel_->qvtk_widget_left->hide();
  this->ui_main_panel_->qvtk_widget_left->show();
  this->ui_main_panel_->qvtk_widget_left->update();
  data_set_->QTW() = this->ui_main_panel_->qvtk_widget_left;
}


void main_windows::create_action()
{
  //file item connect
  connect(this->ui_file_item_.get(), SIGNAL(send_model_file_name(const std::string &)),
          this, SLOT(slot_add_tree_leaf(const std::string &)));
 
  //inspect item connect
  connect(this->ui_inspect_item_.get(), SIGNAL(SignalBeginAlgAction()),
          this, SLOT(SlotMenuStatusChange()));
  connect(this->ui_inspect_item_.get(), SIGNAL(SignalCompleteInspect()),
          this, SLOT(SlotMenuStatusResume()));
  
  //view item connect
  connect(this->ui_view_item_.get(), SIGNAL(signal_show_left_region(int)),
          this, SLOT(slot_show_left_region(int)));
  
  connect(this->ui_view_item_.get(), SIGNAL(signal_flip_model()),
          this->ui_inspect_item_.get(), SLOT(slot_execute_alg()));
  connect(this->ui_view_item_.get(), SIGNAL(signal_clear_color()),
          this->ui_inspect_item_.get(), SLOT(slot_reset_checkable_action()));
  
  connect(this->ui_left_region_.get(), SIGNAL(delete_tree_leaf(const QString &)),
          this, SLOT(slot_delete_render_model(const QString &)));
  
  //context menu connect action
  //must delete the color legend first
  connect(this->ui_main_panel_->qvtk_widget_left, SIGNAL(signal_delete_model(lq::WindowFlag)),
          this, SLOT(slot_delete_render_model(lq::WindowFlag)));
//  connect(this->ui_main_panel_->qvtk_widget_left, SIGNAL(signal_delete_model(int)),
//          this->ui_left_region_.get(), SLOT(slot_delete_tree_item(int)));
  connect(this->ui_main_panel_->qvtk_widget_left, SIGNAL(signal_resize_win()),
          this->ui_view_item_.get(), SLOT(slot_resize_color_bar()));
  connect(this->ui_main_panel_->qvtk_widget_left, SIGNAL(signal_resize_win()),
          this, SLOT(slot_update_text_actor()));
  connect(this->ui_main_panel_->qvtk_widget_left, SIGNAL(signal_open_model()),
          this->ui_file_item_.get(), SLOT(open_file()));
  //connect new and delete action
  connect(this->ui_main_panel_->qvtk_widget_left, SIGNAL(singal_new()),
          this->ui_view_item_.get(), SLOT(slot_new_action()));
  
  connect(this->ui_main_panel_->qvtk_widget_left, SIGNAL(singal_new()),
          this, SLOT(slot_new_action()));
  
  connect(this->ui_main_panel_->qvtk_widget_left, SIGNAL(singal_new()),
          this->ui_inspect_item_.get(), SLOT(slot_reset_checkable_action()));
  
  connect(this->ui_main_panel_->qvtk_widget_left, SIGNAL(signal_delete_model(lq::WindowFlag)),
          this, SLOT(slot_clear_text_actor(lq::WindowFlag)));
  
  //create status bar action
  connect(this->ui_file_item_.get(), SIGNAL(SignalActionDone(const char*)),
          this->ui_status_bar_.get(), SLOT(SlotActionDone(const char*)));
  connect(this->ui_inspect_item_.get(), SIGNAL(SignalActionDone(const char*)),
          this->ui_status_bar_.get(), SLOT(SlotActionDone(const char*)));
  connect(this->ui_descriptor_item_.get(), SIGNAL(SignalActionDone(const char*)),
          this->ui_status_bar_.get(), SLOT(SlotActionDone(const char*)));
  //  connect(this->ui_file_item_.get(), SIGNAL(SignalExportFileDone()),
//          this->ui_status_bar_.get(), SLOT(SlotExportFileDone()));
  connect(this->ui_file_item_.get(), SIGNAL(SignalProgressRange(int, int)),
          this->ui_status_bar_.get(), SLOT(SlotProgressRange(int, int)));
  connect(this->ui_file_item_.get(), SIGNAL(SignalProgressValue(int)),
          this->ui_status_bar_.get(), SLOT(SlotProgressValue(int)));
  connect(this->ui_inspect_item_.get(), SIGNAL(SignalProgressRange(int, int)),
          this->ui_status_bar_.get(), SLOT(SlotProgressRange(int, int)));
  connect(this->ui_descriptor_item_.get(), SIGNAL(SignalProgressRange(int, int)),
          this->ui_status_bar_.get(), SLOT(SlotProgressRange(int, int)));
}

//end of private member fucntion
/**************************************************************************************/
}
