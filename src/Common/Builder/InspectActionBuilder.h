#ifndef INSPECTACTIONBUILDER_H
#define INSPECTACTIONBUILDER_H
#include <MeshData.h>
#include <vtk_data/vtk_model_data.h>
#include <Facade/MenuOperator.h>

namespace sqi {
namespace common {

class InspectActionBuilder 
{
  
public:
  InspectActionBuilder(const sqi::meshdata::MeshDataPtr mesh,
                       const vtkSmartPointer<lq::vtk_model_data>& data) 
    : mesh_data_(mesh), data_set_(data), 
      menu_operator_(new MenuOperator(mesh, data))
  {
  }
  virtual ~InspectActionBuilder(){}
  
  virtual void InitParamters()
  {
    menu_operator_->InitParamters();
  }
  virtual void ComputeError(const lq::AlgType type = lq::kCalTwoDiff,
                            const lq::WindowFlag win = lq::kRight){}
  virtual void InitColor()
  {
    menu_operator_->InitColor();
  }

  virtual void GenerateMapColor(lq::WindowFlag win)
  {
    menu_operator_->GenerateMapColor(win);
  }
  
  virtual void MapColorOnModel(lq::WindowFlag win)
  {
    menu_operator_->MapColorOnModel(win);
  }
  
  virtual void SetParameters(lq::WindowFlag win = lq::kRight)
  {
    menu_operator_->SetParameters(win);
  }
  
  virtual void UpdateColorLegend(lq::WindowFlag win = lq::kRight)
  {
    menu_operator_->UpdateColorLegend(win);
  }
  
  virtual void CleanPolyColor(lq::WindowFlag win = lq::kRight)
  {
    menu_operator_->CleanPolyColor(win);
  }
  
  virtual void ResetLightParameter(lq::WindowFlag win)
  {
    menu_operator_->ResetLightParameter(win);
  }
 
protected:
  InspectActionBuilder(){}
  void ExecCompute(const lq::InspectFlag flag,
                   const lq::AlgType type,
                   const lq::WindowFlag win)
  {
    data_set_->AlgType() = flag;
    if(type == lq::kCalTwoDiff)
      menu_operator_->ComputeError(flag, type, win);
    else if(type == lq::kCalSingleProperty)
    {
      std::cout << "in inspectActionBuilder ExecCompute" << std::endl;
      if((flag >= lq::kMeanCurvature && flag <= lq::kPrincipalK2) ||
         flag == lq::kConvexDiff || (flag >= lq::kDefectFitting && flag <= lq::kZernikerClassify)
         || flag == lq::kClassify || flag == lq::kTrainAndPredict)
        menu_operator_->ComputeError(flag, type, win);
    }
  }
  
  sqi::meshdata::MeshDataPtr mesh_data_;
  vtkSmartPointer<lq::vtk_model_data> data_set_;
  std::tr1::shared_ptr<MenuOperator> menu_operator_;
};

class ReflectionAlgBuilder : public InspectActionBuilder
{
public:
  
  ReflectionAlgBuilder(sqi::meshdata::MeshDataPtr mesh,
                       vtkSmartPointer<lq::vtk_model_data>& data) 
    : InspectActionBuilder(mesh, data)
  {
    
  }
  virtual void InitParamters()
  {
    double camera_pos[3];
    data_set_->CM()->GetPosition(camera_pos);
    sqi::meshdata::Mesh::CoordType view_point(camera_pos[0], camera_pos[1], camera_pos[2]);
    mesh_data_->VP() = view_point;
    InspectActionBuilder::InitParamters();
  }

  virtual void ComputeError(const lq::AlgType type = lq::kCalTwoDiff,
                            const lq::WindowFlag win = lq::kRight)
  {
    ExecCompute(lq::kReflectionMap, type, win);
  }

  ~ReflectionAlgBuilder(){}
  
protected:
  
  ReflectionAlgBuilder(){}
  
};

class IntegralAlgBuilder : public InspectActionBuilder
{
public:
  
  IntegralAlgBuilder(sqi::meshdata::MeshDataPtr mesh,
                       vtkSmartPointer<lq::vtk_model_data>& data) 
    : InspectActionBuilder(mesh, data)
  {
    
  }
 
