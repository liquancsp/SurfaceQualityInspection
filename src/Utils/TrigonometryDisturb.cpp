#include <TrigonometryDisturb.h>

#include <wrap/io_trimesh/export_obj.h>

#include <cmath>
using namespace std;
namespace sqi{
namespace utils{

SineFunctionDisturb::SineFunctionDisturb() : FunctionDisturb()
{
  
}

SineFunctionDisturb::~SineFunctionDisturb()
{
  
}

void SineFunctionDisturb::Update(meshdata::MeshDataPtr mesh_data,
                                 const std::string& out_file,
                                 double amplitude, double phase)
{
  for(int i = 0; i < disturb_idx_.size(); ++i)
  {
    sqi::meshdata::VertexIterator vi = mesh_data->VBeginIter(lq::kLeft) + disturb_idx_[i];
    vi->P()[2] += amplitude * cos(phase * sqrt(vi->cP()[0] * vi->cP()[0] +  vi->cP()[1] * vi->cP()[1]));
  }
  int mark = 0;
  vcg::tri::io::ExporterOBJ<sqi::meshdata::Mesh>::Save(mesh_data->M(lq::kLeft), 
                                                       out_file.c_str(), mark);
}

}
}
