#ifndef ALGORITHMSTRATEGY_H
#define ALGORITHMSTRATEGY_H

//#include <Factory/Singleton.h>
//#include <Factory/CurvatureFactory.h>
//#include <Factory/ReflectionFactory.h>
#include <ReflectionMap.h>
#include <IntegrationReflection.h>
#include <menu_operate/GlobalLSIntegralRefl.h>
#include <menu_operate/GlobalLSNormalComponent.h>
#include <menu_operate/GlobalAlignIntegralMap.h>
#include <menu_operate/GlobalAlignNormalComponent.h>
#include <menu_operate/LocalRegisterIntegralMap.h>
#include <menu_operate/LocalAlignNormalDivision.h>
#include <ConcaveConvex.h>
#include <MixReflectionConvex.h>
#include <MixIntegralConvex.h>
#include <MeanCurvature.h>
#include <GaussianCurvature.h>
#include <PrincipalCurvature.h>
#include <NormalDifferent.h>
#include <DistanceDifferent.h>
#include <MixCurvatureInte.h>
#include <DefectFitting.h>
#include <Algorithms/ZernikerPropagation.h>
#include <Algorithms/ZernikerClassify.h>
#include <Algorithms/CalcuateZernikerCoefficient.h>

#include <Algorithms/ZernikerClassify.h>
#include <TrainAndPredict.h>

#include <MeshData.h>
#include <vtk_data/vtk_model_data.h>

#include <Factory/ObjectFactory.h>
#include <boost/functional/factory.hpp>

#include <algorithm>

namespace sqi {
namespace common {

class AlgorithmStrategy {
  
public:
  AlgorithmStrategy(lq::InspectFlag& flag, 
                    sqi::meshdata::MeshDataPtr mesh,
                    vtkSmartPointer<lq::vtk_model_data>& data) :
    alg_type_(flag), mesh_data_(mesh), data_set_(data)
  {
    
    SetAlgType(flag);
  }
  
