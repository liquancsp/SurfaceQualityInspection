#ifndef FUNCTIONDISTURB_H
#define FUNCTIONDISTURB_H
#include <MeshData.h>
#include <vtk_data/vtk_model_data.h>
#include <vtkIdList.h>

namespace sqi{
namespace utils{

class FunctionDisturb {
  
public :
  FunctionDisturb(){}
  void GenerateDisturbList(sqi::meshdata::MeshDataPtr mesh_data, 
                           vtkSmartPointer<lq::vtk_model_data> data_set,
                           int center_idx, double radius)
  {
    data_set->BuildKdTree(lq::kLeft);
    vtkSmartPointer<vtkIdList> result = 
        vtkSmartPointer<vtkIdList>::New();
    sqi::meshdata::VertexIterator vi = mesh_data->VBeginIter(lq::kLeft) + center_idx;
    double ref_point[3] = {vi->cP()[0], vi->cP()[1], vi->cP()[2]};
    data_set->KdTree()->FindPointsWithinRadius(radius, ref_point, result);
    disturb_idx_.resize(result->GetNumberOfIds());
    for(int i = 0; i < result->GetNumberOfIds(); ++i)
      disturb_idx_[i] = result->GetId(i);
  }
  
  virtual ~FunctionDisturb(){}
  virtual void Update(sqi::meshdata::MeshDataPtr mesh_data,
                      const std::string& out_file,double amplitude,
                      double phase){}
  
  virtual void Update(sqi::meshdata::MeshDataPtr mesh_data,
                      const std::string& out_file, 
                      vcg::Point3f& center, double radius){}
  
protected:
  std::vector<int> disturb_idx_;
  
};

}
}

#endif // FUNCTIONDISTURB_H
