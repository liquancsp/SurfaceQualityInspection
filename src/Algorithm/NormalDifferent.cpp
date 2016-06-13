#include <NormalDifferent.h>
#include <data_type.h>
#include <ConsoleMessage.h>
#include <CurvatureAlg.h>

#include <ctime>
#include <iomanip>
namespace sqi
{
namespace alg
{
using namespace sqi::meshdata;
using namespace std;
using namespace lq;

NormalDifferent::NormalDifferent()
{
  
}

NormalDifferent::~NormalDifferent()
{
  
}

void NormalDifferent::cal_tri_error(meshdata::MeshDataPtr &mesh_data, lq::InspectFlag flag)
{
  sqi::io::ConsoleMessage<string>::SendMessage("", sqi::io::kStartAlg, 
                                               kNormalVectorAngle);
  assert(mesh_data.use_count() > 0);
  clock_t start, end;
  start = clock();
  for(size_t i = 0; i < 2; ++i)
  {
    if(!mesh_data->IsHasFNormal(i))
    {
      vcg::tri::UpdateNormal<Mesh>::PerFace(mesh_data->M(i));
      vcg::tri::UpdateNormal<Mesh>::NormalizePerFace(mesh_data->M(i));
      mesh_data->IsHasFNormal(i) = true;
    }
  }
  sqi::alg::CurvatureAlg<sqi::meshdata::Mesh>::UpdateConcavAndConvex(mesh_data->M(0),
                                                                     mesh_data->M(1));
  
  std::cout << "[#info-openmp]Thread number in integration reflection : " << 
               thread_num(mesh_data->FNum(0), MIN_ITERATOR_NUM) << endl;
  assert(mesh_data->FNum(0) == mesh_data->FNum(1));
  
#pragma omp parallel for num_threads(thread_num(mesh_data->FNum(0), MIN_ITERATOR_NUM))
  for(int i = 0; i < mesh_data->FNum(0); ++i)
  {
    FaceIterator fi1 = mesh_data->FBeginIter(0);
    FaceIterator fi2 = mesh_data->FBeginIter(1);
    fi1 += i;
    fi2 += i;
    double diff = 0.0f;
    Mesh::CoordType tmp;
    tmp = (*fi1).N() - (*fi2).N();
    diff = tmp.Norm();
    if(!is_zero(diff))
      (*fi2).Q() = diff;
    else
      (*fi2).Q() = 0.0f;
  }
  double max_error = 0.0f;
  for(FaceIterator fi = mesh_data->FBeginIter(1); 
      fi != mesh_data->FEndIter(1); ++fi)
  {
    if(is_larger((*fi).Q(), max_error))
      max_error = (*fi).Q();
  }
  mesh_data->MaxE() = max_error;
  BaseAlgorithm::GenerateFinalScalarField(mesh_data, flag);
  end = clock();
  io::ConsoleMessage<string>::SendMessage("", io::kEndAlg, lq::kNull, max_error,
                                          difftime(end, start)/CLOCKS_PER_SEC);
}

}
}
