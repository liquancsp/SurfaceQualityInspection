#ifndef DESCRIPTORACTIONBUILDER_H
#define DESCRIPTORACTIONBUILDER_H

#include <MeshData.h>
#include <vtk_data/vtk_model_data.h>
#include <Facade/MenuOperator.h>

namespace sqi{
namespace common{

class DescriptorActionBuilder {
  
public:
  
  DescriptorActionBuilder(const sqi::meshdata::MeshDataPtr mesh,
                          const vtkSmartPointer<lq::vtk_model_data> data) 
    : mesh_data_(mesh), data_set_(data), 
      menu_operator_(new MenuOperator(mesh, data))
  {
    
  }
  
  virtual ~DescriptorActionBuilder(){}
  virtual void ComputeDescriptor(const lq::InspectFlag flag,
                                 const lq::WindowFlag win)
  {
    
  }
  
protected:
  
  DescriptorActionBuilder(){}
  sqi::meshdata::MeshDataPtr mesh_data_;
  vtkSmartPointer<lq::vtk_model_data> data_set_;
  std::tr1::shared_ptr<MenuOperator> menu_operator_;
};

class MeanZernikerDescriptorBuilder : public DescriptorActionBuilder{
  
public:
  MeanZernikerDescriptorBuilder(const sqi::meshdata::MeshDataPtr mesh,
                                const vtkSmartPointer<lq::vtk_model_data>& data)
    : DescriptorActionBuilder(mesh, data)
  {
    
  }
  
  void ComputeDescriptor(const lq::InspectFlag flag,
                         const lq::WindowFlag win)
  {
    menu_operator_->ComputeDescriptor(flag, win);
  }
  
protected:
  MeanZernikerDescriptorBuilder(){}
  
};

class GaussZernikerDescriptorBuilder : public DescriptorActionBuilder{
public :
  GaussZernikerDescriptorBuilder(const sqi::meshdata::MeshDataPtr mesh,
                                 const vtkSmartPointer<lq::vtk_model_data>& data)
    : DescriptorActionBuilder(mesh, data)
  {
    
  }
  
  void ComputeDescriptor(const lq::InspectFlag flag,
                         const lq::WindowFlag win)
  {
    menu_operator_->ComputeDescriptor(flag, win);
  }
  
protected:
  GaussZernikerDescriptorBuilder(){}
};

class K1ZernikerDescriptorBuilder : public DescriptorActionBuilder{
public:
  K1ZernikerDescriptorBuilder(const sqi::meshdata::MeshDataPtr mesh,
                              const vtkSmartPointer<lq::vtk_model_data>& data)
    : DescriptorActionBuilder(mesh, data)
  {
    
  }
  void ComputeDescriptor(const lq::InspectFlag flag,
                         const lq::WindowFlag win)
  {
    menu_operator_->ComputeDescriptor(flag, win);
  }
private:
  K1ZernikerDescriptorBuilder(){}
};

class K2ZernikerDescriptorBuilder : public DescriptorActionBuilder{
public:
  K2ZernikerDescriptorBuilder(const sqi::meshdata::MeshDataPtr mesh,
                              const vtkSmartPointer<lq::vtk_model_data>& data)
    : DescriptorActionBuilder(mesh, data)
  {
    
  }
  void ComputeDescriptor(const lq::InspectFlag flag,
                         const lq::WindowFlag win)
  {
    menu_operator_->ComputeDescriptor(flag, win);
  }
private:
  K2ZernikerDescriptorBuilder(){}
};

class CurvatureZernikerDescriptorBuilder : public DescriptorActionBuilder{
public :
  
  CurvatureZernikerDescriptorBuilder(const sqi::meshdata::MeshDataPtr mesh,
                                     const vtkSmartPointer<lq::vtk_model_data>& data)
    : DescriptorActionBuilder(mesh, data)
  {
    
  }
  void ComputeDescriptor(const lq::InspectFlag flag,
                         const lq::WindowFlag win)
  {
    menu_operator_->ComputeDescriptor(flag, win);
  }
private:
  CurvatureZernikerDescriptorBuilder(){}
  
};

class CurvatureDescriptorBuilder : public DescriptorActionBuilder{
public :
  CurvatureDescriptorBuilder(const sqi::meshdata::MeshDataPtr mesh,
                             const vtkSmartPointer<lq::vtk_model_data>& data)
    : DescriptorActionBuilder(mesh, data)
  {
    
  }
  void ComputeDescriptor(const lq::InspectFlag flag,
                         const lq::WindowFlag win)
  {
    menu_operator_->ComputeDescriptor(flag, win);
  }
private:
  CurvatureDescriptorBuilder(){}
};

class CurvatureHistogramBuilder : public DescriptorActionBuilder{
public :
  CurvatureHistogramBuilder(const sqi::meshdata::MeshDataPtr mesh,
                            const vtkSmartPointer<lq::vtk_model_data>& data)
    : DescriptorActionBuilder(mesh, data)
  {
    
  }
  void ComputeDescriptor(const lq::InspectFlag flag,
                         const lq::WindowFlag win)
  {
    menu_operator_->ComputeDescriptor(flag, win);
  }
private:
  CurvatureHistogramBuilder(){}
  
};

class SmoothCurvatureDiffBuilder : public DescriptorActionBuilder{
public:
  
  SmoothCurvatureDiffBuilder(const sqi::meshdata::MeshDataPtr mesh,
                             const vtkSmartPointer<lq::vtk_model_data>& data)
    : DescriptorActionBuilder(mesh, data)
  {
    
  }
  void ComputeDescriptor(const lq::InspectFlag flag,
                         const lq::WindowFlag win)
  {
    menu_operator_->ComputeDescriptor(flag, win);
  }
  
private :
  SmoothCurvatureDiffBuilder(){}
  
};

}
}

#endif // DESCRIPTORACTIONBUILDER_H
