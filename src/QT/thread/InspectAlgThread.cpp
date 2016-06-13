#include <thread/InspectAlgThread.h>
#include <vtk_data/vtk_model_data.h>
#include <Builder/InspectActionDirector.h>
#include <Builder/DescriptorActionDirector.h>
using namespace std;
namespace sqi{

namespace thread{

using namespace sqi::common;

InspectAlgThread::InspectAlgThread(const sqi::meshdata::MeshDataPtr mesh, 
                                   const vtkSmartPointer<lq::vtk_model_data> data): QThread(),
  alg_type_(lq::kNull), alg_model_(lq::kCalSingleProperty), thread_type_(kAlg),
  mesh_data_(mesh), data_set_(data),
  inspect_director_ptr_(new InspectActionDirector(mesh, data)),
  descriptor_director_ptr_(new DescriptorActionDirector(mesh, data))
{
  
}

InspectAlgThread::~InspectAlgThread()
{
  
}


void InspectAlgThread::run()
{
  if(thread_type_ == kAlg || thread_type_ == kClassify)
  {
    std::cout << "[#Info]Thread in inspect algorithms." << std::endl;
    inspect_director_ptr_->Execute(alg_type_, alg_model_);
    emit SignalCompleteAlgorithm(alg_type_);
  }
  else if(thread_type_ == kSwitchModel)
    inspect_director_ptr_->ResetScene();
  else if(thread_type_ == kDescriptor)
  {
    cout << "[#Info]Thread in descriptor." << endl;
    descriptor_director_ptr_->Execute(alg_type_);
    emit SignalCompleteAlgorithm(alg_type_);
  }
  
}

}
}
