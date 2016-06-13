#ifndef CLASSIFYALGTHREAD_H
#define CLASSIFYALGTHREAD_H
#include <QThread>
#include <MeshData.h>
#include <vtkSmartPointer.h>

namespace lq{

class vtk_model_data;
}

namespace sqi{
namespace thread{

class ClassifyAlgThread : public QThread {
  Q_OBJECT
public :
  ClassifyAlgThread(const sqi::meshdata::MeshDataPtr mesh,
                    const vtkSmartPointer<lq::vtk_model_data> data);
  ~ClassifyAlgThread();
 
protected:
  void run();
  
private:
  
  
};

}

}

#endif // CLASSIFYALGTHREAD_H
