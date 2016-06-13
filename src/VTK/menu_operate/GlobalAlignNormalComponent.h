#ifndef GLOBALALIGNNORMALCOMPONENT_H
#define GLOBALALIGNNORMALCOMPONENT_H

#include <menu_operate/GlobalAlign.h>
#include <BaseAlgorithm.h>

namespace sqi{
namespace alg{

class GlobalAlignNormalComponent : public BaseAlgorithm, public GlobalAlign
{
  
public :
  
  GlobalAlignNormalComponent();
  virtual ~GlobalAlignNormalComponent();
  virtual void cal_tri_error(meshdata::MeshDataPtr &mesh_data, lq::InspectFlag flag);
  
  
};


}
}

#endif // GLOBALALIGNNORMALCOMPONENT_H
