#ifndef DEFECTFITTING_H
#define DEFECTFITTING_H

#include <BaseAlgorithm.h>

namespace sqi{
namespace alg{

class DefectFitting : public BaseAlgorithm{
public :
  DefectFitting();
  ~DefectFitting();
  void CalSingleProperty(meshdata::MeshDataPtr& mesh_data,
                         lq::InspectFlag flag, lq::WindowFlag win);
private :
  void SearchCapArea(meshdata::Mesh::FaceIterator fi, meshdata::MeshDataPtr& mesh_data,
                     lq::WindowFlag win, std::vector<bool>& visited);

  inline double FacePrincipalDiff(meshdata::Mesh::FacePointer fp);
  inline bool IsPrincipalZero(meshdata::Mesh::FacePointer fp);
  inline double LenEdgeOfFace(meshdata::Mesh::FacePointer fp, int edge_index);
};

}
}

#endif // DEFECTFITTING_H
