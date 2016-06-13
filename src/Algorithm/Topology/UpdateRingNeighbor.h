#ifndef UPDATE_RINGNEIGHBOR_H
#define UPDATE_RINGNEIGHBOR_H
#include <vcg/complex/algorithms/update/topology.h>
#include <vcg/simplex/face/pos.h> 
#include <vcg/simplex/face/jumping_pos.h>
#include <MeshData.h>

namespace sqi{
namespace alg{


template<typename MeshType>
class UpdateRigngNeighbor {
public:
  
  typedef typename MeshType::FaceType FaceType;
  typedef typename MeshType::FacePointer FacePointer;
  typedef typename MeshType::FaceIterator FaceIterator;
  typedef typename MeshType::FaceContainer FaceContainer;
  typedef typename MeshType::VertexIterator VertexIterator;
  typedef typename MeshType::VertContainer VertContainer;
  typedef typename MeshType::VertexType VertexType;
  typedef typename MeshType::VertexPointer VertexPointer;
  typedef typename MeshType::EdgePointer EdgePointer;
  typedef typename MeshType::EdgeType EdgeType;
  typedef typename MeshType::EdgeContainer EdgeContainer;
  typedef typename MeshType::CoordType CoordType;
  
 
  static void VertexOneRingVertexNeighborPos(MeshType& m, VertexType* const vert_ptr,
                                             std::set<int>& neighbor_list)
  {
    if(!vcg::tri::HasFFAdjacency(m))
      vcg::tri::UpdateTopology<MeshType>::FaceFace(m);
    if(!vcg::tri::HasVFAdjacency(m))
      vcg::tri::UpdateTopology<MeshType>::VertexFace(m);
    RequireFFAdjacency(m);
    RequireVFAdjacency(m);
    FaceType* face_ptr = vert_ptr->VFp();
    vcg::face::Pos<FaceType> pos(face_ptr, vert_ptr);
    EdgeType* edge_ptr = pos.F()->FEp(pos.E());
    FlipEAndInsert(m, pos, neighbor_list);
    bool flag = false;
    if(pos.F()->FEp(pos.E())->IsB())
      flag = true;
    flag = (ScanAlongClock(m, edge_ptr, pos, neighbor_list)) ? true : flag; 
    if(!flag)
      return ;
    vcg::face::Pos<FaceType> pos1(face_ptr, vert_ptr);
    if(pos1.F()->FEp(pos1.E())->IsB())
      return;
    pos1.FlipE();
    FlipEAndInsert(m, pos1, neighbor_list);
    ScanAlongClock(m, edge_ptr, pos1, neighbor_list);
  }
  
  static void VertexOneRingVertexNeighborJP(MeshType& m, VertexType* const vert_ptr,
                                            std::set<int>& neighbor_list)
  {
//    if(!vcg::tri::HasFFAdjacency(m))
//      vcg::tri::UpdateTopology<MeshType>::FaceFace(m);
//    if(!vcg::tri::HasVFAdjacency(m))
//      vcg::tri::UpdateTopology<MeshType>::VertexFace(m);
//    RequireFFAdjacency(m);
//    RequireVFAdjacency(m);
//    FaceType* face_ptr = vert_ptr->VFp();
//    vcg::face::JumpingPos<FaceType> jp(face_ptr, vert_ptr);
//    do
//    {
//      FlipEAndInsert(m, jp, neighbor_list);
////      int tmp = sqi::meshdata::GetIndex<VertexType*, VertContainer>(jp.V(),
////                                                                    m.vert);
////      neighbor_list.insert(tmp);
//      jp.NextFE();
//    }while(face_ptr != jp.F());
//    FlipEAndInsert(m, jp, neighbor_list);
  }
  
  void VertexOneRingFaceNeighborVF(MeshType& m, VertexType* const vert_ptr,
                                   std::set<int>& face_list)
  {
    vcg::face::VFIterator<FaceType> vfi(vert_ptr); //initialize the iterator tohe first face
    for(;!vfi.End();++vfi)
    {
      int tmp = sqi::meshdata::GetIndex<FaceType*, FaceContainer>(vfi.F(), 
                                                                  m.face);
      face_list.insert(tmp);
    }
  }
  
  static void FaceOneRingFaceNeighborFF(MeshType& m, FaceType* const face_ptr,
                                        std::set<int>& neighbor_list)
  {
    if(!vcg::tri::HasFFAdjacency(m))
      vcg::tri::UpdateTopology<MeshType>::FaceFace(m);
    for(size_t i = 0; i < 3; ++i)
    {
      FaceType* tmp = face_ptr->FFp(i);
      if(tmp != face_ptr)
      {
        int id = sqi::meshdata::GetIndex<FaceType*, FaceContainer>(tmp, 
                                                                   m.face);
        neighbor_list.insert(id);
      }
    }
      
  }
  
private:
  
  static inline void FlipEAndInsert(const MeshType& m, vcg::face::Pos<FaceType>& pos,
                                    std::set<int>& neighbor_list)
  {
    pos.FlipV();
    int tmp = sqi::meshdata::GetIndex<VertexType*, VertContainer>(pos.V(), 
                                                                  m.vert);
    neighbor_list.insert(tmp);
    pos.FlipV();
  }
  
  static inline bool ScanAlongClock(const MeshType& m,  const EdgeType* edge_ptr,
                                    vcg::face::Pos<FaceType>& pos,
                                    std::set<int>& neighbor_list)
  {
    do
    {
      pos.FlipF();
      pos.FlipE();
      FlipEAndInsert(m, pos, neighbor_list);
      if(pos.F()->FEp(pos.E())->IsB())
      {
        return true;
      }
    }while(pos.F()->FEp(pos.E()) != edge_ptr);
    return false;
  }
  
  
};
}
}
#endif // UPDATE_RINGNEIGHBOR_H
