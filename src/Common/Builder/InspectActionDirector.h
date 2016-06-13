#ifndef INSPECTACTIONDIRECTOR_H
#define INSPECTACTIONDIRECTOR_H
#include <Builder/InspectActionBuilder.h>

namespace sqi {
namespace common {

class InspectActionDirector 
{
public:
  
  InspectActionDirector(const sqi::meshdata::MeshDataPtr mesh,
                        const vtkSmartPointer<lq::vtk_model_data>& data)
    : mesh_data_(mesh), data_set_(data), 
      inspect_bulid_ptr_(new InspectActionBuilder(mesh, data))
  {
    
  }
  InspectActionDirector(){}
  
  void SetMeshData(sqi::meshdata::MeshDataPtr other)
  {
    this->mesh_data_ = other;
  }
  
  void SetDataSet(vtkSmartPointer<lq::vtk_model_data>& data)
  {
    this->data_set_ = data;
  }

  void ResetScene()
  {
    data_set_->ResetColorBar(lq::kLeft);
    data_set_->ResetColorBar(lq::kRight);
    if(mesh_data_->IsWinHas(lq::kLeft))
    {
      inspect_bulid_ptr_->CleanPolyColor(lq::kLeft);
      inspect_bulid_ptr_->ResetLightParameter(lq::kLeft);
    }
    if(mesh_data_->IsWinHas(lq::kRight))
    {
      inspect_bulid_ptr_->CleanPolyColor(lq::kRight);
      inspect_bulid_ptr_->ResetLightParameter(lq::kRight);
    }
  }
  
  lq::ReturnValue Execute(lq::InspectFlag flag, lq::AlgType type = lq::kCalTwoDiff)
  {
    if(type == lq::kCalTwoDiff)
    {
      if(mesh_data_->IsWinHas(lq::kLeft) && mesh_data_->IsWinHas(lq::kRight))
        Execute(flag, lq::kCalTwoDiff, lq::kRight);
      else 
        return lq::kError;
    }
    else if(type == lq::kCalSingleProperty)
    {
      std::cout << "in inspect Action Director " << std::endl;
      if(mesh_data_->IsWinHas(lq::kLeft))
        Execute(flag, lq::kCalSingleProperty, lq::kLeft);
      if(mesh_data_->IsWinHas(lq::kRight))
        Execute(flag, lq::kCalSingleProperty, lq::kRight);
    }
    return lq::kOK;
  }
  
  void Execute(lq::InspectFlag flag, lq::AlgType type, 
               lq::WindowFlag win)
  {
    if(flag == lq::kReflectionMap)
      inspect_bulid_ptr_.reset(new ReflectionAlgBuilder(mesh_data_, data_set_));
    else if(flag == lq::kIntegrateReflection)
      inspect_bulid_ptr_.reset(new IntegralAlgBuilder(mesh_data_, data_set_));
    else if(flag ==lq::kGlobalAlignInteRefl)
      inspect_bulid_ptr_.reset(new GlobalAlignInteBuilder(mesh_data_, data_set_));
    else if(flag == lq::kGlobalAlignNormalComponent)
      inspect_bulid_ptr_.reset(new GlobalAlignNormalComBuilder(mesh_data_, data_set_));
    else if(flag == lq::kLocalAlignIntegralRefl)
      inspect_bulid_ptr_.reset(new LocalAlignIntegralRefl(mesh_data_, data_set_));
    else if(flag == lq::kLocalAlignNormalDivision)
      inspect_bulid_ptr_.reset(new LocalAlignNormalDivision(mesh_data_, data_set_));
    else if(flag == lq::kGlobalLSIntegralRefl)
      inspect_bulid_ptr_.reset(new GlobalLSIntegralReflBulider(mesh_data_, data_set_));
    else if(flag == lq::kGlobalLSNormalComponent)
      inspect_bulid_ptr_.reset(new GlobalLSNormalComponent(mesh_data_, data_set_));
    else if(flag == lq::kConvexDiff)
      inspect_bulid_ptr_.reset(new ConvexDiffBuilder(mesh_data_, data_set_));
    else if(flag == lq::kMixReflConvex)
      inspect_bulid_ptr_.reset(new MixReflectionConvexBuilder(mesh_data_, data_set_));
    else if(flag == lq::kMixInteConvex)
      inspect_bulid_ptr_.reset(new MixIntegralConvexBuilder(mesh_data_, data_set_));
    else if(flag == lq::kMeanCurvature)
      inspect_bulid_ptr_.reset(new MeanCurAlgBuilder(mesh_data_, data_set_));
    else if(flag == lq::kGaussianCurvature)
      inspect_bulid_ptr_.reset(new GaussCurAlgBuilder(mesh_data_, data_set_));
    else if(flag == lq::kPrincipalK1)
      inspect_bulid_ptr_.reset(new PrincipalK1CurAlgBuilder(mesh_data_, data_set_));
    else if(flag == lq::kPrincipalK2)
      inspect_bulid_ptr_.reset(new PrincipalK2CurAlgBuilder(mesh_data_, data_set_));
    else if(flag == lq::kNormalVectorAngle)
      inspect_bulid_ptr_.reset(new NormalDiffAlgBuilder(mesh_data_, data_set_));
    else if(flag == lq::kDistanceDifferent)
      inspect_bulid_ptr_.reset(new DistanceDiffAlgBuilder(mesh_data_, data_set_));
    else if(flag == lq::kMixMeanInte)
      inspect_bulid_ptr_.reset(new MixMeanInteAlgBuilder(mesh_data_, data_set_));
    else if(flag == lq::kMixGaussInte)
      inspect_bulid_ptr_.reset(new MixGaussInteAlgBuilder(mesh_data_, data_set_));
    else if(flag == lq::kMixK1Inte)
      inspect_bulid_ptr_.reset(new MixK1InteAlgBuilder(mesh_data_, data_set_));
    else if(flag == lq::kMixK2Inte)
      inspect_bulid_ptr_.reset(new MixK2InteAlgBuilder(mesh_data_, data_set_));
    else if(flag == lq::kDefectFitting)
      inspect_bulid_ptr_.reset(new DefectFittingBuilder(mesh_data_, data_set_));
    else if(flag == lq::kZernikePropagation)
      inspect_bulid_ptr_.reset(new ZernikerCoeffPropagationBuilder(mesh_data_, data_set_));
    else if(flag == lq::kCalcuateZernikerCoefficient)
      inspect_bulid_ptr_.reset(new CalcuateZernikerCoefficientBuilder(mesh_data_, data_set_));
    else if(flag == lq::kTrainAndPredict)
      inspect_bulid_ptr_.reset(new TrainAndPredictBuilder(mesh_data_, data_set_));
    else if(flag == lq::kClassify || flag == lq::kZernikerClassify)
      inspect_bulid_ptr_.reset(new ZernikerClassifyBuilder(mesh_data_, data_set_));
    
    inspect_bulid_ptr_->InitParamters();
    inspect_bulid_ptr_->InitColor();
    inspect_bulid_ptr_->ComputeError(type, win);
    inspect_bulid_ptr_->GenerateMapColor(win);
    inspect_bulid_ptr_->MapColorOnModel(win);
    inspect_bulid_ptr_->SetParameters(win);
    inspect_bulid_ptr_->UpdateColorLegend(win);
  }
  
protected:
  
  sqi::meshdata::MeshDataPtr mesh_data_;
  vtkSmartPointer<lq::vtk_model_data> data_set_;
  std::tr1::shared_ptr<InspectActionBuilder> inspect_bulid_ptr_;
};

}
}

#endif // INSPECTACTIONDIRECTOR_H
