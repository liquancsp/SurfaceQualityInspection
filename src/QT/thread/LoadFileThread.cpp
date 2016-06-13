#include <thread/LoadFileThread.h>
#include <Facade/MenuOperator.h>
#include <vtk_data/vtk_model_data.h>

namespace sqi {
namespace thread{

using namespace std;
using namespace sqi::common;
using namespace lq;

LoadFileThread::LoadFileThread(sqi::meshdata::MeshDataPtr mesh,
                               vtkSmartPointer<lq::vtk_model_data> data) : QThread(),
  win_(lq::kNullWin), file_name_(""), mesh_data_(mesh), data_set_(data),
  menu_operator_(new MenuOperator(mesh, data))
{
}

LoadFileThread::~LoadFileThread() 
{
  
}


void LoadFileThread::run()
{
  menu_operator_->LoadModelFile(file_name_, win_);
  menu_operator_->SetRenderData(win_);
  emit SignalCompleteLoadFile(win_);
}


void LoadFileThread::SlotSetWinAndFile(const lq::WindowFlag win, 
                                       const std::string& file_name)
{
  win_ = win;
  file_name_ = file_name;
}


}

}


