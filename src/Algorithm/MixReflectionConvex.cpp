#include <MixReflectionConvex.h>
#include <data_type.h>
#include <fstream>
namespace sqi{
namespace alg {
using namespace lq;

MixReflectionConvex::MixReflectionConvex() : ReflectionMap()
{
  
}

MixReflectionConvex::~MixReflectionConvex()
{
  
}

void MixReflectionConvex::cal_tri_error(meshdata::MeshDataPtr &mesh_data, 
                                        lq::InspectFlag flag)
{
  ReflectionMap::cal_tri_error(mesh_data, flag);
  BaseAlgorithm::MixAlgWithConvex(mesh_data, lq::kRight);
}

}

}
