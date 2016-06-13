#ifndef DESCRIPTORALGTHREAD_H
#define DESCRIPTORALGTHREAD_H
#include <QThread>
#include <MeshData.h>
#include <vtkSmartPointer.h>

namespace lq{
class vtk_model_data;
}

namespace sqi{
namespace common{
class DescriptorActionDirector;
}
}

namespace sqi{
namespace thread {
class DescriptorAlgThread : public QThread
{
  Q_OBJECT
public:
  DescriptorAlgThread(const sqi::meshdata::MeshDataPtr mesh,
                      const vtkSmartPointer<lq::vtk_model_data> data);
  
  ~DescriptorAlgThread();
  
};

}
}



#endif // DESCRIPTORALGTHREAD_H
