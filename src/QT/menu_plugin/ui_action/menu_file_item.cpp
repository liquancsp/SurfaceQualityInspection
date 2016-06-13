/****************************************************************************************
 *COPYRIGHT NOTICE
 *Copyright (c) 2013, Zhe Jiang University
 *All rights reserved
 *
 *@file menu_file_item.cpp
 *@author li quan
 *version 2.0
 *data 2013-3
 *ingroup QT/menu_plugin/ui_action
 *
 *@brief this file is to support the function of menubar->file item. It include open file,
 *Recent files, exit item.
 *
 ***************************************************************************************/

#include "menu_file_item.h"
#include <configure/configure.h>
#include <vtk_data/vtk_model_data.h>
#include <Facade/MenuOperator.h>
#include <thread/LoadFileThread.h>


#include <fstream>

#include <QtGui>
#include <QFile>
#include <QtXml/QDomDocument>
#include <QXmlStreamWriter>



namespace lq {

using namespace sqi::common;
using namespace sqi::thread;
using namespace sqi::meshdata;
using namespace std;

menu_file_item::menu_file_item(sqi::meshdata::MeshDataPtr mesh,
                               vtkSmartPointer<lq::vtk_model_data> data, QWidget* parent) : QMenu(parent),
  file_menu_handle_(new Ui::ui_menu_file_item()),
  menu_operator_(new MenuOperator(mesh, data)), 
  load_file_thread_left_(new LoadFileThread(mesh, data)),
  load_file_thread_right_(new LoadFileThread(mesh, data))
{
  cout << "[#Info-Init] Initialize file menu item " << endl;
  this->file_menu_handle_->setup_ui(this);
  create_action();
}

menu_file_item::~menu_file_item()
{
}

void menu_file_item::set_data_set(const vtkSmartPointer<vtk_model_data> &other_data_set)
{
  this->data_set_ = other_data_set;
}

const Ui::ui_menu_file_item* menu_file_item::GetUiHandle()
{
  return this->file_menu_handle_.get();
}

//------------------------------------------------------------------------------
// begin slot
void menu_file_item::open_file()
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
                                                   tr("OBJ files (*.obj *.nas *vproj)"));
  
  if(!file_name.isEmpty())
  {
    std::string str(qPrintable(file_name));
    emit SignalProgressRange(0, 0);
    if(GetExtendName(str) == "vproj")
    {
      ParseProjFile(str);
      size_t flag = str.find_last_of('/');
      string path = str.substr(0, flag + 1);
      cout << path << endl;
      if(file_name.size() == 0)
      {
        return ;
      }
      get_model_file_name(file_list_[kLeft], kLeft);
      load_file_thread_left_->SlotSetWinAndFile(kLeft, path + file_list_[kLeft] );
      load_file_thread_left_->start();
      if(file_name.size() > 1)
      {
        get_model_file_name(file_list_[kRight], kRight);
        load_file_thread_right_->SlotSetWinAndFile(kRight, path + file_list_[kRight]);
        load_file_thread_right_->start();
      }
    }
    else
    {
      if(!mesh_data_->IsWinHas(kLeft))
      {
        connect(this, SIGNAL(SignalSendWinAndFile(lq::WindowFlag,std::string)),
                load_file_thread_left_.get(), SLOT(SlotSetWinAndFile(lq::WindowFlag,std::string)));
        emit SignalSendWinAndFile(kLeft, str);
       
        load_file_thread_left_->start();
        get_model_file_name(str, kLeft);     
        
      }
      else if(!mesh_data_->IsWinHas(kRight))
      {
        connect(this, SIGNAL(SignalSendWinAndFile(lq::WindowFlag,std::string)),
                load_file_thread_right_.get(), SLOT(SlotSetWinAndFile(lq::WindowFlag,std::string)));
        emit SignalSendWinAndFile(kRight, str);
        ///emit SignalProgressRange(0, 0);
        load_file_thread_right_->start();
        get_model_file_name(str, kRight);
      }
    }
  }//end of if
}

void menu_file_item::new_two_render()
{
  if(mesh_data_->IsWinHas(kLeft))
  {
    data_set_->Reset(kLeft);
    mesh_data_->Reset(kLeft);
  }
  if(mesh_data_->IsWinHas(kRight))
  {
    data_set_->Reset(kRight);
    mesh_data_->Reset(kRight);
  }
  emit signal_new_two_render();
}


