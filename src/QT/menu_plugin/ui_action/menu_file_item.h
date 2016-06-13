/*************************************************************************************
 *
 *COPYRIGHT NOTICE
 *Copyright (c) 2013, Zhe Jiang University
 *All rights reserved.
 *
 *@file menu_file_item.h
 *@author li quan
 *
 *version 2.0
 *data 2013-3
 *ingroup QT/menu_plugin/ui_action
 *
 *@brief this file is to support the function of menubar->file item. It include open file,
 *Recent files, exit item.
 *
 *************************************************************************************/

#ifndef MENU_FILE_ITEM_H
#define MENU_FILE_ITEM_H
#include <menu_plugin/ui_resource/ui_menu_file.h>
#include <vtkSmartPointer.h>
#include <MeshData.h>
#if defined(_MSC_VER)
#include <memory>
#endif

#if defined(__GNUC__)
#include <tr1/memory>
#endif

namespace sqi{
namespace common{
class MenuOperator;
}
}

namespace sqi{
namespace thread{
class LoadFileThread;
}
}

namespace lq {

class vtk_model_data;

  /*********************************************************************************
   *
   *@class menu_file_item
   *@ingroup qt_vtk
   *@brief this file is to support the function of menubar->file item. It include open file,
   *Recent files, exit item.
   *
  **********************************************************************************/ 

  class menu_file_item : public QMenu
  {
    Q_OBJECT

public :
    menu_file_item(sqi::meshdata::MeshDataPtr mesh,
                   vtkSmartPointer<lq::vtk_model_data> data, QWidget* parent=0);
    ~menu_file_item();
    void set_data_set(const vtkSmartPointer<vtk_model_data> &other_data_set);
    inline void set_mesh_data(const std::tr1::shared_ptr<sqi::meshdata::MeshData<sqi::meshdata::Mesh> >& other)
    {
      this->mesh_data_ = other;
    }
   
    const Ui::ui_menu_file_item* GetUiHandle();
    
    
 signals:
    
    void send_model_file_name(const std::string &file_name);
    void signal_new_two_render();
    void SignalSendWinAndFile(const lq::WindowFlag win,
                              const std::string& file_name);
    void SignalActionDone(const char* str);
    void SignalExportFileDone();
    void SignalLoadLadFileDone();
    void SignalProgressRange(int begin, int end);
    void SignalProgressValue(int value);
    
public slots:

    void open_file();
    void new_two_render();
    void slot_import_training_label();
    void slot_export_training_label();
    void slot_export_file();
    void slot_import_file();
    void exit_application();
    void SlotGetCompleteFlag(const int win);

 private:

    std::tr1::shared_ptr<Ui::ui_menu_file_item> file_menu_handle_;
    void get_model_file_name(const std::string &path_name, int flag);
    void ParseProjFile(const std::string& proj_file);
    std::string GetExtendName(const std::string& file_name);
    void ImportLocalAlignData(const std::string& file_name);
    std::vector<std::string> file_list_;
    vtkSmartPointer<vtk_model_data> data_set_;
    sqi::meshdata::MeshDataPtr mesh_data_;
    std::tr1::shared_ptr<sqi::common::MenuOperator> menu_operator_;
    std::tr1::shared_ptr<sqi::thread::LoadFileThread> load_file_thread_left_;
    std::tr1::shared_ptr<sqi::thread::LoadFileThread> load_file_thread_right_;
    
    void create_action();
  };

}

#endif
