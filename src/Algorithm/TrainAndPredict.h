#ifndef TRAINANDPREDICT_H
#define TRAINANDPREDICT_H
#include <BaseAlgorithm.h>

namespace sqi {
namespace alg {

class TrainAndPredict : public BaseAlgorithm
{
public:
  TrainAndPredict();
  ~TrainAndPredict();
  
  void CalSingleProperty(meshdata::MeshDataPtr &mesh_data, 
                         lq::InspectFlag flag, 
                         lq::WindowFlag win);
};

}

}
#endif // TRAINANDPREDICT_H
