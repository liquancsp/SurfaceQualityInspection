#ifndef TRIGONOMETRYDISTURB_H
#define TRIGONOMETRYDISTURB_H

#include <FunctionDisturb.h>
#include <vector>
namespace sqi{
namespace utils{

class SineFunctionDisturb : public FunctionDisturb{
  
public:
  
 SineFunctionDisturb();
 ~SineFunctionDisturb();
 
 void Update(sqi::meshdata::MeshDataPtr mesh_data,
             const std::string& out_file,
             double amplitude, double phase);
};

}

}

#endif // TRIGONOMETRYDISTURB_H
