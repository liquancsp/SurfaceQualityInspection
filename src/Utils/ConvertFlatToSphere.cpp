#include <ConvertFlatToSphere.h>
#include <wrap/io_trimesh/export_obj.h>

using namespace std;

namespace sqi{
namespace utils{

ConvertFlatToSphere::ConvertFlatToSphere() : FunctionDisturb()
{
  
}

ConvertFlatToSphere::~ConvertFlatToSphere()
{
  
}

void ConvertFlatToSphere::Update(meshdata::MeshDataPtr mesh_data,
                                 const string &out_file, 
                                 vcg::Point3f &center, double radius)
{
  cout << "[#Info] Radius is : " << radius << endl;
  for(sqi::meshdata::VertexIterator vi = mesh_data->VBeginIter(lq::kLeft);
      vi != mesh_data->VEndIter(lq::kLeft); ++vi)
  {
    double d = vcg::Distance(vi->cP(), center);
    double l = sqrt(radius * radius - d * d);
    double delta_d = radius - l;
    vi->P()[2] -= delta_d;
  }
  int mark = 0;
  vcg::tri::io::ExporterOBJ<sqi::meshdata::Mesh>::Save(mesh_data->M(lq::kLeft), 
                                                       out_file.c_str(), mark);
}

}
}
