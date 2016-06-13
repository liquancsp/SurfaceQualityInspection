#ifndef SURFACEDESCRIPTOR_H
#define SURFACEDESCRIPTOR_H
#include <vector>
#include <string>
#include <MeshData.h>
#include <vtk_data/vtk_model_data.h>

#include <vtkTimerLog.h>

namespace sqi {
namespace alg {

class SurfaceDescriptor{
public:
  SurfaceDescriptor();
  virtual ~SurfaceDescriptor(); 
  
  virtual void Descriptor(meshdata::MeshDataPtr &mesh_data,
                          vtkSmartPointer<lq::vtk_model_data>& data_set,
                          lq::WindowFlag win, lq::InspectFlag flag){}
  
protected:
  typedef double(*func_ptr)(double);
  void WriteFile(const std::vector<std::vector<double> >& height_map,
                 const std::string& file_name) const;
  void PushRadialPolyFunc(std::vector<func_ptr>& radial_poly);
  void CalGridFlag(std::vector<bool>& flag, double e, double radius);
  
  void InitializeParameters(meshdata::MeshDataPtr& mesh_data, 
                            std::vector<func_ptr>& radial_poly,
                            const double bound[6], double& radius);
  void InitializeParameters(meshdata::MeshDataPtr &mesh_data,
                            const double bound[], double &radius);
  
  void InitializeParameters(meshdata::MeshDataPtr& mesh_data,
                            vtkSmartPointer<lq::vtk_model_data>& data_set,
                            lq::WindowFlag win, double bound[6]);
  int MapCurvatureToBin(double cur, int div);  
  template<typename T>
  void DivVector(std::vector<T>& vec, T num)
  {
    if(num == 0)
      return;
    for(int i = 0; i < vec.size(); ++i)
      vec[i] /= num;
  }
  void CalZernikerCoeff(meshdata::MeshDataPtr& mesh_data,
                        const std::vector<double>& surface_feature,
                        const vtkSmartPointer<vtkIdList>& idx_list,
                        std::vector<func_ptr>& radial_poly,
                        int i, lq::WindowFlag win,
                        std::vector<std::vector<double> >& zerniker_coeff,
                        bool is_diff = false);
  
  void CalZernikerCoeff(meshdata::Mesh& mesh,
                        const std::vector<double>& surface_feature,
                        const vtkSmartPointer<vtkIdList>& idx_list,
                        std::vector<func_ptr>& radial_poly,
                        int i, std::vector<std::vector<double> >& zerniker_coeff,
                        bool is_diff = false);
  
  void CalGaussianWeightZernikeCoeff(meshdata::MeshDataPtr& mesh_data,
                                     vtkSmartPointer<vtkKdTreePointLocator>& kd_tree,
                                     const std::vector<std::vector<double> >& zerniker_coeff,
                                     std::vector<std::vector<double> >&zer_coeff_gauss, 
                                     double radius, lq::WindowFlag win);
  
  void CalGaussianWeightZernikeCoeff(sqi::meshdata::Mesh& mesh, 
                                     vtkSmartPointer<vtkKdTreePointLocator>& kd_tree,
                                     const std::vector<std::vector<double> >& zerniker_coeff,
                                     std::vector<std::vector<double> >&zer_coeff_gauss,
                                     double radius, bool is_diff);
  
  void GenerateExportFileName(const std::string& model_file, const std::string& flag_str,
                              std::string& export_file);
  
  vtkSmartPointer<vtkTimerLog> timer_;
               
};

}

}

#endif // SURFACEDESCRIPTOR_H