  virtual void ComputeError(const lq::AlgType type = lq::kCalTwoDiff,
                            const lq::WindowFlag win = lq::kRight)
  {
    InspectActionBuilder::ExecCompute(lq::kIntegrateReflection, type, win);
  }

  ~IntegralAlgBuilder(){}
  
protected:
  
  IntegralAlgBuilder(){}
  
};

class GlobalAlignInteBuilder : public InspectActionBuilder
{
public:
  GlobalAlignInteBuilder(sqi::meshdata::MeshDataPtr mesh,
                         vtkSmartPointer<lq::vtk_model_data>& data)
    : InspectActionBuilder(mesh, data)
  {
    
  }
  virtual void ComputeError(const lq::AlgType type = lq::kCalTwoDiff,
                            const lq::WindowFlag win = lq::kRight)
  {
    InspectActionBuilder::ExecCompute(lq::kGlobalAlignInteRefl, type, win);
  }
};

class GlobalAlignNormalComBuilder : public InspectActionBuilder
{
public:
  
  GlobalAlignNormalComBuilder(meshdata::MeshDataPtr mesh, 
                              vtkSmartPointer<lq::vtk_model_data> &data)
    : InspectActionBuilder(mesh, data)
  {
    
  }
  
  virtual void ComputeError(const lq::AlgType type = lq::kCalTwoDiff,
                            const lq::WindowFlag win = lq::kRight)
  {
    InspectActionBuilder::ExecCompute(lq::kGlobalAlignNormalComponent, type, win);
  }
};

class LocalAlignIntegralRefl : public InspectActionBuilder
{
public:
  LocalAlignIntegralRefl(sqi::meshdata::MeshDataPtr mesh,
                         vtkSmartPointer<lq::vtk_model_data>& data)
    : InspectActionBuilder(mesh, data)
  {
    
  }
  virtual void ComputeError(const lq::AlgType type = lq::kCalTwoDiff, 
                            const lq::WindowFlag win = lq::kRight)
  {
    InspectActionBuilder::ExecCompute(lq::kLocalAlignIntegralRefl, type, win);
  }
};

class LocalAlignNormalDivision : public InspectActionBuilder
{
public:
  LocalAlignNormalDivision(sqi::meshdata::MeshDataPtr mesh,
                           vtkSmartPointer<lq::vtk_model_data>& data)
      : InspectActionBuilder(mesh, data)
  {
    
  }
  virtual void ComputeError(const lq::AlgType type = lq::kCalTwoDiff, 
                            const lq::WindowFlag win = lq::kRight)
  {
    InspectActionBuilder::ExecCompute(lq::kLocalAlignNormalDivision, type, win);
  }
};

class GlobalLSIntegralReflBulider : public InspectActionBuilder
{
public :
  GlobalLSIntegralReflBulider(sqi::meshdata::MeshDataPtr mesh,
                              vtkSmartPointer<lq::vtk_model_data>& data)
    : InspectActionBuilder(mesh, data)
  {
    
  }
  virtual void ComputeError(const lq::AlgType type = lq::kCalTwoDiff, 
                            const lq::WindowFlag win = lq::kRight)
  {
    InspectActionBuilder::ExecCompute(lq::kGlobalLSIntegralRefl, type, win);
  }
  
};

class GlobalLSNormalComponent : public InspectActionBuilder
{
public :
  GlobalLSNormalComponent(sqi::meshdata::MeshDataPtr mesh,
                          vtkSmartPointer<lq::vtk_model_data>& data)
    : InspectActionBuilder(mesh, data)
  {
    
  }
  virtual void ComputeError(const lq::AlgType type = lq::kCalTwoDiff, 
                            const lq::WindowFlag win = lq::kRight)
  {
    InspectActionBuilder::ExecCompute(lq::kGlobalLSNormalComponent, type, win);
  }
};

class ConvexDiffBuilder : public InspectActionBuilder
{
public: 
  ConvexDiffBuilder(sqi::meshdata::MeshDataPtr mesh,
                    vtkSmartPointer<lq::vtk_model_data>& data) 
    : InspectActionBuilder(mesh, data)
  {
    
  }
  ~ConvexDiffBuilder(){}
  
