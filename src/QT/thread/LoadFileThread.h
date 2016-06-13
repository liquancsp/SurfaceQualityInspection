#ifndef LOADFILETHREAD_H
#define LOADFILETHREAD_H

#include <QThread>
#include <vtkSmartPointer.h>
#include <MeshData.h>


namespace sqi {
namespace common {
class MenuOperator;
}
}


namespace lq {
class vtk_model_data;
}

namespace sqi {
namespace thread{

class LoadFileThread : public QThread
{
  Q_OBJECT
public:
  LoadFileThread(sqi::meshdata::MeshDataPtr mesh,
                 vtkSmartPointer<lq::vtk_model_data> data);
  ~LoadFileThread();
  void SetWinFlag(const lq::WindowFlag win)
  {
    this->win_ = win;
  }
  
  void SetFileName(const std::string& file_name)
  {
    this->file_name_ = file_name;
  }
  
signals:
  void SignalCompleteLoadFile(const int win);
  
public slots:
 
 void SlotSetWinAndFile(const lq::WindowFlag win, const std::string& file_name);
 
protected:
  
  void run();
  
private:

  lq::WindowFlag win_;
  std::string file_name_;
  sqi::meshdata::MeshDataPtr mesh_data_;
  vtkSmartPointer<lq::vtk_model_data> data_set_;
  std::tr1::shared_ptr<sqi::common::MenuOperator> menu_operator_;
};

    
}

}
#endif // LOADFILETHREAD_H
