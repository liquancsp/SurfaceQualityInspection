#ifndef GEODESIC_H
#define GEODESIC_H
#include <vcg/simplex/face/pos.h>
#include <vcg/simplex/face/topology.h>
#include <vcg/complex/algorithms/update/quality.h>
#include <vcg/complex/algorithms/geodesic.h>

#include <MeshData.h>
#include <Topology/UpdateRingNeighbor.h>

#include <string.h>
#include <queue>

namespace sqi{
namespace alg{
const double INF = 100000.0f;

template<typename MeshType >
class Geodesic {
  
public:
  typedef typename MeshType::VertexType      VertexType;
  typedef typename MeshType::VertexPointer   VertexPointer;
  typedef typename MeshType::VertexIterator  VertexIterator;
  typedef typename MeshType::VertContainer   VertContainer;
  
  typedef typename MeshType::FacePointer     FacePointer;
  typedef typename MeshType::FaceIterator    FaceIterator;
  typedef typename MeshType::FaceContainer   FaceContainer;
  
  typedef typename MeshType::ScalarType      ScalarType;
  
  typedef std::pair<double, int> pii;
  
  void FaceDijsktraCompute(MeshType& m, const FacePointer face_ptr,
                           ScalarType radius, std::vector<int>& neighbor_list,
                           FacePointer target_face = NULL)
  {
    std::vector<double> d;
    d.resize(m.face.size());
    for(size_t i = 0; i < d.size(); ++i)
      d[i] = INF;
    int idx = sqi::meshdata::GetIndex<FacePointer, FaceContainer>(face_ptr, m.face);
    d[idx] = 0.0f;
    std::priority_queue<pii, std::vector<pii>, std::greater<pii> > Q;
    Q.push(std::make_pair(d[idx], idx));
    neighbor_list.push_back(idx);
    int face_size = m.face.size();
    std::vector<bool> mark;
    //mark.resize(m.face.size());
    mark.assign(m.face.size(), false);
    //memset(mark, false, sizeof(mark));
    mark[idx] = true;
    while(!Q.empty())
    {
      pii tmp = Q.top();
      idx = tmp.second;
      Q.pop();
      FacePointer curr = &m.face[idx];
      if(target_face != NULL && curr == target_face)
        return;
      for(size_t i = 0; i < 3; ++i)
      {
        if(!vcg::face::IsBorder(*curr, i))
        {
          FacePointer next_face = curr->FFp(i);
          ScalarType dist = d[idx] + vcg::tri::EuclideanDistance<MeshType>()(curr, next_face);
          int next_idx = sqi::meshdata::GetIndex<FacePointer, FaceContainer>(next_face,
                                                                             m.face);
          if(dist < radius && dist < d[next_idx])
          {
            Q.push(std::make_pair(d[next_idx], next_idx));
            d[next_idx] = dist;
            if(!mark[next_idx])
            {
              neighbor_list.push_back(next_idx);
              mark[next_idx] = true;
            }
          }
        }
      }
    }
  }
  
  void VertexDijisktraCompute(MeshType& m, VertexPointer vert_ptr,
                              ScalarType radius, std::vector<int>& neighbor_list,
                              VertexPointer target_vert = NULL)
  {
    std::vector<double> d;
    d.resize(m.vert.size());
    for(size_t i = 0; i < d.size(); ++i)
      d[i] = INF;
    int idx = sqi::meshdata::GetIndex<VertexPointer, VertContainer>(vert_ptr, m.vert);
    d[idx] = 0.0f;
    std::priority_queue<pii, std::vector<pii>, std::greater<pii> > Q;
    Q.push(std::make_pair(d[idx], idx));
    neighbor_list.push_back(idx);
    std::vector<bool> mark;
    mark.assign(m.face.size(), false);
    mark[idx] = true;
    while(!Q.empty())
    {
      pii tmp = Q.top();
      idx = tmp.second;
      Q.pop();
      VertexPointer curr = &m.vert[idx];
      if(target_vert != NULL && curr == target_vert)
        return;
      std::set<int> tmp_neigh_set;
      UpdateRigngNeighbor<MeshType>::VertexOneRingVertexNeighborPos(m, curr, 
                                                                    tmp_neigh_set);
      std::set<int>::iterator iter;
      for(iter = tmp_neigh_set.begin(); iter != tmp_neigh_set.end(); ++iter)
      {
        VertexPointer next_vert = &m.vert[(*iter)];
        ScalarType dist = d[idx] + vcg::tri::EuclideanDistance<MeshType>()(curr, next_vert);
        int next_idx = meshdata::GetIndex<VertexPointer, VertContainer>(next_vert,
                                                                        m.vert);
        if(dist < radius && dist < d[next_idx])
        {
          Q.push(std::make_pair(d[next_idx], next_idx));
          d[next_idx] = dist;
          if(!mark[next_idx])
          {
            neighbor_list.push_back(next_idx);
            mark[next_idx] = true;
          }
        }
      }
    }
  }
  
};

}
}

#endif // GEODESIC_H
