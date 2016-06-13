#include <IntegrationReflection.h>
#include <integration_reflection_map.h>
#include <coordinate_rotation.h>
#include <ConsoleMessage.h>
#include <data_type.h>
#include <CurvatureAlg.h>

#include <ctime>
#include <iomanip>
#include <iostream>

namespace sqi
{
namespace alg
{
using namespace sqi::meshdata;
using namespace lq;
using namespace std;

IntegrationReflection::IntegrationReflection()
{
  
}

IntegrationReflection::~IntegrationReflection()
{
  
}

void IntegrationReflection::cal_tri_error(meshdata::MeshDataPtr &mesh_data, 
                                          lq::InspectFlag flag)
{
  GenerateIntegralReflMapList(mesh_data);
  io::ConsoleMessage<string>::SendMessage("", io::kStartAlg, kIntegrateReflection);
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
  CurvatureAlg<Mesh>::UpdateConcavAndConvex(mesh_data->M(0), mesh_data->M(1));
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
    if((*fi1).N() == (*fi2).N())
    {
      (*fi2).Q() = 0.0f;
      continue;
    }
    vec normal1((*fi1).N()[0], (*fi1).N()[1], (*fi1).N()[2]);
    vec normal2((*fi2).N()[0], (*fi2).N()[1], (*fi2).N()[2]);
    normal1 = normal1 / vec_norm(normal1);
    normal2 = normal2 / vec_norm(normal2);
    double angle = acos(dot_mult(normal1, normal2));
    angle = (angle * 180.0f) / PI;
    int diff = static_cast<int>(angle * 100);
    if(diff > mesh_data->IntegralReflList().size() - 1)
      diff =  mesh_data->IntegralReflList().size() - 1;
    (*fi2).Q() = mesh_data->IntegralReflList()[diff];
//    vec normal1((*fi1).N()[0], (*fi1).N()[1], (*fi1).N()[2]);
//    vec normal2((*fi2).N()[0], (*fi2).N()[1], (*fi2).N()[2]);
//    vec std_axis(0, 0, 1);
//    double normal_theta;
//    double value = 0.0f;
//    coordinate_rotation rotate_tool;
//    rotate_tool.execute_rotation(normal1, normal2, std_axis);
//    integration_reflection_map::get_normal_range_angle(normal1, normal2, normal_theta);
//    integration_reflection_map::cal_tri_integrate_error(normal1, normal2, 
//                                                        normal_theta, PI / 2, value);
//    value *= 4.0f;
//    if(!is_zero(value, meshdata::EPS_E6))
//      (*fi2).Q() = value;
//    else
//      (*fi2).Q() = 0.0f;
    
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

void IntegrationReflection::GenerateIntegralReflMapList(MeshDataPtr &mesh_data)
{
  if(mesh_data->IsHasIntegralReflList())
    return ;
  ifstream fin("./IntegralReflMapList.txt");
  if(fin.fail())
  {
    vec std_axis(0, 0, 1);
    double angle = 0.0f;
    lq::coordinate_rotation rotate_tool;
    vec normal1(0.0, 0.0, 0.0);
    vec normal2(0.0, 0.0, 0.0);
    double diff = 0.0f;
    mesh_data->IntegralReflList().clear();
    for(size_t i = 0; i <= 18000; ++i)
    {
      diff = 0.0f;
      if(i == 0)
      {
        mesh_data->IntegralReflList().push_back(0.0f);
        continue;
      }
      angle = i * 0.01f;
      angle = (angle * lq::PI) / 180.0f;
      angle /= 2.0f;
      
      rotate_tool.rotation_axis_with_angle(std_axis, angle, normal1);
      rotate_tool.rotation_axis_with_angle(std_axis, 0.0 - angle, normal2);
      integration_reflection_map::get_normal_range_angle(normal1, normal2, angle);
      integration_reflection_map::cal_tri_integrate_error(normal1, normal2, 
                                                          angle, PI / 2, diff);
      diff *= 4.0f;
      if(is_zero(diff, meshdata::EPS_E6))
        diff = 0.0f;
      mesh_data->IntegralReflList().push_back(diff);
    }
    ofstream fout("./IntegralReflMapList.txt");
    for(size_t i = 0; i < mesh_data->IntegralReflList().size(); ++i)
      fout << mesh_data->IntegralReflList()[i] << endl;
    fout.close();
  }
  else 
  {
    double tmp_error = 0.0f;
    mesh_data->IntegralReflList().clear();
    while(fin >> tmp_error)
    {
      mesh_data->IntegralReflList().push_back(tmp_error);
    }
  }
  mesh_data->IsHasIntegralReflList() = true;
  fin.close();
}
}
}
