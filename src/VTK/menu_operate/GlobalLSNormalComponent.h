#ifndef GLOBALLSNORMALCOMPONENT_H
#define GLOBALLSNORMALCOMPONENT_H

#include <BaseAlgorithm.h>

namespace sqi {
namespace alg{

class GlobalLSNormalComponent : public BaseAlgorithm
{
public:
  GlobalLSNormalComponent();
  ~GlobalLSNormalComponent();
  virtual void cal_tri_error(meshdata::MeshDataPtr &mesh_data, lq::InspectFlag flag);
};

}

}

#endif // GLOBALLSNORMALCOMPONENT_H
