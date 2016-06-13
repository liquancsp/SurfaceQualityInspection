#ifndef REFLECTION_FACTORY_H
#define REFLECTION_FACTORY_H
#include <BaseAlgorithm.h>
#include <ReflectionMap.h>
#include <IntegrationReflection.h>
#include <ConcaveConvex.h>
#include <MixReflectionConvex.h>
#include <MixIntegralConvex.h>
#include <NormalDifferent.h>
#include <DistanceDifferent.h>
#include <factory/Singleton.h>

namespace sqi {
namespace common {

class ReflectionFactory {
public:
  virtual ~ReflectionFactory(){}
  virtual sqi::alg::BaseAlgorithm* CreateReflectionObj() = 0;
protected:
  ReflectionFactory() {}
  
  //DeclarSingletonClass(ReflectionFactory);
  //friend class Singleton<ReflectionFactory>;
};


class ReflectionMapFactory : public ReflectionFactory 
{
public:
  
  sqi::alg::BaseAlgorithm* CreateReflectionObj()
  {
    return new sqi::alg::ReflectionMap();
  }
  
  ~ReflectionMapFactory()
  {
  }

protected:
  
  ReflectionMapFactory() {}
  friend class Singleton<ReflectionMapFactory>;
};


class IntegralReflectionFactory : public ReflectionFactory
{
public :
  sqi::alg::BaseAlgorithm* CreateReflectionObj()
  {
    return new sqi::alg::IntegrationReflection();
  }
protected:
  
  IntegralReflectionFactory() {}
  friend class Singleton<IntegralReflectionFactory>;
};

class ConcaveConvexFactory : public ReflectionFactory
{
public :
  sqi::alg::BaseAlgorithm* CreateReflectionObj()
  {
    return new sqi::alg::ConcaveConvex();
  }
  
protected:
  ConcaveConvexFactory(){}
  friend class Singleton<ConcaveConvexFactory>;
};

class MixReflConvexFactory : public ReflectionFactory
{
public :
  
  sqi::alg::BaseAlgorithm* CreateReflectionObj()
  {
    return new sqi::alg::MixReflectionConvex();
  }
  
protected:
  MixReflConvexFactory(){}
  friend class Singleton<MixReflConvexFactory>;
};

class MixInteConvexFactory : public ReflectionFactory
{
public:
  
  sqi::alg::BaseAlgorithm* CreateReflectionObj()
  {
    return new sqi::alg::MixIntegralConvex();
  }
protected:
  
  MixInteConvexFactory(){}
  
  friend class Singleton<MixInteConvexFactory>;
};

class NormalDiffFactory : public ReflectionFactory
{
public:
  
  sqi::alg::BaseAlgorithm* CreateReflectionObj()
  {
    return new sqi::alg::NormalDifferent();
  }
  ~NormalDiffFactory(){}
protected:
  NormalDiffFactory() {}
  
  friend class Singleton<NormalDiffFactory>;
  
};

class DistDiffFactory : public ReflectionFactory 
{
public:
  
  sqi::alg::BaseAlgorithm* CreateReflectionObj()
  {
    return new sqi::alg::DistanceDifferent();
  }
  ~DistDiffFactory(){}
protected:
  DistDiffFactory() {}
  
  friend class Singleton<DistDiffFactory>;
};

}
}

#endif
