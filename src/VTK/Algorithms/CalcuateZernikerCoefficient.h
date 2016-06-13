#ifndef CALCUATEZERNIKERCOEFFICIENT_H
#define CALCUATEZERNIKERCOEFFICIENT_H
#include <Algorithms/ZernikerPropagation.h>


namespace sqi{
namespace alg {

class CalcuateZernikerCoefficient : public ZernikerPropagation
{
  public:
  
  CalcuateZernikerCoefficient();
  ~CalcuateZernikerCoefficient();
  
  void CalSingleProperty(meshdata::MeshDataPtr& mesh_data, 
                         lq::InspectFlag flag, lq::WindowFlag win);
  
};

}

}
#endif // CALCUATEZERNIKERCOEFFICIENT_H
