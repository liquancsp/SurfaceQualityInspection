#ifndef CONVERTFLATTOCYLINDER_H
#define CONVERTFLATTOCYLINDER_H

#include <FunctionDisturb.h>

namespace sqi {
namespace utils {

class ConvertFlatToCylinder : public FunctionDisturb
{
  
public : 
  ConvertFlatToCylinder();
  ~ConvertFlatToCylinder();
  void Update(sqi::meshdata::MeshDataPtr mesh_data,
              const std::string& out_file, 
              vcg::Point3f& center, double radius);
  
};

}
}

#endif // CONVERTFLATTOCYLINDER_H
