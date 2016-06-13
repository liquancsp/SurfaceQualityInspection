#include <ReflectionMap.h>
#include <CurvatureAlg.h>
#include <data_type.h>
#include <ConsoleMessage.h>


#include <iostream>
#include <ctime>
#include <iomanip>
namespace sqi
{
namespace alg
{
using namespace sqi::meshdata;
using namespace lq;
using namespace std;

ReflectionMap::ReflectionMap()
{
}

ReflectionMap::~ReflectionMap()
{
  
}
  
void ReflectionMap::cal_tri_error(meshdata::MeshDataPtr& mesh_data, lq::InspectFlag flag)
{
  //assert(mesh_data.use_count() > 0);
  std::cout << "in cal_tr_error" << std::endl;
  io::ConsoleMessage<string>::SendMessage("", io::kStartAlg, lq::kReflectionMap);
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
  size_t error_num = 0.0;
  io::ConsoleMessage<string>::MessageText("Face number is : ", io::kInfoAlg, 
                                          mesh_data->FNum(0));
  sqi::meshdata::Mesh::CoordType view_point(-8912, 283, 1368);
  mesh_data->VP() = view_point;
  cout << "[#Info]View Point is : " << mesh_data->VP()[0] << " " << mesh_data->VP()[1] <<
          " " << mesh_data->VP()[2] << endl;
#pragma omp parallel for num_threads(thread_num(mesh_data->FNum(0), MIN_ITERATOR_NUM))
  for(int i = 0; i < mesh_data->FNum(0); ++i)
  {
    
    double diff = 0.0f;
    Mesh::CoordType center1, center2;
    FaceIterator fi1 = mesh_data->FBeginIter(0);
    FaceIterator fi2 = mesh_data->FBeginIter(1);
    fi1 += i;
    fi2 += i;
    get_tri_center(mesh_data, fi1, 0, center1);
    get_tri_center(mesh_data, fi2, 1, center2);
    if((*fi1).N() == (*fi2).N() || IsValidViewPoint(center1, center2, 
                                                    (*fi1).N(), (*fi2).N(), mesh_data->VP()))
    {
      (*fi2).Q() = 0.0f;// it is very important for MSVC compiler, because MSVC didn't set 0.0 as it default value
      continue;
    }
   
    cal_normal_diff(center1, center2, (*fi1).N(), (*fi2).N(), 
                    mesh_data->VP(), diff);
  
    if(!is_zero(diff))
    {
      (*fi2).Q() = diff;
      error_num++;
      
    }
    else 
      (*fi2).Q() = 0.0f;
  
  }
  BaseAlgorithm::GenerateFinalScalarField(mesh_data, flag);
  end = clock();
  io::ConsoleMessage<string>::SendMessage("", io::kEndAlg, lq::kNull, mesh_data->MinE(),
                                          difftime(end, start)/CLOCKS_PER_SEC);
}


//------------------------------------------------------------------------------
//begin private function 
//judge wether the view point is valid for the two normal
bool ReflectionMap::IsValidViewPoint(const sqi::meshdata::Mesh::CoordType& center1,
                                     const sqi::meshdata::Mesh::CoordType& center2,
                                     const sqi::meshdata::Mesh::CoordType& normal1,
                                     const sqi::meshdata::Mesh::CoordType& normal2,
                                     const sqi::meshdata::Mesh::CoordType& view_point)
{
  if(is_less_equal(normal1 * (view_point - center1), sqi::meshdata::EPS_E6) ||
     is_less_equal(normal2 * (view_point - center2), sqi::meshdata::EPS_E6))
    return true;
  else 
    return false;
}

}
}