void menu_file_item::slot_import_training_label()
{
  QStringList file_list = QFileDialog::getOpenFileNames(this,
                                                   tr("Open File"), ".",
                                                   tr("select files (*.select)"));
  
  mesh_data_->SelectFileList().clear();
  for (QStringList::iterator iter = file_list.begin(); iter != file_list.end(); ++iter) 
  { 
    mesh_data_->SelectFileList().push_back(qPrintable(*iter));
    
  } 
  sort(mesh_data_->SelectFileList().begin(), mesh_data_->SelectFileList().end());
  mesh_data_->SelectIdxList().clear();
  mesh_data_->SelectIdxList().resize(mesh_data_->SelectFileList().size());
  for(size_t i = 0; i < mesh_data_->SelectFileList().size(); ++i)
  {
    ifstream fin(mesh_data_->SelectFileList()[i].c_str());
    if(fin.fail())
    {
      cerr << "[#Error]Fail to open : " << mesh_data_->SelectFileList()[i] << endl;
      return ;
    }
    string in;
    while(getline(fin, in))
      mesh_data_->SelectIdxList()[i].insert(atoi(in.c_str()));
    fin.close();
    std::cout << "[#Info]Select file path is : " << mesh_data_->SelectFileList()[i] << endl;
  }
  menu_operator_->SetSelectClassifyColor(lq::kLeft);
}

void menu_file_item::slot_export_training_label()
{
  for(size_t i = 0; i < mesh_data_->SelectFileList().size(); ++i)
  {
    string file_name = mesh_data_->SelectFileList()[i];
    ofstream fout(file_name.c_str());
    if(fout.fail())
    {
      cerr << "[#Error]Fail to open : " << file_name << endl;
      return ;
    }
    for(set<int>::iterator iter = mesh_data_->SelectIdxList()[i].begin();
        iter != mesh_data_->SelectIdxList()[i].end(); ++iter)
      fout << *iter << endl;
    fout.close();
    menu_operator_->SetSelectClassifyColor(lq::kLeft);
  }
}

void menu_file_item::slot_export_file()
{
  QString file_name = QFileDialog::getSaveFileName(this, tr("Save File"),
                             "untitled.lad",
                             tr("Images (*.lad)"));
  if(!equal(mesh_data_->LocalRadius() , 0.0))
  {
    cout << "export file name : " << qPrintable(file_name) << std::endl;
    ofstream fout(qPrintable(file_name));
    fout << "Radius " << mesh_data_->LocalRadius() << endl;
    fout << data_set_->ModelName(kLeft) << " " << data_set_->ModelName(kRight) << endl;
    FaceHandleD local_inte_handle = mesh_data_->GetLocalIntegral();
    FaceHandleD local_normal_handle = mesh_data_->GetLocalNormalCom();
    emit SignalProgressRange(0, mesh_data_->FNum(kRight) - 1);
    for(size_t i = 0; i < mesh_data_->FNum(kRight); ++i)
    {
      fout << local_inte_handle[i] << " " << local_normal_handle[i] << endl;
      emit SignalProgressValue(i);
    }
    fout.close();
    emit SignalActionDone(SUCCESS_EXPORT_LAB);
  }
  else 
  {
    QMessageBox::warning(this, tr(WARNNING_TITLE),
                         tr(NO_EXPORT_DATA),
                         QMessageBox::Cancel);
    return ;
  }
//    file.open(QIODevice::WriteOnly);
//    QXmlStreamWriter writer(&file);
//    writer.setAutoFormatting(true);
//    writer.writeStartDocument();
//    writer.writeStartElement("Information");
//    QString radius_qstr;
//    radius_qstr.setNum(mesh_data_->LocalRadius());
//    writer.writeTextElement("AlignmentRadius", radius_qstr);
//    writer.writeTextElement("LeftFileName", data_set_->ModelName(kLeft).c_str());
//    writer.writeTextElement("RightFileName", data_set_->ModelName(kRight).c_str());
//    sqi::meshdata::FaceHandleD local_inte_handle = mesh_data_->GetLocalIntegral();
//    writer.writeStartElement("LocalAlignData");
//    writer.writeStartElement("LocalIntegral");
//    for(size_t i = 0; i < mesh_data_->FNum(kRight); ++i)
//    {
//      if(!is_zero(local_inte_handle[i], EPS_E6))
//      {
//        QString tmp1, tmp2;
//        tmp1.setNum(local_inte_handle[i], 'g', 10);
//        tmp2.setNum(i, 10);
//        tmp2 += " ";
//        writer.writeTextElement("Data", tmp2 + tmp1);
//      }
//    }
//    writer.writeEndElement();//LocalIntegral
//    writer.writeStartElement("LocalNormalComponent");
//    sqi::meshdata::FaceHandleD local_normal_handle = mesh_data_->GetLocalNormalCom();
//    for(size_t i = 0; i < mesh_data_->FNum(kRight); ++i)
//    {
//      if(!is_zero(local_normal_handle[i], EPS_E6))
//      {
//        QString tmp1, tmp2;
//        tmp1.setNum(local_normal_handle[i], 'g', 10);
//        tmp2.setNum(i, 10);
//        tmp2 += " ";
//        writer.writeTextElement("Data", tmp2 + tmp1);
//      }
//    }
//    writer.writeEndElement();//LocalNormalComponent
//    writer.writeEndElement();//LocalAlignData
//    writer.writeEndElement();
    
    
//    writer.writeEndDocument();
//    file.close();
  
}

