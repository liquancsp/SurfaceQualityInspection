#ifndef INSPECTALGTHREAD_H
#define INSPECTALGTHREAD_H

#include <QThread>
#include <MeshData.h>
#include <vtkSmartPointer.h>

namespace lq{
class vtk_model_data;
}

namespace sqi{
namespace common{
class InspectActionDirector;
class DescriptorActionDirector;
}
}

namespace sqi{
namespace thread{


class InspectAlgThread :  public QThread
{
  Q_OBJECT
public :
  InspectAlgThread(const sqi::meshdata::MeshDataPtr mesh,
                   const vtkSmartPointer<lq::vtk_model_data> data);
  ~InspectAlgThread();
  enum ThreadType
  {
    kAlg = 0,
    kSwitchModel,
    kDescriptor,
    kClassify
  };
  
  void SetAlgType(lq::InspectFlag alg_type)
  {
    this->alg_type_ = alg_type;
  }
  
  void SetAlgModel(const lq::AlgType alg_model)
  {
    this->alg_model_ = alg_model;
  }
  
  void SetThreadType(const ThreadType type)
  {
    this->thread_type_ = type;
  }

signals:
  
  void SignalCompleteAlgorithm(lq::InspectFlag flag);
  
protected:

  void run();

private:
  
  lq::InspectFlag alg_type_;
  lq::AlgType alg_model_;
  ThreadType thread_type_;
  sqi::meshdata::MeshDataPtr mesh_data_;
  vtkSmartPointer<lq::vtk_model_data> data_set_;
  std::tr1::shared_ptr<sqi::common::InspectActionDirector> inspect_director_ptr_;
  std::tr1::shared_ptr<sqi::common::DescriptorActionDirector> descriptor_director_ptr_;
};

}

}

#endif // INSPECTALGTHREAD_H