  virtual void ComputeError(const lq::AlgType type = lq::kCalTwoDiff, 
                            const lq::WindowFlag win = lq::kRight)
  {
    InspectActionBuilder::ExecCompute(lq::kConvexDiff, type, win);
  }
  
protected:
  ConvexDiffBuilder(){}
};

class MixReflectionConvexBuilder : public ReflectionAlgBuilder
{
public : 
  MixReflectionConvexBuilder(sqi::meshdata::MeshDataPtr mesh,
                             vtkSmartPointer<lq::vtk_model_data>& data)
    :ReflectionAlgBuilder(mesh, data)
  {
    
  }
  ~MixReflectionConvexBuilder(){}
  
  virtual void InitParamters()
  {
    ReflectionAlgBuilder::InitParamters();
  }
  
  virtual void ComputeError(const lq::AlgType type = lq::kCalTwoDiff, 
                            const lq::WindowFlag win = lq::kRight)
  {
    InspectActionBuilder::ExecCompute(lq::kMixReflConvex, type, win);
  }
  
};

class MixIntegralConvexBuilder : public InspectActionBuilder
{
public:
  MixIntegralConvexBuilder(sqi::meshdata::MeshDataPtr mesh,
                           vtkSmartPointer<lq::vtk_model_data>& data)
    : InspectActionBuilder(mesh, data)
  {
    
  }
  ~MixIntegralConvexBuilder(){}
  
  virtual void ComputeError(const lq::AlgType type = lq::kCalTwoDiff, 
                            const lq::WindowFlag win = lq::kRight)
  {
    InspectActionBuilder::ExecCompute(lq::kMixInteConvex, type, win);
  }
};

class MeanCurAlgBuilder : public InspectActionBuilder
{
public:
  
  MeanCurAlgBuilder(sqi::meshdata::MeshDataPtr mesh,
                       vtkSmartPointer<lq::vtk_model_data>& data) 
    : InspectActionBuilder(mesh, data)
  {
    
  }

  virtual void ComputeError(const lq::AlgType type = lq::kCalTwoDiff,
                            const lq::WindowFlag win = lq::kRight)
  {
    InspectActionBuilder::ExecCompute(lq::kMeanCurvature, type, win);
  }

  ~MeanCurAlgBuilder(){}
  
protected:
  
  MeanCurAlgBuilder(){}
  
};

class GaussCurAlgBuilder : public InspectActionBuilder
{
public:
  
  GaussCurAlgBuilder(sqi::meshdata::MeshDataPtr mesh,
                       vtkSmartPointer<lq::vtk_model_data>& data) 
    : InspectActionBuilder(mesh, data)
  {
    
  }

  virtual void ComputeError(const lq::AlgType type = lq::kCalTwoDiff,
                            const lq::WindowFlag win = lq::kRight)
  {
    InspectActionBuilder::ExecCompute(lq::kGaussianCurvature, type, win);
  }

  ~GaussCurAlgBuilder(){}
  
protected:
  
  GaussCurAlgBuilder(){}
  
};

class PrincipalK1CurAlgBuilder : public InspectActionBuilder
{
public:
  
  PrincipalK1CurAlgBuilder(sqi::meshdata::MeshDataPtr mesh,
                       vtkSmartPointer<lq::vtk_model_data>& data) 
    : InspectActionBuilder(mesh, data)
  {
    
  }

  virtual void ComputeError(const lq::AlgType type = lq::kCalTwoDiff,
                            const lq::WindowFlag win = lq::kRight)
  {
    InspectActionBuilder::ExecCompute(lq::kPrincipalK1, type, win);
  }

  ~PrincipalK1CurAlgBuilder(){}
  
protected:
  
  PrincipalK1CurAlgBuilder(){}
  
};

class PrincipalK2CurAlgBuilder : public InspectActionBuilder
{
public:
  
  PrincipalK2CurAlgBuilder(sqi::meshdata::MeshDataPtr mesh,
                       vtkSmartPointer<lq::vtk_model_data>& data) 
    : InspectActionBuilder(mesh, data)
  {
    
  }

  virtual void ComputeError(const lq::AlgType type = lq::kCalTwoDiff,
                            const lq::WindowFlag win = lq::kRight)
  {
    InspectActionBuilder::ExecCompute(lq::kPrincipalK2, type, win);
  }