void menu_file_item::slot_import_file()
{
  QString file_name = QFileDialog::getOpenFileName(this,
                                                   tr("Open File"), ".",
                                                   tr("OBJ files (*.lad)"));
  if(mesh_data_.get() != NULL)
  {
    ImportLocalAlignData(qPrintable(file_name));
  }
}

void menu_file_item::exit_application()
{
  exit(1);
}

//end of slots

void menu_file_item::SlotGetCompleteFlag(const int win)
{
  menu_operator_->CreateVtkRender(static_cast<lq::WindowFlag>(win));
  mesh_data_->IsWinHas(win) = true;
  //if(mesh_data_->IsWinHas(kLeft) && mesh_data_->IsWinHas(kRight))
  emit SignalActionDone(LOAD_MODEL_FILE_DONE);
}

//------------------------------------------------------------------------------


void menu_file_item::get_model_file_name(const std::string &path_name, int flag)
{
  std::string file_name;
  get_model_name(path_name, file_name);
  data_set_->ModelName(flag) = file_name;
  //data_set_->model_list_.push_back(file_name);
  emit send_model_file_name(file_name);
}

void menu_file_item::ParseProjFile(const std::string &proj_file)
{
  file_list_.clear();
  QFile xml(proj_file.c_str());
  if(!xml.open(QFile::ReadOnly))
    return ;
  QDomDocument doc;
  if(!doc.setContent(&xml))
  {
    xml.close();
    return ;
  }
  QDomNodeList node_list = doc.elementsByTagName("project");
  for(size_t i = 0; i < node_list.count(); ++i)
  {
    QDomElement element = node_list.at(i).toElement();
    QDomNode node = element.firstChild();
    while(!node.isNull())
    {
      QDomElement tmp = node.toElement();
      QString tag_name = tmp.tagName();
      if(tag_name == "model")
        file_list_.push_back(qPrintable(tmp.text()));
      node = node.nextSibling();
    }
  }
}

std::string menu_file_item::GetExtendName(const std::string &file_name)
{
  size_t flag = file_name.find_last_of('.');
  return file_name.substr(flag+1);
}

