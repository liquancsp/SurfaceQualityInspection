#ifndef SPHERECAPDISTURB_H
#define SPHERECAPDISTURB_H
#include <FunctionDisturb.h>

namespace sqi {
namespace utils{

class SphereCapDisturb : public FunctionDisturb{
  
public :
  SphereCapDisturb();
  ~SphereCapDisturb();
  
  void Update(sqi::meshdata::MeshDataPtr mesh_data,
              const std::string& out_file, 
              vcg::Point3f& center, double radius);
  
};

}

}
#endif // SPHERECAPDISTURB_H
