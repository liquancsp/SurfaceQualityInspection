#ifndef GLOBALLSINTEGRALREFL_H
#define GLOBALLSINTEGRALREFL_H

#include <BaseAlgorithm.h>
#include <menu_operate/GlobalAlign.h>
namespace sqi{
namespace alg{

class GlobalLSIntegralRefl : public BaseAlgorithm, public GlobalAlign
{
public:
  GlobalLSIntegralRefl();
  ~GlobalLSIntegralRefl();
  virtual void cal_tri_error(meshdata::MeshDataPtr &mesh_data, lq::InspectFlag flag);
};

}
}

#endif // GLOBALLSINTEGRALREFL_H
