#ifndef VTKBULIDER_H
#define VTKBULIDER_H

#include <BaseAlgorithm.h>
#include <ReflectionMap.h>
#include <IntegrationReflection.h>
#include <MeanCurvature.h>
#include <GaussianCurvature.h>
#include <PrincipalCurvature.h>
#include <NormalDifferent.h>
#include <DistanceDifferent.h>
#include <menu_operate/edit_menu_operate.h>

namespace Sqi{

namespace VtkInterface {

class Builder{
  
  
public:
  virtual void ExecuteAlg() = 0;
  virtual void MapColor() 
  {
    
  }
  
  virtual void SetColorOnPloy() 
  {
    
  }
  
private:
  
};

class ReflectionMapBuilder : public Builder {
public:
  
  void ExecuteAlg();
  
  
  
};

class IntegralReflectionBuilder : public Builder 
{
  
}

}

}

#endif // VTKBULIDER_H