  ~PrincipalK2CurAlgBuilder(){}
  
protected:
  
  PrincipalK2CurAlgBuilder(){}
  
};

class NormalDiffAlgBuilder : public InspectActionBuilder
{
public:
  
  NormalDiffAlgBuilder(sqi::meshdata::MeshDataPtr mesh,
                       vtkSmartPointer<lq::vtk_model_data>& data) 
    : InspectActionBuilder(mesh, data)
  {
    
  }

  virtual void ComputeError(const lq::AlgType type = lq::kCalTwoDiff,
                            const lq::WindowFlag win = lq::kRight)
  {
    InspectActionBuilder::ExecCompute(lq::kNormalVectorAngle, type, win);
  }

  ~NormalDiffAlgBuilder(){}
  
protected:
  
  NormalDiffAlgBuilder(){}
  
};

class DistanceDiffAlgBuilder : public InspectActionBuilder
{
public:
  
  DistanceDiffAlgBuilder(sqi::meshdata::MeshDataPtr mesh,
                       vtkSmartPointer<lq::vtk_model_data>& data) 
    : InspectActionBuilder(mesh, data)
  {
    
  }

  virtual void ComputeError(const lq::AlgType type = lq::kCalTwoDiff,
                            const lq::WindowFlag win = lq::kRight)
  {
    InspectActionBuilder::ExecCompute(lq::kDistanceDifferent, type, win);
  }

  ~DistanceDiffAlgBuilder(){}
  
protected:
  
  DistanceDiffAlgBuilder(){}
  
};

class MixMeanInteAlgBuilder : public InspectActionBuilder
{
public :
  MixMeanInteAlgBuilder(sqi::meshdata::MeshDataPtr mesh,
                        vtkSmartPointer<lq::vtk_model_data>& data) : 
    InspectActionBuilder(mesh, data)
  {
    
  }
  virtual void ComputeError(const lq::AlgType type = lq::kCalTwoDiff, 
                            const lq::WindowFlag win = lq::kRight)
  {
    InspectActionBuilder::ExecCompute(lq::kMixMeanInte, type, win);
  }
};


class MixGaussInteAlgBuilder : public InspectActionBuilder
{
public :
  MixGaussInteAlgBuilder(sqi::meshdata::MeshDataPtr mesh,
                         vtkSmartPointer<lq::vtk_model_data>& data)
    : InspectActionBuilder(mesh, data)
  {
    
  }
  virtual void ComputeError(const lq::AlgType type = lq::kCalTwoDiff, 
                            const lq::WindowFlag win = lq::kRight)
  {
    InspectActionBuilder::ExecCompute(lq::kMixGaussInte, type, win);
  }
};

class MixK1InteAlgBuilder : public InspectActionBuilder
{
public: 
  MixK1InteAlgBuilder(sqi::meshdata::MeshDataPtr mesh,
                      vtkSmartPointer<lq::vtk_model_data>& data)
    : InspectActionBuilder(mesh, data)
  {
    
  }
  virtual void ComputeError(const lq::AlgType type = lq::kCalTwoDiff, 
                            const lq::WindowFlag win = lq::kRight)
  {
    InspectActionBuilder::ExecCompute(lq::kMixK1Inte, type, win);
  }
};

class MixK2InteAlgBuilder : public InspectActionBuilder
{
public:
  MixK2InteAlgBuilder(sqi::meshdata::MeshDataPtr mesh,
                      vtkSmartPointer<lq::vtk_model_data>& data)
    : InspectActionBuilder(mesh, data)
  {
    
  }
  virtual void ComputeError(const lq::AlgType type = lq::kCalTwoDiff, 
                            const lq::WindowFlag win = lq::kRight)
  {
    InspectActionBuilder::ExecCompute(lq::kMixK2Inte, type, win);
  }
};

class DefectFittingBuilder : public InspectActionBuilder
{
public :
  DefectFittingBuilder(sqi::meshdata::MeshDataPtr mesh,
                       vtkSmartPointer<lq::vtk_model_data>& data)
    : InspectActionBuilder(mesh, data)
  {
    
  }
  virtual void ComputeError(const lq::AlgType type = lq::kCalTwoDiff, 
                            const lq::WindowFlag win = lq::kRight)
  {
    std::cout << "In InspectActionBuilder" << std::endl;
    InspectActionBuilder::ExecCompute(lq::kDefectFitting, type, win);
  }
};

class ZernikerCoeffPropagationBuilder : public InspectActionBuilder {
  
public :
  ZernikerCoeffPropagationBuilder(sqi::meshdata::MeshDataPtr mesh,
                                  vtkSmartPointer<lq::vtk_model_data>& data) 
    : InspectActionBuilder(mesh, data)
  {
    
  }
  virtual void ComputeError(const lq::AlgType type = lq::kCalSingleProperty, 
                            const lq::WindowFlag win = lq::kLeft)
  {
    InspectActionBuilder::ExecCompute(lq::kZernikePropagation, type, win);
  }
};

class ZernikerClassifyBuilder : public InspectActionBuilder{
 public :
  ZernikerClassifyBuilder(sqi::meshdata::MeshDataPtr mesh,
                          vtkSmartPointer<lq::vtk_model_data>& data)
  : InspectActionBuilder(mesh, data)
  {
    
  }
  
