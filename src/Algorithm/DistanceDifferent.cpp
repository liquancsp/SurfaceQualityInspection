#include <DistanceDifferent.h>
#include <data_type.h>
#include <ConsoleMessage.h>
#include <CurvatureAlg.h>

#include <ctime>
namespace sqi
{
namespace alg
{
using namespace sqi::meshdata;
using namespace lq;
using namespace std;

DistanceDifferent::DistanceDifferent()
{
  
}

DistanceDifferent::~DistanceDifferent()
{
  
}

void DistanceDifferent::cal_tri_error(meshdata::MeshDataPtr& mesh_data, 
                                      lq::InspectFlag flag)
{
  sqi::io::ConsoleMessage<string>::SendMessage("",sqi::io::kStartAlg, 
                                               kDistanceDifferent,1);
  assert(mesh_data.use_count() > 0);
  clock_t start, end;
  start = clock();
  vector<double> vert_error;
  if(mesh_data->VNum(0) != mesh_data->VNum(1))
  {
    cerr << "[#Error]Two mesh vertex number are not same." << endl;
    cerr << "[#Error]Error happen in file : " << __FILE__ << " in line : " <<
            __LINE__ << endl;
    return ;
  }
  sqi::alg::CurvatureAlg<sqi::meshdata::Mesh>::UpdateConcavAndConvex(mesh_data->M(0),
                                                                     mesh_data->M(1));
  vert_error.resize(mesh_data->VNum(0));
  for(int i = 0; i < mesh_data->VNum(0); ++i)
  {
    //write like this can be paralleled 
    VertexIterator vi1 = mesh_data->VBeginIter(0);
    VertexIterator vi2 = mesh_data->VBeginIter(1);
    vi1 += i;
    vi2 += i;
    Mesh::CoordType tmp = (*vi1).P() - (*vi2).P();
    vert_error[i] = tmp.Norm();
  }
  double max_error = 0.0f;
  max_error = BaseAlgorithm::GenerateTriError(mesh_data, vert_error);
  BaseAlgorithm::GenerateFinalScalarField(mesh_data, flag);
  end = clock();
  io::ConsoleMessage<string>::SendMessage("", io::kEndAlg, lq::kNull, max_error,
                                          difftime(end, start)/CLOCKS_PER_SEC);
}
  
}
}
