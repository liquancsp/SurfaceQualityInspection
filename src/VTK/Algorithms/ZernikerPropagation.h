#ifndef ZERNIKERPROPAGATION_H
#define ZERNIKERPROPAGATION_H

#include <MeshData.h>
#include <BaseAlgorithm.h>
#include <Algorithms/CalHeightMap.h>
#include <Algorithms/ZernikeCoefficient.h>
#include <Algorithms/Propagation.h>
#include <ExportTrainningData.h>
#include <vtk_data/vtk_model_data.h>

namespace sqi{
namespace alg{

class ZernikerPropagation : public BaseAlgorithm{
public : 
  ZernikerPropagation();
  ~ZernikerPropagation();
  
  virtual void CalSingleProperty(meshdata::MeshDataPtr& mesh_data, 
                                 lq::InspectFlag flag, lq::WindowFlag win);
  
  void CreateTrainningData(meshdata::MeshDataPtr &mesh_data,  const std::string& training_file,
                           lq::WindowFlag win, sqi::io::ClassifyType type);
  
  void CreateTrainningData(meshdata::MeshDataPtr &mesh_data, const std::string& training_file,
                           lq::WindowFlag win, sqi::io::ClassifyType type, int exemplar_idx,
                           double radius);
  
  void CreateTrainningData(meshdata::MeshDataPtr &mesh_data, const std::string& training_file,
                           lq::WindowFlag win, sqi::io::ClassifyType type, 
                           std::vector<int>& idx_list);
  
  void CreateTestData(meshdata::MeshDataPtr& mesh_data, const std::string& test_file,
                      lq::WindowFlag win);
  
  void CreateTestData(meshdata::MeshDataPtr &mesh_data, const std::string &test_file,
                      lq::WindowFlag win, int exemplar_idx, double radius);
  
  void CreateTestData(meshdata::MeshDataPtr &mesh_data, const std::string &test_file,
                      lq::WindowFlag win, std::vector<int>& idx_list);
  
  static void SetDataSet(vtkSmartPointer<lq::vtk_model_data>& data)
  {
    data_set_ = data;
  }
  
  void SetExemplarIndex(int idx)        {exemplar_idx_ = idx;}
  void SetNeighborRadius(double r)      {neighbor_radius_ = r;}
  void SetThresholdTau(double tau)      {threshold_tau_ = tau;}
  
protected:
  static vtkSmartPointer<lq::vtk_model_data> data_set_;
  
private:

  
  std::tr1::shared_ptr<CalHeightMap> height_map_;
  std::tr1::shared_ptr<ZernikeCoefficient> zerniker_coeff_;
  std::tr1::shared_ptr<Propagation> propagate_;
  int exemplar_idx_;
  double neighbor_radius_;
  double threshold_tau_;
  void GenerateTriError(meshdata::MeshDataPtr &mesh_data, 
                        lq::WindowFlag win);
  void GenerateZernikerCoeff(meshdata::MeshDataPtr &mesh_data,
                             std::vector<std::vector<double> >&zerniker_coeff_vec,
                             lq::WindowFlag win);
};

}
}

#endif // ZERNIKERPROPAGATION_H
