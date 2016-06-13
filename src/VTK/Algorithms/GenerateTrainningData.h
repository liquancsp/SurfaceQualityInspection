#ifndef GENERATETRAINNINGDATA_H
#define GENERATETRAINNINGDATA_H

#include <MeshData.h>

namespace sqi{
namespace alg{

class GenerateTrainningData {
  
public:
  GenerateTrainningData();
  ~GenerateTrainningData();
  
  void CreateTrainningData(meshdata::MeshDataPtr &mesh_data, 
                           lq::WindowFlag win);
  
  void CreateTrainningData(meshdata::MeshDataPtr &mesh_data, 
                           lq::WindowFlag win);
  
};

}
}

#endif // GENERATETRAINNINGDATA_H
