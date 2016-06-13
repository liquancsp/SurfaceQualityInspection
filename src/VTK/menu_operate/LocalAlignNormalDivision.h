#ifndef LOCALALIGNNORMALDIVISION_H
#define LOCALALIGNNORMALDIVISION_H

#include <BaseAlgorithm.h>
#include <vtk_data/vtk_model_data.h>
#include <menu_operate/LocalAlign.h>

namespace sqi{
namespace alg{

class LocalAlignNormalDivision : public BaseAlgorithm, public LocalAlign
{
public:
  LocalAlignNormalDivision();
  ~LocalAlignNormalDivision();
  
  virtual void cal_tri_error(meshdata::MeshDataPtr &mesh_data, lq::InspectFlag flag);

  void SetDataSet(vtkSmartPointer<lq::vtk_model_data>& data)
  {
      this->data_set_ = data;
  }

private:
  
  vtkSmartPointer<lq::vtk_model_data> data_set_;
};

}
}


#endif // LOCALALIGNNORMALDIVISION_H
