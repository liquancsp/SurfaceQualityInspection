#ifndef CURVATURE_FACTORY_H
#define CURVATURE_FACTORY_H

#include <BaseAlgorithm.h>
#include <MeanCurvature.h>
#include <GaussianCurvature.h>
#include <PrincipalCurvature.h>
#include <MixCurvatureInte.h>

#include <factory/Singleton.h>

namespace sqi {

namespace common {

class CurvatureFactory {
  
public:
  
  virtual ~CurvatureFactory() {}
  virtual sqi::alg::BaseAlgorithm* CreateCurvatureObj() = 0;
protected:
  CurvatureFactory() {}
  
};



class MeanCurvatureFactory : public CurvatureFactory 
{
public:
  
  sqi::alg::BaseAlgorithm* CreateCurvatureObj()
  {
    return new sqi::alg::MeanCurvature();
  }
  virtual ~MeanCurvatureFactory()
  {
    
  }
protected:
  MeanCurvatureFactory() {}
  friend class Singleton<MeanCurvatureFactory>;
};

class GaussCurvatureFactory : public CurvatureFactory 
{
public :
  
  sqi::alg::BaseAlgorithm* CreateCurvatureObj()
  {
    return new sqi::alg::GaussianCurvature();
  }
  virtual ~GaussCurvatureFactory() {}
protected:
  GaussCurvatureFactory(){}
  
  friend class Singleton<GaussCurvatureFactory>;
};

class PrincipalCurvatureFactory : public CurvatureFactory
{
public:
  
  sqi::alg::BaseAlgorithm* CreateCurvatureObj()
  {
    return new sqi::alg::PrincipalCurvature();
  }
  virtual ~PrincipalCurvatureFactory(){}
  
protected:
  PrincipalCurvatureFactory() : CurvatureFactory() {}
 
  friend class Singleton<PrincipalCurvatureFactory>;
};

class MixCurtrueInteFactory : public CurvatureFactory
{
public :
  
  
  sqi::alg::BaseAlgorithm* CreateCurvatureObj()
  {
    return new sqi::alg::MixCurvatureInte();
  }
  virtual ~MixCurtrueInteFactory(){}

protected:
  MixCurtrueInteFactory() : CurvatureFactory()
  {}
  friend class Singleton<MixCurtrueInteFactory>;
};

}

}
#endif
