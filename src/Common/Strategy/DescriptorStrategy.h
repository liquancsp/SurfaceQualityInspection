#ifndef DESCRIPTORSTRATEGY_H
#define DESCRIPTORSTRATEGY_H

#include <Algorithms/MeanCurvatureZerniker.h>
#include <Algorithms/GaussCurvatureZerniker.h>
#include <Algorithms/PrincipalCurvatureZerniker.h>
#include <Algorithms/CurvatureZerniker.h>
#include <Algorithms/CurvatureDescriptor.h>
#include <Algorithms/MeanCurvatureHistogram.h>
#include <Algorithms/CurvatureSmoothDiffZerniker.h>

#include <MeshData.h>
#include <vtk_data/vtk_model_data.h>
#include <Factory/ObjectFactory.h>
#include <boost/functional/factory.hpp>

namespace sqi{
namespace common{

class DescriptorStrategy {
  
public:
  DescriptorStrategy(lq::InspectFlag& flag, sqi::meshdata::MeshDataPtr mesh,
                     vtkSmartPointer<lq::vtk_model_data>& data) :
    alg_type_(flag), mesh_data_(mesh), data_set_(data)
  {
    std::cout << "[#Info-Init]Init descriptor strategy" << std::endl;
    SetAlgType(flag);
  }
  
  DescriptorStrategy() : obj_factory_(new ObjectFactory<lq::InspectFlag, sqi::alg::SurfaceDescriptor>())
  {
    std::cout << "[#Info-Init]Init descriptor strategy." << std::endl;
    alg_ptr_.resize(50);
    obj_factory_->RegisteObjCreator(lq::kCalMeanZernikerCoeff, boost::factory<sqi::alg::MeanCurvatureZerniker*>());
    obj_factory_->RegisteObjCreator(lq::kCalGaussZernikerCoeff, boost::factory<sqi::alg::GaussCurvatureZerniker*>());
    obj_factory_->RegisteObjCreator(lq::kCalK1ZernikerCoeff, boost::factory<sqi::alg::PrincipalCurvatureZerniker*>());
    obj_factory_->RegisteObjCreator(lq::kCalK2ZernikerCoeff, boost::factory<sqi::alg::PrincipalCurvatureZerniker*>());
    obj_factory_->RegisteObjCreator(lq::kCalCurvatureZernikerCoeff, boost::factory<sqi::alg::CurvatureZerniker*>());
    obj_factory_->RegisteObjCreator(lq::kCalCurvatureDescriptor, boost::factory<sqi::alg::CurvatureDescriptor*>());
    obj_factory_->RegisteObjCreator(lq::kCalCurvatureHistogram, boost::factory<sqi::alg::MeanCurvatureHistogram*>());
    obj_factory_->RegisteObjCreator(lq::kCalSmoothCurvatureDiff, boost::factory<sqi::alg::CurvatureSmoothDiffZerniker*>());
    for(size_t i = static_cast<size_t>(lq::kCalMeanZernikerCoeff);  
        i <= static_cast<size_t>(lq::kCalSmoothCurvatureDiff); ++i)
      alg_ptr_[i].reset(obj_factory_->MakeObject(static_cast<lq::InspectFlag>(i)));
    std::cout << "[#Info-Init]Init descriptor strategy successfully." << std::endl;
  }
  void SetAlgType(lq::InspectFlag flag)
  {
    this->alg_type_ = flag;
    
  }
  
  void SetMeshData(sqi::meshdata::MeshDataPtr other)
  {
    this->mesh_data_ = other;
  }
  
  void SetDataSet(vtkSmartPointer<lq::vtk_model_data>& other)
  {
    this->data_set_ = other;
  }
  
  void Execute(lq::WindowFlag win)
  {
    std::cout << "[#Info-Alg] inspect flag : " << alg_type_ << std::endl;
    alg_ptr_[alg_type_]->Descriptor(mesh_data_, data_set_, win, alg_type_);
  }

private:
  sqi::meshdata::MeshDataPtr mesh_data_;
  vtkSmartPointer<lq::vtk_model_data> data_set_;
  std::vector<std::tr1::shared_ptr<sqi::alg::SurfaceDescriptor> > alg_ptr_;
  lq::InspectFlag alg_type_;
  std::tr1::shared_ptr<ObjectFactory<lq::InspectFlag, sqi::alg::SurfaceDescriptor> > obj_factory_;
  
};

}
}

#endif // DESCRIPTORSTRATEGY_H
