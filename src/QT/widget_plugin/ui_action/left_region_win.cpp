#include "left_region_win.h"
#include "../../configure/configure.h"

namespace lq
{

using namespace  std;

QString select_txt;

void left_region_win::init()
{
  root = new QTreeWidgetItem(this->tree_widget,
                             QStringList(QString(MODEL)));
  this->tree_widget->setColumnCount(1);
  this->tree_widget->setHeaderLabels(QStringList()<<TREE_TITLE);
//  this->std_spin_box->setRange(STD_ZERO, STD_MAX_RANGE);
//  this->std_slider->setRange(STD_ZERO, STD_MAX_RANGE);
  //this->color_legend_btn->setEnabled(false);
  this->delete_btn_action->setEnabled(false);
//  this->std_slider->setEnabled(false);
//  this->std_spin_box->setEnabled(false);
//  this->std_slider->setSliderPosition(STD_DEFAULT_VALUE);  
}

left_region_win::left_region_win(QWidget* parent):QDockWidget(parent)
{
  //Just call this to setup the GUI using QT UI file
  cout << "[#Info]Left region" << endl;
  setupUi(this);
  create_action();
  init();
}

left_region_win::~left_region_win()
{
 // root->~QTreeWidgetItem();
}

void left_region_win::set_data_set(const vtkSmartPointer<vtk_model_data> &other_data_set)
{
  this->data_set_ = other_data_set;
  left_operate_.set_data_set(data_set_);
}

void left_region_win::add_tree_leaf(const string &file_name)
{
  this->delete_btn_action->setEnabled(true);
  QTreeWidgetItem *leaf = new QTreeWidgetItem(root,
                                              QStringList(QString(file_name.c_str())));
  root->addChild(leaf);
  tree_widget->insertTopLevelItem(0, root);
//  if(leaf != NULL)
//  {
//    delete leaf;    
//    leaf = NULL;
//  }
}

//////////////////////////////////////////////////////////////////
//begin slot
void left_region_win::select_item(QTreeWidgetItem *item, int column)
{
  //get deleted file name
  select_txt = item->text(column);
  
}

void left_region_win::delete_btn_down()
{
  //send file name which will be deleted to the model view
  emit delete_tree_leaf(tree_widget->currentItem()->text(0));
  delete tree_widget->currentItem();
}

//slot of context menu delet action
void left_region_win::slot_delete_tree_item(int flag)
{
  QString qstr(data_set_->ModelName(flag).c_str());
  std::cout << "delete " << data_set_->ModelName(flag) << std::endl;
  for(size_t i = 0; i < root->childCount(); ++i)
  {
    QTreeWidgetItem *tmp = root->child(i);
    if(QString::compare(tmp->text(0), qstr) == 0)
    {
      root->removeChild(tmp);
    }
  } 
}

void left_region_win::adjust_color()
{
//  left_operate_.set_spin_value(this->std_spin_box->value());
//  left_operate_.set_spin_standard(100);
  left_operate_.update_color_on_poly();
  left_operate_.update_legend_color();
  data_set_->QTW()->update();
}

//end of slots
//------------------------------------------------------------------------------
void left_region_win::set_tree(const vector<string> &model_list)
{
 

  for(size_t i = 0; i < model_list.size(); ++i)
  {
    QTreeWidgetItem *leaf = new QTreeWidgetItem(root,
                                                QStringList(QString(model_list[i].c_str())));
    root->addChild(leaf);
  }
  QList<QTreeWidgetItem *> rootList;
  rootList << root;
  tree_widget->insertTopLevelItems(0, rootList);
}

void left_region_win::create_action()
{
  connect(tree_widget, SIGNAL(itemClicked(QTreeWidgetItem *, int)),
           this, SLOT(select_item(QTreeWidgetItem*, int)));

  connect(delete_btn_action, SIGNAL(clicked()), this,
          SLOT(delete_btn_down()));

//  connect(std_spin_box, SIGNAL(valueChanged(int)),
//          std_slider, SLOT(setValue(int)));

//  connect(std_slider, SIGNAL(valueChanged(int)),
//          std_spin_box, SLOT(setValue(int)));

//  connect(std_spin_box, SIGNAL(valueChanged(int)),
//          this, SLOT(adjust_color()));
}


}
