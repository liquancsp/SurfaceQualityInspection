#ifndef DESCRIPTORACTIONDIRECTOR_H
#define DESCRIPTORACTIONDIRECTOR_H
#include <Builder/DescriptorActionBuilder.h>
namespace sqi {
namespace common{

class DescriptorActionDirector {
public:
  DescriptorActionDirector(const sqi::meshdata::MeshDataPtr mesh,
                          const vtkSmartPointer<lq::vtk_model_data> data) :
    mesh_data_(mesh), data_set_(data),
    descriptor_bulid_ptr_(new DescriptorActionBuilder(mesh, data))
  {
    
  }
 
  ~DescriptorActionDirector(){}
  
  void SetMeshData(sqi::meshdata::MeshDataPtr& other)
  {
    this->mesh_data_ = other;
  }
  
  void SetDataSet(const vtkSmartPointer<lq::vtk_model_data>& other)
  {
    this->data_set_ = other;
  }
  
  void Execute(lq::InspectFlag flag)
  {
    std::cout << "[#Info]In descriptor director." << std::endl;
    if(mesh_data_->IsWinHas(lq::kLeft))
      Execute(flag, lq::kLeft);
    if(mesh_data_->IsWinHas(lq::kRight))
      Execute(flag, lq::kRight);
  }
  void Execute(lq::InspectFlag flag, lq::WindowFlag win)
  {
    if(flag == lq::kCalMeanZernikerCoeff)
      descriptor_bulid_ptr_.reset(new MeanZernikerDescriptorBuilder(mesh_data_, data_set_));
    else if(flag == lq::kCalGaussZernikerCoeff)
      descriptor_bulid_ptr_.reset(new GaussZernikerDescriptorBuilder(mesh_data_, data_set_));
    else if(flag == lq::kCalK1ZernikerCoeff)
      descriptor_bulid_ptr_.reset(new K1ZernikerDescriptorBuilder(mesh_data_, data_set_));
    else if(flag == lq::kCalK2ZernikerCoeff)
      descriptor_bulid_ptr_.reset(new K2ZernikerDescriptorBuilder(mesh_data_, data_set_));
    else if(flag == lq::kCalCurvatureZernikerCoeff)
      descriptor_bulid_ptr_.reset(new CurvatureZernikerDescriptorBuilder(mesh_data_, data_set_));
    else if(flag == lq::kCalCurvatureDescriptor)
      descriptor_bulid_ptr_.reset(new CurvatureDescriptorBuilder(mesh_data_, data_set_));
    else if(flag == lq::kCalCurvatureHistogram)
      descriptor_bulid_ptr_.reset(new CurvatureHistogramBuilder(mesh_data_, data_set_));
    else if(flag == lq::kCalSmoothCurvatureDiff)
      descriptor_bulid_ptr_.reset(new SmoothCurvatureDiffBuilder(mesh_data_, data_set_));
    descriptor_bulid_ptr_->ComputeDescriptor(flag, win);
  }

private:
  sqi::meshdata::MeshDataPtr mesh_data_;
  vtkSmartPointer<lq::vtk_model_data> data_set_;
  std::tr1::shared_ptr<DescriptorActionBuilder> descriptor_bulid_ptr_;

};
}

}
#endif // DESCRIPTORACTIONDIRECTOR_H
