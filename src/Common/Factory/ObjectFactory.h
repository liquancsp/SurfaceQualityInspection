#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H
//#define _CPPUNWIND
#include <boost/function/function0.hpp>
#include <MeshData.h>
#include <vtk_data/vtk_model_data.h>

#include <map>
#include <iterator>

namespace sqi{
namespace common {

template<typename obj_id, typename obj_type>
class ObjectFactory {
public:
  
  typedef boost::function<obj_type*()> construct_ptr;
  
  void RegisteObjCreator(const obj_id& id, const construct_ptr& creator)
  {
    obj_constrct_map_[id] = creator;
  }
  
  obj_type* MakeObject(const obj_id& id)
  {
    typename std::map<obj_id, construct_ptr>::const_iterator iter = 
        obj_constrct_map_.find(id);
    if(iter == obj_constrct_map_.end())
    {
      return NULL;
    }
    else
    {
      return (iter->second)();
    }
  }
  
private:
  std::map<obj_id, construct_ptr> obj_constrct_map_;
  
};

}
}

#endif // OBJECTFACTORY_H
