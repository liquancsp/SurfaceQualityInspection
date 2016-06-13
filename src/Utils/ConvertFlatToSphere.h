#ifndef CONVERTFLATTOSPHERE_H
#define CONVERTFLATTOSPHERE_H

#include <FunctionDisturb.h>

namespace sqi {
namespace utils {

class ConvertFlatToSphere : public FunctionDisturb
{
  
public:
  ConvertFlatToSphere();
  ~ConvertFlatToSphere();
  void Update(sqi::meshdata::MeshDataPtr mesh_data,
              const std::string& out_file, 
              vcg::Point3f& center, double radius);
};

}

}
#endif // CONVERTFLATTOSPHERE_H
