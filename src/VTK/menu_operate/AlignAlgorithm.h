#ifndef ALIGNALGORITHM_H
#define ALIGNALGORITHM_H

#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkLandmarkTransform.h>

#include <MeshData.h>

#include <vcg/math/matrix44.h>

namespace sqi{
namespace alg{

class AlignAlgorithm
{
  
public: 
  AlignAlgorithm();
  virtual ~AlignAlgorithm();
  
  void GenerateRigidTransMatrix4x4(const vtkSmartPointer<vtkPoints> &points1,
                                   const vtkSmartPointer<vtkPoints> &points2,
                                   vtkSmartPointer<vtkLandmarkTransform> &land_mark_transform); 
  
  void TransMatrix4x4FromVtkToVcg(const vtkSmartPointer<vtkMatrix4x4> &vtk_mat,
                                  vcg::Matrix44f& vcg_mat);
  
  virtual void CalIntegralRefl(const size_t idx, const vcg::Matrix44f& mat,
                               meshdata::FaceHandleD& handle,
                               meshdata::MeshDataPtr& mesh_data);
  
  virtual void CalIntegralReflVertex(const size_t idx, const vcg::Matrix44f& mat,
                                     meshdata::MeshDataPtr& mesh_data, 
                                     std::vector<double>& vert_error);
  
  void CalNormalComponent(const size_t idx, const vcg::Matrix44f& mat,
                          meshdata::FaceHandleD& handle,
                          meshdata::MeshDataPtr& mesh_data);
  
  void CalNormalComponentVertex(const size_t idx, const vcg::Matrix44f& mat,
                                meshdata::MeshDataPtr& mesh_data,
                                std::vector<double>& vert_error);
  
};

}
}

#endif // ALIGNALGORITHM_H