void menu_file_item::ImportLocalAlignData(const std::string &file_name)
{
  ifstream fin(file_name.c_str());
  double radius = 0.0f;
  string input;
  getline(fin, input);
  char tmp[20];
  sscanf(input.c_str(), "%s %lf", &tmp, &radius);
  char left_file_name[30], right_file_name[30];
  getline(fin, input);
  sscanf(input.c_str(), "%s %s", &left_file_name, &right_file_name);
  bool flag1 = false;
  bool flag2 = false;
  if(strcmp(left_file_name, data_set_->ModelName(kLeft).c_str()) == 0 && 
     strcmp(right_file_name, data_set_->ModelName(kRight).c_str())  == 0)
    flag1 = true;
  if(strcmp(left_file_name, data_set_->ModelName(kRight).c_str())== 0 &&
     strcmp(right_file_name, data_set_->ModelName(kLeft).c_str()) == 0)
    flag2 = true;
  if(!(flag1 || flag2))
  {
    QMessageBox::warning(this, tr(WARNNING_TITLE),
                         tr(WRONG_MATCH),
                         QMessageBox::Cancel);
    fin.close();
    return ;
  }
  mesh_data_->PreLocalRadius() = radius;
  double diff1 = 0.0f;
  double diff2 = 0.0f;
  FaceHandleD local_inte_handle = mesh_data_->GetLocalIntegral();
  FaceHandleD local_normal_handle = mesh_data_->GetLocalNormalCom();
  int idx = 0;
  emit SignalProgressRange(0, mesh_data_->FNum(kRight) - 1);
  while(getline(fin, input))
  {
    sscanf(input.c_str(), "%lf %lf", &diff1, &diff2);
    local_inte_handle[idx] = diff1;
    local_normal_handle[idx] = diff2;
    emit SignalProgressValue(idx);
    ++idx;
  }
  fin.close();
//  QFile file(file_name.c_str());
//  if(!file.open(QFile::ReadOnly))
//  {
//    QMessageBox::warning(this, tr(WARNNING_TITLE),
//                         tr(FAIL_OPEN_DATA),
//                         QMessageBox::Cancel);
//    return ;
//  }
//  QDomDocument doc;
//  if(!doc.setContent(&file))
//  {
//    QMessageBox::warning(this, tr(WARNNING_TITLE),
//                         tr(FAIL_INITIALIZE_DOC),
//                         QMessageBox::Cancel);
//    file.close();
//    return ;
//  }
  
//  QDomNodeList node_list = doc.elementsByTagName(QString("Information"));
//  for(size_t i = 0; i < node_list.count(); ++i)
//  {
//    QDomElement element = node_list.at(i).toElement();
//    QDomNode node = element.firstChild();
//    QDomElement tmp = node.toElement();
//    double radius = tmp.text().toDouble();
//    node = node.nextSibling();
//    tmp = node.toElement();
//    string left_file_name = tmp.text().toStdString();
//    node = node.nextSibling();
//    tmp = node.toElement();
//    string right_file_name = tmp.text().toStdString();
//    bool flag1 = false;
//    bool flag2 = false;
//    if(strcmp(left_file_name.c_str(), data_set_->ModelName(kLeft).c_str()) == 0 && 
//       strcmp(right_file_name.c_str(), data_set_->ModelName(kRight).c_str())  == 0)
//      flag1 = true;
//    if(strcmp(left_file_name.c_str(), data_set_->ModelName(kRight).c_str())== 0 &&
//       strcmp(right_file_name.c_str(), data_set_->ModelName(kLeft).c_str()) == 0)
//      flag2 = true;
//    if(!(flag1 || flag2))
//    {
//      QMessageBox::warning(this, tr(WARNNING_TITLE),
//                           tr(WRONG_MATCH),
//                           QMessageBox::Cancel);
//      file.close();
//      return ;
//    }
//    mesh_data_->PreLocalRadius() = radius;
//    node = node.nextSibling();//<localAlignData>
//    tmp = node.toElement();
//    QString tag_name = tmp.tagName();
//    FaceHandleD handle;
//    if(tag_name == "LocalAlignData")
//    {
//      QDomNode child_node = node.firstChild();//<LocalIntegral>
//      while(!child_node.isNull())
//      {
//        if(child_node.toElement().tagName() == "LocalIntegral")
//          handle = mesh_data_->GetLocalIntegral();
//        else if(child_node.toElement().tagName() == "LocalNormalComponent")
//          handle = mesh_data_->GetLocalNormalCom();
//        mesh_data_->InitAttribute<FaceHandleD, double>(handle, 0.0f, 
//                                                       mesh_data_->FNum(kRight));
//        QDomNode data_child = child_node.firstChild();
//        while(!data_child.isNull())
//        {
//          string data_str = data_child.toElement().text().toStdString();
//          int idx = 0;
//          double error = 0.0f;
//          sscanf(data_str.c_str(), "%d %lf", &idx, &error);
//          handle[idx] = error;
//          data_child = data_child.nextSibling();
//        }
//        child_node = child_node.nextSibling();
//      }
//    }
//  }
//  file.close();
  emit SignalActionDone(SUCCESS_LOAD_LAB);
}

//qRegisterMetaType<lq::WindowFlag>("lq::WindowFlag"); 
void menu_file_item::create_action()
{
  connect(this->file_menu_handle_->OpenAction(), SIGNAL(triggered()),
          this, SLOT(open_file()));
  connect(this->file_menu_handle_->NewAction(),SIGNAL(triggered()),
          this, SLOT(new_two_render()));
  
  connect(this->file_menu_handle_->ImportTrainingLabelAction(), SIGNAL(triggered()),
          this, SLOT(slot_import_training_label()));
  connect(this->file_menu_handle_->ExportTrainingLabelAction(), SIGNAL(triggered()),
          this, SLOT(slot_export_training_label()));
  
  connect(this->file_menu_handle_->ExportAction(), SIGNAL(triggered()),
          this, SLOT(slot_export_file()));
  connect(this->file_menu_handle_->ImportAction(), SIGNAL(triggered()),
          this, SLOT(slot_import_file()));
  
  connect(this->file_menu_handle_->ExitAction(), SIGNAL(triggered()),
          this, SLOT(exit_application()));
  
  connect(this->load_file_thread_left_.get(), SIGNAL(SignalCompleteLoadFile(const int)),
          this, SLOT(SlotGetCompleteFlag(int)),Qt::QueuedConnection);
  connect(this->load_file_thread_right_.get(), SIGNAL(SignalCompleteLoadFile(const int)),
          this, SLOT(SlotGetCompleteFlag(int)),Qt::QueuedConnection);
}


}
