#include <SphereCapDisturb.h>
#include <wrap/io_trimesh/export_obj.h>

namespace sqi{
namespace utils{

SphereCapDisturb::SphereCapDisturb() : FunctionDisturb()
{
  
}

SphereCapDisturb::~SphereCapDisturb()
{
  
}

void SphereCapDisturb::Update(meshdata::MeshDataPtr mesh_data, 
                              const std::string &out_file, 
                              vcg::Point3f& center, double radius)
{
  std::cout << "[#Info]radius is : " << radius << std::endl; 
  for(sqi::meshdata::VertexIterator vi = mesh_data->VBeginIter(lq::kLeft);
      vi != mesh_data->VEndIter(lq::kLeft); ++vi)
  {
    double dist = vcg::Distance(vi->cP(), center);
    if(lq::is_less_equal(dist, radius))
    {
      vcg::Point3f normal(vi->cP()[0] - center[0], vi->cP()[1] - center[1],
          vi->cP()[2] - center[2]);
      normal.Normalize();
      vi->P() = vi->P() + normal * (radius - dist);
    }
  }
  int mark = 0;
  vcg::tri::io::ExporterOBJ<sqi::meshdata::Mesh>::Save(mesh_data->M(lq::kLeft), 
                                                       out_file.c_str(), mark);
  
}

}
}
