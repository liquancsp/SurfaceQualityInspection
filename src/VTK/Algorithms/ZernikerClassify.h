#ifndef ZERNIKERCLASSIFY_H
#define ZERNIKERCLASSIFY_H
#include <BaseAlgorithm.h>
namespace sqi
{
namespace alg{

class ZernikerClassify : public BaseAlgorithm
{ 
public:
  ZernikerClassify();
  ~ZernikerClassify();
  void CalSingleProperty(meshdata::MeshDataPtr& mesh_data, 
                         lq::InspectFlag flag, lq::WindowFlag win);
};

}
}

#endif // ZERNIKERCLASSIFY_H