  virtual void ComputeError(const lq::AlgType type = lq::kCalSingleProperty, 
                            const lq::WindowFlag win = lq::kLeft)
  {
    InspectActionBuilder::ExecCompute(lq::kZernikerClassify, type, win);
  }
  
  virtual void InitColor()
  {
    menu_operator_->InitClassifyColor();
  }
  
  virtual void GenerateMapColor(lq::WindowFlag win)
  {
    menu_operator_->GenerateClassifyMapColor(win);
  }
  
  virtual void MapColorOnModel(lq::WindowFlag win)
  {
    menu_operator_->MapColorOnVertex(win);
  }
  
  virtual void UpdateColorLegend(lq::WindowFlag win = lq::kRight)
  {
    menu_operator_->UpdateClassifyColorLengend(win);
  }
};



class CalcuateZernikerCoefficientBuilder : public InspectActionBuilder{
public :
  CalcuateZernikerCoefficientBuilder(sqi::meshdata::MeshDataPtr mesh,
                                     vtkSmartPointer<lq::vtk_model_data>& data)
    : InspectActionBuilder(mesh, data)
  {
      
  }
  virtual void ComputeError(const lq::AlgType type = lq::kCalSingleProperty, 
                            const lq::WindowFlag win = lq::kLeft)
  {
    InspectActionBuilder::ExecCompute(lq::kCalcuateZernikerCoefficient, type, win);
  }
  
  virtual void InitColor()
  {
    //menu_operator_->InitClassifyColor();
  }
  
  virtual void GenerateMapColor(lq::WindowFlag win)
  {
    //menu_operator_->GenerateClassifyMapColor(win);
  }
  
  virtual void MapColorOnModel(lq::WindowFlag win)
  {
    //menu_operator_->MapColorOnVertex(win);
  }
  
 
  
  virtual void UpdateColorLegend(lq::WindowFlag win = lq::kRight)
  {
    //menu_operator_->UpdateClassifyColorLengend(win);
  }
  
};
class TrainAndPredictBuilder : public InspectActionBuilder{
public :
  TrainAndPredictBuilder(const sqi::meshdata::MeshDataPtr mesh,
                  const vtkSmartPointer<lq::vtk_model_data>& data)
    : InspectActionBuilder(mesh, data)
  {
    
  }
  virtual void ComputeError(const lq::AlgType type = lq::kCalSingleProperty, 
                            const lq::WindowFlag win = lq::kLeft)
  {
    InspectActionBuilder::ExecCompute(lq::kTrainAndPredict, type, win);
  }
  virtual void InitColor()
  {
  }
  
  virtual void GenerateMapColor(lq::WindowFlag win)
  {
  }
  
  virtual void MapColorOnModel(lq::WindowFlag win)
  {
  }
  
  virtual void UpdateColorLegend(lq::WindowFlag win = lq::kRight)
  {
  }
  
private :
  TrainAndPredictBuilder(){}
};

}
}


#endif // INSPECTACTIONBUILDER_H
