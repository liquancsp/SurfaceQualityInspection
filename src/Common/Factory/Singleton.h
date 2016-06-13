#ifndef SINGLETON_H
#define SINGLETON_H
#if defined(_MSC_VER)
#include <memory>
#endif

#if defined(__GNUC__)
#include <tr1/memory>
#endif

namespace sqi {
namespace common {


template <class T>

class Singleton {
  
public :
  static inline T* Instance()
  {
    if(NULL == obj_instance_)
    {
      obj_instance_ =  new T;
    }
    return obj_instance_;
    //return std::tr1::shared_ptr<T>(obj_instance_);
  }
  
protected:
  
  
  Singleton() {}
  Singleton(const Singleton &other) {}
  ~Singleton() {}
  Singleton& operator = (const Singleton &other) {}
  class Release
  {
  public:
    ~Release()
    { 
      if (NULL != Singleton<T>::obj_instance_)
        delete Singleton<T>::obj_instance_;
    }
  };
  
  static T* obj_instance_;
  static Release release_;
};

template<class T>
T* Singleton<T>::obj_instance_ = NULL;
template<class T>
typename Singleton<T>::Release Singleton<T>::release_;

#define DeclarSingletonClass(type) \
       friend class Singleton<type>;
}
}
#endif // SINGLETON_H
