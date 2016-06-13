#ifndef AREAPICKCALLBACK_H
#define AREAPICKCALLBACK_H

#include <vtk_data/vtk_model_data.h>
#include <MeshData.h>

#include <vtkDataSetMapper.h>
#include <vtkCommand.h>

namespace sqi{
namespace vtk{

class AreaPickCallback : public vtkCommand
{
  
public :
  AreaPickCallback();
  ~AreaPickCallback();
  static AreaPickCallback* New(){
    return new AreaPickCallback;
  }
  
  void SetDataSet(const vtkSmartPointer<lq::vtk_model_data>& data);
  void SetMeshData(const sqi::meshdata::MeshDataPtr& mesh);
  void SetWinFlag(const lq::WindowFlag& flag);
  
  virtual void Execute(vtkObject *obj, unsigned long, void *cell_data);
  
private:
  vtkSmartPointer<lq::vtk_model_data> data_set_;
  sqi::meshdata::MeshDataPtr mesh_data_;
  lq::WindowFlag win_flag_;
  vtkSmartPointer<vtkActor> selected_actor_;
  vtkSmartPointer<vtkDataSetMapper> selected_mapper_;
};

}
}



#endif // AREAPICKCALLBACK_H
