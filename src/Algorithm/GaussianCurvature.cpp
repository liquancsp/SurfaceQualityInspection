#include <GaussianCurvature.h>
#include <data_type.h>
#include <ConsoleMessage.h>
#include <CurvatureAlg.h>
#include <iostream>
#include <fstream>
#include <ctime>
namespace sqi
{
namespace alg
{

using namespace std;
using namespace lq;

GaussianCurvature::GaussianCurvature()
{
  
}

GaussianCurvature::~GaussianCurvature()
{
  
}

void GaussianCurvature::cal_tri_error(meshdata::MeshDataPtr &mesh_data,
                                      lq::InspectFlag flag)
{
  io::ConsoleMessage<string>::SendMessage("", io::kStartAlg, kGaussianCurvature);
  assert(mesh_data.use_count() > 0);
  clock_t start, end;
  start = clock();
//  for(size_t i = 0; i < 2; ++i)
//  {
//    if(!vcg::tri::HasFFAdjacency(mesh_data->M(i)))
//      vcg::tri::UpdateTopology<sqi::meshdata::Mesh>::FaceFace(mesh_data->M(i));
//    if(!mesh_data->IsHasGaussCur(i))
//    {
//      vcg::tri::UpdateCurvature<sqi::meshdata::Mesh>::MeanAndGaussian(mesh_data->M(i));
//      mesh_data->IsHasMeanCur(i) = true;
//      mesh_data->IsHasGaussCur(i) = true;
//    }
//  }
  sqi::alg::CurvatureAlg<sqi::meshdata::Mesh>::UpdateConcavAndConvex(mesh_data->M(0),
                                                                     mesh_data->M(1));
  cout << "[#info]Finish curvature compute." << endl;
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
      double tmp = 0.0f;
      if((*vi2).IsB())
        tmp = 0.0f;
      else
        tmp = (*vi2).Kg();
#else
      double tmp =  (*vi2).Kg() - (*vi1).Kg();
#endif     
      if(!is_zero(tmp))
        vert_error[num++] = tmp;
      else
        vert_error[num++] = 0.0f;
    }
  }
  double max_error = 0.0f;
  max_error = BaseAlgorithm::GenerateTriError(mesh_data, vert_error, lq::kRight);
  BaseAlgorithm::GenerateFinalScalarField(mesh_data, flag, lq::kRight);
  end = clock();
  io::ConsoleMessage<string>::SendMessage("", io::kEndAlg, lq::kNull, max_error,
                                          difftime(end, start)/CLOCKS_PER_SEC);
}

void GaussianCurvature::CalSingleProperty(meshdata::MeshDataPtr &mesh_data, 
                                          InspectFlag flag, WindowFlag win)
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
      vert_error[num++] = vi->Kg();
  }
  GenerateTriError(mesh_data, vert_error, win);
  GenerateFinalScalarField(mesh_data, flag, win);
}

}
}
