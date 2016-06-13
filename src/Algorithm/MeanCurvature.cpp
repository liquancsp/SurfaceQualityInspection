#include "MeanCurvature.h"
#include <data_type.h>
#include <vcg/complex/algorithms/update/curvature.h>
#include <ConsoleMessage.h>
#include <CurvatureAlg.h>
#include <vector>
#include <ctime>

namespace sqi
{
namespace alg
{

using namespace std;
using namespace lq;
MeanCurvature::MeanCurvature() : BaseAlgorithm()
{
  
}

MeanCurvature::~MeanCurvature()
{
  
}

void MeanCurvature::cal_tri_error(sqi::meshdata::MeshDataPtr& mesh_data,
                                  lq::InspectFlag flag)
{
  io::ConsoleMessage<string>::SendMessage("", io::kStartAlg, kMeanCurvature);
  std::cout << "-=-==-==" << std::endl;
  assert(mesh_data.use_count() > 0);
  clock_t start, end;
  start = clock();
  double max_error = 0.0f;
  sqi::alg::CurvatureAlg<sqi::meshdata::Mesh>::UpdateConcavAndConvex(mesh_data->M(0),
                                                                     mesh_data->M(1));
  vector<double> vert_error;
  vert_error.resize(mesh_data->M(0).VN());
  meshdata::Mesh::VertexIterator vi1, vi2;
  size_t num = 0;
  assert(mesh_data->VNum(0) == mesh_data->VNum(1));
  for(vi1 = mesh_data->M(0).vert.begin(), vi2 = mesh_data->M(1).vert.begin();
      vi1 != mesh_data->M(0).vert.end(), vi2 != mesh_data->M(1).vert.end() ; 
      ++vi1, ++vi2)
  {
    if(!(*vi1).IsD() && !(*vi2).IsD())
    {
#ifdef SINGLE_CURVATURE
      vert_error[num++] = (*vi2).Kh();
#else
      vert_error[num++] = (*vi2).Kh() - (*vi1).Kh();
#endif
    }
  }
  
  max_error = BaseAlgorithm::GenerateTriError(mesh_data, vert_error);
  BaseAlgorithm::GenerateFinalScalarField(mesh_data, flag);
  end = clock();
  io::ConsoleMessage<string>::SendMessage("", io::kEndAlg, lq::kNull, max_error,
                                          difftime(end, start)/CLOCKS_PER_SEC);
}

void MeanCurvature::CalSingleProperty(meshdata::MeshDataPtr &mesh_data, InspectFlag flag,
                                      WindowFlag win)
{
  if(!mesh_data->IsHasConvex(win)())
    CurvatureAlg<meshdata::Mesh>::MeanAndGaussian(mesh_data->M(win));
  vector<double> vert_error;
  vert_error.resize(mesh_data->M(win).VN());
  meshdata::Mesh::VertexIterator vi;
  size_t num = 0;
  for(vi = mesh_data->VBeginIter(win); vi != mesh_data->VEndIter(win); ++vi)
  {
    if(!vi->IsD())
      vert_error[num++] = vi->Kh();
  }
  BaseAlgorithm::GenerateTriError(mesh_data, vert_error, win);
  BaseAlgorithm::GenerateFinalScalarField(mesh_data, flag, win);
}

}
}