  AlgorithmStrategy() : obj_factory_(new ObjectFactory<lq::InspectFlag, sqi::alg::BaseAlgorithm>())
  {
    std::cout << "[#Info-Init]Init algorithm strategy." << std::endl;
    //take advantage of boost factory and function class to complete factory pattern
    alg_ptr_.resize(64);
    obj_factory_->RegisteObjCreator(lq::kReflectionMap, boost::factory<sqi::alg::ReflectionMap *>());
    obj_factory_->RegisteObjCreator(lq::kIntegrateReflection, boost::factory<sqi::alg::IntegrationReflection*>());
    obj_factory_->RegisteObjCreator(lq::kGlobalAlignInteRefl, boost::factory<sqi::alg::GlobalAlignIntegralMap*>());
    obj_factory_->RegisteObjCreator(lq::kGlobalAlignNormalComponent, boost::factory<alg::GlobalAlignNormalComponent*>());
    obj_factory_->RegisteObjCreator(lq::kLocalAlignIntegralRefl, boost::factory<sqi::alg::LocalRegisterIntegralMap*>());
    obj_factory_->RegisteObjCreator(lq::kLocalAlignNormalDivision, boost::factory<sqi::alg::LocalAlignNormalDivision*>());
    obj_factory_->RegisteObjCreator(lq::kGlobalLSIntegralRefl, boost::factory<sqi::alg::GlobalLSIntegralRefl*>());
    obj_factory_->RegisteObjCreator(lq::kGlobalLSNormalComponent, boost::factory<sqi::alg::GlobalLSNormalComponent*>());
    obj_factory_->RegisteObjCreator(lq::kConvexDiff, boost::factory<sqi::alg::ConcaveConvex*>());
    obj_factory_->RegisteObjCreator(lq::kMixReflConvex, boost::factory<sqi::alg::MixReflectionConvex*> ());
    obj_factory_->RegisteObjCreator(lq::kMixInteConvex, boost::factory<sqi::alg::MixIntegralConvex*> ());
    obj_factory_->RegisteObjCreator(lq::kMeanCurvature, boost::factory<sqi::alg::MeanCurvature*> ());
    obj_factory_->RegisteObjCreator(lq::kGaussianCurvature, boost::factory<sqi::alg::GaussianCurvature*>());
    obj_factory_->RegisteObjCreator(lq::kPrincipalK1, boost::factory<sqi::alg::PrincipalCurvature*> ());
    obj_factory_->RegisteObjCreator(lq::kPrincipalK2, boost::factory<sqi::alg::PrincipalCurvature*> ());
    obj_factory_->RegisteObjCreator(lq::kNormalVectorAngle, boost::factory<sqi::alg::NormalDifferent*> ());
    obj_factory_->RegisteObjCreator(lq::kDistanceDifferent, boost::factory<sqi::alg::DistanceDifferent*> ());
    obj_factory_->RegisteObjCreator(lq::kMixMeanInte, boost::factory<sqi::alg::MixCurvatureInte*>());
    obj_factory_->RegisteObjCreator(lq::kMixGaussInte, boost::factory<sqi::alg::MixCurvatureInte*>());
    obj_factory_->RegisteObjCreator(lq::kMixK1Inte, boost::factory<sqi::alg::MixCurvatureInte*> ());
    obj_factory_->RegisteObjCreator(lq::kMixK2Inte, boost::factory<sqi::alg::MixCurvatureInte*> ());
    obj_factory_->RegisteObjCreator(lq::kDefectFitting, boost::factory<sqi::alg::DefectFitting*> ());
    obj_factory_->RegisteObjCreator(lq::kZernikePropagation, boost::factory<sqi::alg::ZernikerPropagation*> ());
    obj_factory_->RegisteObjCreator(lq::kCalcuateZernikerCoefficient, boost::factory<sqi::alg::CalcuateZernikerCoefficient*>());
    obj_factory_->RegisteObjCreator(lq::kZernikerClassify, boost::factory<sqi::alg::ZernikerClassify*>());
    obj_factory_->RegisteObjCreator(lq::kTrainAndPredict, boost::factory<sqi::alg::TrainAndPredict*>());
    obj_factory_->RegisteObjCreator(lq::kClassify, boost::factory<sqi::alg::ZernikerClassify*>());
    for(size_t i = static_cast<size_t>(lq::kReflectionMap);  i <= static_cast<size_t>(lq::kZernikerClassify); ++i)
      alg_ptr_[i].reset(obj_factory_->MakeObject(static_cast<lq::InspectFlag>(i)));
    
    alg_ptr_[static_cast<size_t>(lq::kTrainAndPredict)].reset(obj_factory_->MakeObject(lq::kTrainAndPredict));
    alg_ptr_[static_cast<size_t>(lq::kClassify)].reset(obj_factory_->MakeObject(lq::kClassify));
    
    //    use my own factory class
    //    alg_ptr_[lq::kReflectionMap].reset(Singleton<ReflectionMapFactory>::Instance()->CreateReflectionObj());
    //    alg_ptr_[lq::kIntegrateReflection].reset(Singleton<IntegralReflectionFactory>::Instance()->CreateReflectionObj());
    //    alg_ptr_[lq::kConvexDiff].reset(Singleton<ConcaveConvexFactory>::Instance()->CreateReflectionObj());
    //    alg_ptr_[lq::kMixReflConvex].reset(Singleton<MixReflConvexFactory>::Instance()->CreateReflectionObj());
    //    alg_ptr_[lq::kMixInteConvex].reset(Singleton<MixInteConvexFactory>::Instance()->CreateReflectionObj());
    //    alg_ptr_[lq::kMeanCurvature].reset(Singleton<MeanCurvatureFactory>::Instance()->CreateCurvatureObj());
    //    alg_ptr_[lq::kGaussianCurvature].reset(Singleton<GaussCurvatureFactory>::Instance()->CreateCurvatureObj());
    //    alg_ptr_[lq::kPrincipalK1].reset(Singleton<PrincipalCurvatureFactory>::Instance()->CreateCurvatureObj());
    //    alg_ptr_[lq::kPrincipalK2].reset(Singleton<PrincipalCurvatureFactory>::Instance()->CreateCurvatureObj());
    //    alg_ptr_[lq::kNormalVectorAngle].reset(Singleton<NormalDiffFactory>::Instance()->CreateReflectionObj());
    //    alg_ptr_[lq::kDistanceDifferent].reset(Singleton<DistDiffFactory>::Instance()->CreateReflectionObj());
    //    alg_ptr_[lq::kMixMeanInte].reset(Singleton<MixCurtrueInteFactory>::Instance()->CreateCurvatureObj());
    //    alg_ptr_[lq::kMixGaussInte].reset(Singleton<MixCurtrueInteFactory>::Instance()->CreateCurvatureObj());
    //    alg_ptr_[lq::kMixK1Inte].reset(Singleton<MixCurtrueInteFactory>::Instance()->CreateCurvatureObj());
    //    alg_ptr_[lq::kMixK2Inte].reset(Singleton<MixCurtrueInteFactory>::Instance()->CreateCurvatureObj());
  }
  
  void SetAlgType(lq::InspectFlag flag)
  {
    this->alg_type_ = flag;
    
  }
  
  void SetAlgPtr(sqi::alg::BaseAlgorithm* ptr)
  {
    //this->alg_ptr_[].reset(ptr);
  }
  
  void SetMeshData(sqi::meshdata::MeshDataPtr other)
  {
    this->mesh_data_ = other;
  }
  
  void SetDataSet(vtkSmartPointer<lq::vtk_model_data>& other)
  {
    this->data_set_ = other;
  }
  
  void CalTwoModelDiff()
  {
    std::cout << "inspect flag : " << alg_type_ << std::endl;
    alg_ptr_[alg_type_]->cal_tri_error(mesh_data_, alg_type_);
  }
  
  void CalOneModelProperty(lq::WindowFlag win)
  {
    alg_ptr_[alg_type_]->CalSingleProperty(mesh_data_, alg_type_, win);
  }
  
private:
  
  sqi::meshdata::MeshDataPtr mesh_data_;
  vtkSmartPointer<lq::vtk_model_data> data_set_;
  std::vector<std::tr1::shared_ptr<sqi::alg::BaseAlgorithm> > alg_ptr_; 
  lq::InspectFlag alg_type_;
  std::tr1::shared_ptr<ObjectFactory<lq::InspectFlag, sqi::alg::BaseAlgorithm> > obj_factory_;
  
  
};

}

}

#endif // ALGORITHMSTRATEGY_H
