#include <PrincipalCurvature.h>
#include <CurvatureAlg.h>
#include <data_type.h>
#include <ConsoleMessage.h>
#include <ctime>
namespace sqi
{
namespace alg
{
using namespace sqi::meshdata;
using namespace sqi::alg;
using namespace lq;
using namespace std;

PrincipalCurvature::PrincipalCurvature()
{
  
}

PrincipalCurvature::~PrincipalCurvature()
{
}

void PrincipalCurvature::cal_tri_error(meshdata::MeshDataPtr& mesh_data, 
                                       lq::InspectFlag flag)
{
  io::ConsoleMessage<string>::SendMessage("", io::kStartAlg, kGaussianCurvature);
  assert(mesh_data.use_count() > 0);
  clock_t start, end;
  start = clock();
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
      if(!mesh_data->IsHasPrinCur())
      {
        double delta1 = 0.0f, delta2 = 0.0f;
        delta1 = (*vi1).Kh() * (*vi1).Kh() - (*vi1).Kg();
        delta2 = (*vi2).Kh() * (*vi2).Kh() - (*vi2).Kg();
        if(is_less(delta1, 0.0f))
          delta1 = 0.0f;
        if(is_less(delta2, 0.0f))
          delta2 = 0.0f;
        (*vi1).K1() = (*vi1).Kh() + sqrt(delta1);
        (*vi1).K2() = (*vi1).Kh() - sqrt(delta1);
        (*vi2).K1() = (*vi2).Kh() + sqrt(delta2);
        (*vi2).K2() = (*vi2).Kh() - sqrt(delta2);
      }
      double tmp = 0.0f;
      if(flag == kPrincipalK1)
      {
        tmp = (*vi2).K1() - (*vi1).K1() ;
      }
      else if(flag == kPrincipalK2)
      {
        tmp = (*vi2).K2() - (*vi1).K2();
      }
//      if(!is_zero(tmp))
//        vert_error[num++] = tmp;
//      else
//        vert_error[num++] = 0.0f;
      vert_error[num++] = tmp;
    }
  }
  double max_error = 0.0f;
  max_error = BaseAlgorithm::GenerateTriError(mesh_data, vert_error);
  BaseAlgorithm::GenerateFinalScalarField(mesh_data, flag);
  end = clock();
  io::ConsoleMessage<string>::SendMessage("", io::kEndAlg, lq::kNull, max_error,
                                          difftime(end, start)/CLOCKS_PER_SEC);
}

void PrincipalCurvature::CalSingleProperty(MeshDataPtr &mesh_data, InspectFlag flag, 
                                           WindowFlag win)
{
  if(!mesh_data->IsHasConvex(win)())
  {
    CurvatureAlg<sqi::meshdata::Mesh>::MeanAndGaussian(mesh_data->M(win));
  }
  meshdata::Mesh::VertexIterator vi;
  size_t num = 0;
  vector<double> vert_error;
  vert_error.resize(mesh_data->M(win).VN());
  CurvatureAlg<meshdata::Mesh>::PrincipalCurvatureAndDirection(mesh_data->M(win)); 
  if(flag == lq::kPrincipalK1)
  {
    for(vi = mesh_data->VBeginIter(win); vi != mesh_data->VEndIter(win); ++vi)
      vert_error[num++] = (*vi).K1();
  }
  else if(flag == lq::kPrincipalK2)
  {
    for(vi = mesh_data->VBeginIter(win); vi != mesh_data->VEndIter(win); ++vi)
      vert_error[num++] = (*vi).K2();
  }
  BaseAlgorithm::GenerateTriError(mesh_data, vert_error, win);
  BaseAlgorithm::GenerateFinalScalarField(mesh_data, flag, win);
}

}
}
