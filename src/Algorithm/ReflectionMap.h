#ifndef REFLECTIONMAP_H
#define REFLECTIONMAP_H

#include <BaseAlgorithm.h>
#include <Factory/Singleton.h>
namespace sqi
{
namespace alg
{

class ReflectionMap : public BaseAlgorithm
{
public:
  
  
  ~ReflectionMap();
  ReflectionMap();
  void cal_tri_error(meshdata::MeshDataPtr &mesh_data, lq::InspectFlag flag);
  
protected:
  
  inline void get_tri_center(const meshdata::MeshDataPtr& mesh_data, 
                             const sqi::meshdata::FaceIterator& fi,
                             const int flag, sqi::meshdata::Mesh::CoordType& center)
  {
    int idx1, idx2, idx3;
    center.SetZero();
    for(size_t i = 0; i < 3; ++i)
    {
      center += mesh_data->M(flag).vert[mesh_data->GetIndexVertex((*fi).V(i), flag)].P();
    }
    center /= 3.0;
  }
  
  inline void cal_normal_diff(const sqi::meshdata::Mesh::CoordType& center1,
                              const sqi::meshdata::Mesh::CoordType& center2,
                              sqi::meshdata::Mesh::CoordType& normal1,
                              sqi::meshdata::Mesh::CoordType& normal2,
                              const sqi::meshdata::Mesh::CoordType& view_point,
                              double& diff)
  {
    meshdata::Mesh::CoordType refl_vec1(0.0, 0.0, 0.0);
    meshdata::Mesh::CoordType refl_vec2(0.0, 0.0, 0.0);
    meshdata::Mesh::CoordType incident1(0.0, 0.0, 0.0);
    meshdata::Mesh::CoordType incident2(0.0, 0.0, 0.0);
    incident1 = center1 - view_point;
    incident1.normalized();
    incident2 = center2 - view_point;
    incident2.normalized();
    refl_vec1 = incident1 - normal1 * (incident1 * normal1) * 2;
    refl_vec2 = incident2 - normal2 * (incident2 * normal2) * 2;
    diff = (refl_vec1 - refl_vec2).Norm();
  }
  
  bool IsValidViewPoint(const sqi::meshdata::Mesh::CoordType& center1,
                        const sqi::meshdata::Mesh::CoordType& center2,
                        const sqi::meshdata::Mesh::CoordType& normal1,
                        const sqi::meshdata::Mesh::CoordType& normal2,
                        const sqi::meshdata::Mesh::CoordType& view_point);
  
 // friend class sqi::common::Singleton<ReflectionMap>;
};

}
}

#endif // REFLECTIONMAP_H
