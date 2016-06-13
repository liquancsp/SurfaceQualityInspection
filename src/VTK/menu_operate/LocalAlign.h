 #ifndef LOCALALIGN_H
#define LOCALALIGN_H
#include <MeshData.h>
#include <menu_operate/AlignAlgorithm.h>
#include <vtk_data/vtk_model_data.h>
#include <vtkKdTreePointLocator.h>

#include <vector>

namespace sqi{
namespace alg{

class LocalAlign : public AlignAlgorithm
{
  
public:
  LocalAlign();
  virtual ~LocalAlign();  
  
 
  
  void ExecuteComputeErrorUseFace(meshdata::MeshDataPtr& mesh_data, lq::InspectFlag flag,
                                  const bool is_computed);
  
  void ExecuteComputeErrorUseVertex(meshdata::MeshDataPtr& mesh_data);
  
  void ExecuteLocalAlignFace(const size_t idx, std::vector<int> &neigh_list,
                             vcg::Matrix44f& mat, meshdata::MeshDataPtr& mesh);
  
  int ExecuteLocalAlignVertex(const size_t idx, std::vector<int> &neigh_list,
                              vcg::Matrix44f& mat, meshdata::MeshDataPtr& mesh);
  
  void SearchNeighWithKDTree(const size_t idx, std::vector<int>& neigh_list,
                             meshdata::MeshDataPtr& mesh);
  void CalIntegralRefl(const vcg::Matrix44f& mat, const std::vector<int> &neigh_list, 
                       const size_t index, meshdata::FaceHandleD& handle,
                       meshdata::MeshDataPtr &mesh_data);
  
//  void CalIntegralReflNonMutlScalar(const vcg::Matrix44d& mat, const size_t index, 
//                                    meshdata::FaceHandleD& handle,
//                                    meshdata::MeshDataPtr &mesh_data);
  
  void CalNormalComponent(const vcg::Matrix44f& mat, const size_t idx, 
                          meshdata::MeshDataPtr& mesh_data);
  
  void SaveResult(const std::vector<double>& vert_error, 
                  const meshdata::MeshDataPtr& mesh_data,
                  lq::WindowFlag win,
                  meshdata::FaceHandleD& handle);
  
  static void SetDataSet(vtkSmartPointer<lq::vtk_model_data>& data)
  {
    data_set_ = data;
  }
  
protected:
  
  inline void ComputeTransferMatrixICP(std::vector<int> &neigh_list, 
                                       vcg::Matrix44f &mat, meshdata::MeshDataPtr &mesh);
  
  inline void ComputeTransferMatrixLS(std::vector<int> &neigh_list,
                                      vcg::Matrix44f& mat, meshdata::MeshDataPtr& mesh);
private:
  static vtkSmartPointer<lq::vtk_model_data> data_set_;
};


}
}



#endif // LOCALALIGN_H
