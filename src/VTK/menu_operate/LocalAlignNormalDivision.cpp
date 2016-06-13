#include <menu_operate/LocalAlignNormalDivision.h>
//#include <Topology/Geodesic.h>
#include <ConsoleMessage.h>
#include <IntegrationReflection.h>

#include <vtkSmartPointer.h>
#include <vtkMatrix4x4.h>

#include <ctime>


namespace sqi{
namespace alg{

using namespace std;
using namespace lq;
using namespace sqi::meshdata;

LocalAlignNormalDivision::LocalAlignNormalDivision() : BaseAlgorithm(), LocalAlign()
{
  
}

LocalAlignNormalDivision::~LocalAlignNormalDivision()
{
  
}

void LocalAlignNormalDivision::cal_tri_error(meshdata::MeshDataPtr &mesh_data, lq::InspectFlag flag)
{
  IntegrationReflection::GenerateIntegralReflMapList(mesh_data);
  io::ConsoleMessage<std::string>::MessageText<double>("radius is :", io::kInfo, mesh_data->LocalRadius());
  clock_t start, end;
  start = clock();
//  stringstream s1 ;
//  s1 << mesh_data->PreLocalRadius();
//  stringstream s2;
//  s2 << mesh_data->LocalRadius();
 // std::cout << "pre radius : " << s1.str() << "  radius : " << s2.str() << std::endl;
  //bool is_computed = (strcmp(s1.str().c_str(), s2.str().c_str()) == 0);
  bool is_computed = lq::equal(mesh_data->PreLocalRadius(), mesh_data->LocalRadius());
  LocalAlign::ExecuteComputeErrorUseFace(mesh_data, flag, is_computed);
//  LocalAlign::ExecuteComputeErrorUseVertex(mesh_data);
  BaseAlgorithm::GenerateFinalScalarField(mesh_data, flag);
  end = clock();
  io::ConsoleMessage<string>::SendMessage("", io::kEndAlg, lq::kNull, mesh_data->MaxE(),
                                          difftime(end, start)/CLOCKS_PER_SEC);
}

}

}
