#include <ConcaveConvex.h>
#include <CurvatureAlg.h>

namespace sqi {
namespace alg {
using namespace lq;

ConcaveConvex::ConcaveConvex() : BaseAlgorithm()
{
  
}

ConcaveConvex::~ConcaveConvex()
{
  
}

void ConcaveConvex::cal_tri_error(meshdata::MeshDataPtr &mesh_data, 
                                  lq::InspectFlag flag)
{
  CurvatureAlg<sqi::meshdata::Mesh>::UpdateConcavAndConvex(mesh_data->M(kLeft),
                                                           mesh_data->M(kRight));
  sqi::meshdata::FaceHandleD convex_diff = mesh_data->GetConvexDiff(kRight);
  sqi::meshdata::FaceIterator fi1, fi2;
  double max_error = -10000.0f;
  double min_error = 10000.0f;
  for(fi1 = mesh_data->FBeginIter(kLeft), fi2 = mesh_data->FBeginIter(kRight);
      fi1 = mesh_data->FEndIter(kLeft), fi2 != mesh_data->FEndIter(kRight);
      ++fi1, ++fi2)
  {
   
    if(is_zero(convex_diff[*fi2], sqi::meshdata::EPS_E6))
      fi2->Q() = 0.0f;
    else
      fi2->Q() = convex_diff[*fi2];
    if(is_less(fi2->Q(), min_error))
      min_error = fi2->Q();
    if(is_larger(fi2->Q(), max_error))
      max_error = fi2->Q();
  }
  mesh_data->MaxE(lq::kRight) = max_error;
  mesh_data->MinE(lq::kRight) = min_error;
}

void ConcaveConvex::CalSingleProperty(meshdata::MeshDataPtr &mesh_data, 
                                      lq::InspectFlag flag, lq::WindowFlag win)
{
  CurvatureAlg<sqi::meshdata::Mesh>::GenerateConcaveConvex(mesh_data->M(win));
  sqi::meshdata::FaceHandleD value_convex_value = 
      mesh_data->GetConcaveConvexInfo(win);
  sqi::meshdata::FaceIterator fi;
  double max_error = -10000.0f;
  double min_error = 10000.0f;
  for(fi = mesh_data->FBeginIter(win); fi != mesh_data->FEndIter(win); ++fi)
  {
    if(is_zero(value_convex_value[*fi], sqi::meshdata::EPS_E6))
      fi->Q() = 0.0f;
    else 
      fi->Q() = value_convex_value[*fi];
    if(is_less(fi->Q(), min_error))
      min_error = fi->Q();
    if(is_larger(fi->Q(), max_error))
      max_error = fi->Q();
  }
  mesh_data->MaxE(win) = max_error;
  mesh_data->MinE(win) = min_error;
}

}

}