#include <CalcuateZernikerCoefficient.h>

namespace sqi{
namespace alg{

CalcuateZernikerCoefficient::CalcuateZernikerCoefficient() 
{
  
}

CalcuateZernikerCoefficient::~CalcuateZernikerCoefficient()
{
  
}

void CalcuateZernikerCoefficient::CalSingleProperty(meshdata::MeshDataPtr &mesh_data, 
                                                    lq::InspectFlag flag, lq::WindowFlag win)
{
  std::string out_file = ZernikerPropagation::data_set_->ModelName(win);
  int flag1 = out_file.find_last_of("/");
  int flag2 = out_file.find_last_of(".");
  out_file = out_file.substr(flag1 + 1, flag2 - flag1 - 1);
  out_file += ".test";
  ZernikerPropagation::CreateTestData(mesh_data, out_file, win);
  cout << "[#Info]Out put test file is : " << out_file << endl;
}

}
}
