#ifndef BASEALGORITHM_H
#define BASEALGORITHM_H
#include <vcg/complex/complex.h>
#include <MeshData.h>
#include <data_type.h>
#if defined(_MSC_VER)
#include <memory>
#endif

#if defined(__GNUC__)
#include <tr1/memory>
#endif

//#define SINGLE_CURVATURE
namespace sqi
{
namespace alg
{

class BaseAlgorithm
{
public:
  
  BaseAlgorithm();
  virtual ~BaseAlgorithm();
  void PrintInfo();
  virtual void cal_tri_error(meshdata::MeshDataPtr& mesh_data ,
                             lq::InspectFlag flag);
  virtual void CalSingleProperty(meshdata::MeshDataPtr& mesh_data,
                                 lq::InspectFlag flag, lq::WindowFlag win);
  

  
  static double GenerateTriError(meshdata::MeshDataPtr& mesh_data,
                                 const std::vector<double>& vert_error, 
                                 lq::WindowFlag win = lq::kRight);
  protected:
  void GenerateFinalScalarField(meshdata::MeshDataPtr& mesh_data,
                                lq::InspectFlag flag, 
                                lq::WindowFlag win = lq::kRight);
  
  void MixAlgWithConvex(meshdata::MeshDataPtr& mesh_data,
                        lq::WindowFlag win = lq::kRight);
  
  void MixAlgWithConvexVertex(meshdata::MeshDataPtr& mesh_data,
                              lq::WindowFlag win = lq::kRight);
  
  void MixCurvatureWithInte(meshdata::MeshDataPtr& mesh_data,
                            lq::InspectFlag flag,
                            lq::WindowFlag win = lq::kRight);
  
};

}
}
#endif // BASEALGORITHM_H
