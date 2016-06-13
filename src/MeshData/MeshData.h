#pragma once
#ifndef MESH_DATA_H
#define MESH_DATA_H

#include <vcg/complex/complex.h>
#include <vcg/complex/algorithms/create/platonic.h>
#include <vcg/complex/algorithms/update/curvature.h>
#include <vcg/complex/algorithms/update/normal.h>

#include <ConsoleMessage.h>
#include <RegistAttribute.h>

#include <iostream>
#include <vector>
#include <map>
#include <set>

#if defined(_MSC_VER)
#include <memory>
#endif

#if defined(__GNUC__)
#include <tr1/memory>
#endif

namespace sqi
{
namespace meshdata
{

const double EPS_E6 = 1e-10;
const double EPS_E7 = 1e-7;
const double EPS_E8 = 1e-8;

enum WinFlag
{
  kLeft = 0,
  kRight
};

enum ColorScalarType
{
  kIndependent = 0,
  kUniform
};

class MeshEdge;
class MeshFace;
class MeshVertex;
struct MeshUsedTypes : public vcg::UsedTypes<	vcg::Use<MeshVertex>::AsVertexType,
    vcg::Use<MeshEdge>::AsEdgeType,
    vcg::Use<MeshFace>::AsFaceType>{};

class MeshVertex : public vcg::Vertex<MeshUsedTypes, vcg::vertex::Coord3f, 
    vcg::vertex::Normal3f, vcg::vertex::Curvaturef, vcg::vertex::Qualityd,
    vcg::vertex::CurvatureDirf, vcg::vertex::VFAdj, vcg::vertex::Color4b,
    vcg::vertex::VEAdj, vcg::vertex::BitFlags>{};

class MeshFace : public vcg::Face< MeshUsedTypes, 
    vcg::face::FFAdj, vcg::face::VFAdj, vcg::face::FEAdj,
    vcg::face::Qualityd, vcg::face::Color4b, vcg::face::Normal3f, 
    vcg::face::VertexRef, vcg::face::BitFlags> {};

class MeshEdge : public vcg::Edge<MeshUsedTypes, vcg::edge::VertexRef, 
    vcg::edge::EFAdj, vcg::edge::EEAdj, vcg::edge::EVAdj, vcg::edge::VEAdj,
    vcg::edge::BitFlags>{};

class Mesh : public vcg::tri::TriMesh< std::vector<MeshVertex>, 
    std::vector<MeshFace>, std::vector<MeshEdge>  > {};

typedef Mesh::VertexIterator                     VertexIterator;
typedef Mesh::VertexType                         VertexType;
typedef Mesh::VertexPointer                      VertexPointer;
typedef Mesh::ConstVertexPointer                 ConstVertexPointer;
typedef Mesh::ConstVertexIterator                ConstVertexIterator;
typedef Mesh::VertContainer                      VertContainer;

typedef Mesh::FaceIterator                       FaceIterator;
typedef Mesh::FaceType                           FaceType;
typedef Mesh::FacePointer                        FacePointer;
typedef Mesh::ConstFacePointer                   ConstFacePointer; 
typedef Mesh::ConstFaceIterator                  ConstFaceIterator;
typedef Mesh::FaceContainer                      FaceContainer;

typedef Mesh::EdgeIterator                       EdgeIterator;
typedef Mesh::EdgeType                           EdgeType;
typedef Mesh::EdgePointer                        EdgePointer;
typedef Mesh::ConstEdgeIterator                  ConstEdgeIterator;

typedef Mesh::CoordType                          CoordType;

typedef vcg::face::VFIterator<MeshFace>          VFIterator;

typedef Mesh::PerMeshAttributeHandle<bool>       MeshHandleB;
typedef Mesh::PerMeshAttributeHandle<double>     MeshHandleD;
typedef Mesh::PerMeshAttributeHandle<int>        MeshHandleI;
typedef Mesh::PerMeshAttributeHandle<CoordType>  MeshHandleCoord;

typedef Mesh::PerFaceAttributeHandle<bool>       FaceHandleB;
typedef Mesh::PerFaceAttributeHandle<double>     FaceHandleD;
typedef Mesh::PerFaceAttributeHandle<int>        FaceHandleI;
typedef Mesh::PerFaceAttributeHandle<CoordType>  FaceHandleCoord;

typedef Mesh::PerVertexAttributeHandle<bool>      VertexHandleB;
typedef Mesh::PerVertexAttributeHandle<double>    VertexHandleD;
typedef Mesh::PerVertexAttributeHandle<int>       VertexHandleI;
typedef Mesh::PerVertexAttributeHandle<CoordType> VertexHandleCoord;


struct Color
{
  unsigned char R;
  unsigned char G;
  unsigned char B;
  double Scalar;
};


template<typename ItemPtr, typename ItemContainer>
inline int GetIndex(const ItemPtr ptr, const ItemContainer& container)
{
  return ptr - &*(container.begin());
}

template<class OpenMeshType>
class MeshData 
{
  
public:
  
  MeshData() : view_point_(0, 0, 0) ,is_color_init_(false), scalar_type_(kIndependent),
    is_has_prin_curvature_(false), alg_model_(lq::kCalSingleProperty), radius_ratio_(1.0f),
    height_radius_multi_(1.0f), is_has_global_mat_(false), 
    is_first_input_radius_(true), min_threshold_(0.0f)
  {
    for(size_t i = 0; i < 2; ++i)
    {
      win_flag_[i] = false;
      is_has_mean_curvature_[i] = false;
      is_has_gauss_curvature_[i] = false;
      is_has_tri_normal_[i] = false;
      is_has_face_face_[i] = false;
      is_has_vert_face_[i] = false;
      is_has_edge_edge_[i] = false;
      is_has_vert_normal_[i] = false;
      max_error_[i] = 0.0f;
      min_error_[i] = 0.0f;
      max_scalar_[i] = 0.0f;
      min_scalar_[i] = 0.0f;
    }
    smooth_step_ = 0;
    RegistNewAttribute();
  }
  ~MeshData()
  {
    for(size_t i = 0; i < 2; ++i)
    { 
      vcg::tri::Allocator<Mesh>::DeletePerFaceAttribute(mesh_[i], "ValueConvex");
      vcg::tri::Allocator<Mesh>::DeletePerFaceAttribute(mesh_[i], "ConvexDiff");
      vcg::tri::Allocator<Mesh>::DeletePerMeshAttribute(mesh_[i], "MaxValueConvex");
      vcg::tri::Allocator<Mesh>::DeletePerMeshAttribute(mesh_[i], "MinValueConvex");
      vcg::tri::Allocator<Mesh>::DeletePerMeshAttribute(mesh_[i], "IsHaveConvex");
      vcg::tri::Allocator<Mesh>::DeletePerMeshAttribute(mesh_[i], "MaxError");
      vcg::tri::Allocator<Mesh>::DeletePerMeshAttribute(mesh_[i], "IsBackSide");
      vcg::tri::Allocator<Mesh>::DeletePerMeshAttribute(mesh_[i], "PreIsBackSide");
      vcg::tri::Allocator<Mesh>::DeletePerMeshAttribute(mesh_[i], "IsFlip");
      vcg::tri::Allocator<Mesh>::DeletePerVertexAttribute(mesh_[i], "MixedArea");
      vcg::tri::Allocator<Mesh>::DeletePerVertexAttribute(mesh_[i], "MeanCurvatureOpt");
    }
    vcg::tri::Allocator<Mesh>::DeletePerFaceAttribute(mesh_[lq::kRight], "GlobalIntegral");
    vcg::tri::Allocator<Mesh>::DeletePerFaceAttribute(mesh_[lq::kRight], "GlobalNormalCom");
    vcg::tri::Allocator<Mesh>::DeletePerFaceAttribute(mesh_[lq::kRight], "LocalIntegral");
    vcg::tri::Allocator<Mesh>::DeletePerFaceAttribute(mesh_[lq::kRight], "LocalNormalCom");
  }
  
  inline void RegistNewAttribute()
  {
    //RegistAttribute<OpenMeshType>::RegistMeanCurDir(mesh_[0]);
    for(size_t i = 0; i < 2; ++i)
    {
      vcg::tri::Allocator<Mesh>::GetPerMeshAttribute<double>(mesh_[i],
                                                             std::string("MaxError"));
      concave_convex_info_[i] = 
          vcg::tri::Allocator<Mesh>::GetPerFaceAttribute<double>(mesh_[i], 
                                                                 std::string("ValueConvex"));
      
      convex_diff_[i] =
          vcg::tri::Allocator<Mesh>::GetPerFaceAttribute<double>(mesh_[i], 
                                                                 std::string("ConvexDiff"));
      
      vcg::tri::Allocator<Mesh>::GetPerMeshAttribute<double>(mesh_[i], 
                                                             std::string("MaxValueConvex"));
      vcg::tri::Allocator<Mesh>::GetPerMeshAttribute<double>(mesh_[i],
                                                             std::string("MinValueConvex"));
      is_back_side_[i] = vcg::tri::Allocator<Mesh>::GetPerMeshAttribute<bool>(mesh_[i],
                                                                                      std::string("IsBackSide"));
      is_back_side_[i]() = false;
      pre_is_back_side_[i] = vcg::tri::Allocator<Mesh>::GetPerMeshAttribute<bool>(mesh_[i],
                                                                                      std::string("PreIsBackSide"));
      pre_is_back_side_[i]() = false;
      is_has_convex_[i] = vcg::tri::Allocator<Mesh>::GetPerMeshAttribute<bool>(mesh_[i], 
                                                                               std::string("IsHaveConvex"));
      is_has_convex_[i]() = false;
      
      is_flip_[i] = vcg::tri::Allocator<Mesh>::GetPerMeshAttribute<bool>(mesh_[i],
                                                                         std::string("IsFlip"));
      is_flip_[i]() = false;
      
      vcg::tri::Allocator<Mesh>::GetPerVertexAttribute<double>(mesh_[i], 
                                                               std::string("MixedArea"));
      vcg::tri::Allocator<Mesh>::GetPerVertexAttribute<CoordType>(mesh_[i],
                                                                  std::string("MeanCurvatureOpt"));
      
    }
    global_integral_ = vcg::tri::Allocator<Mesh>::GetPerFaceAttribute<double>(mesh_[lq::kRight],
        std::string("GlobalIntegral"));
    global_normal_com_ = vcg::tri::Allocator<Mesh>::GetPerFaceAttribute<double>(mesh_[lq::kRight],
        std::string("GlobalNormalCom"));
    local_integral_ = vcg::tri::Allocator<Mesh>::GetPerFaceAttribute<double>(mesh_[lq::kRight],
        std::string("LocalIntegral"));
    local_normal_com_ = vcg::tri::Allocator<Mesh>::GetPerFaceAttribute<double>(mesh_[lq::kRight],
        std::string("LocalNormalCom"));
  }
  template<typename attribute, typename type>
  void InitAttribute(attribute& handle, type value, size_t num)
  {
    for(size_t i = 0; i < num; ++i)
      handle[i] = value;
  }
  
  inline int GetIndexVertex(const VertexType *p, int flag)
  {
    return p - &*(mesh_[flag].vert.begin());
  }
 
  inline int GetIndexFace(const FaceType *p, int flag)
  {
      return p - &*(mesh_[flag].face.begin());
  }
  
  void EdgeBorderFromFace(OpenMeshType& m)
  {
    RequirePerFaceFlags(m);
    for(FaceIterator fi = m.face.begin(); fi != m.face.end(); ++fi)
    {
      for(size_t i = 0; i < 3; ++i)
      {
        if((*fi).FFp(i) == &(*fi))
          (*fi).FEp(i)->SetB();
      }
    }
  }
  void RebuildMesh(int flag, bool is_edge_mesh = false)
  {
    vcg::tri::Clean<Mesh>::RemoveDuplicateVertex(mesh_[flag], is_edge_mesh);
    vcg::tri::Clean<Mesh>::RemoveDuplicateFace(mesh_[flag]);
    vcg::tri::Clean<Mesh>::RemoveUnreferencedVertex(mesh_[flag]);
    vcg::tri::Allocator<OpenMeshType>::CompactVertexVector(mesh_[flag]);
    vcg::tri::Allocator<Mesh>::CompactFaceVector(mesh_[flag]);
    vcg::tri::Allocator<Mesh>::CompactEdgeVector(mesh_[flag]);
  }
  
  void UpdateMesh(int flag)
  {
    vcg::tri::UpdateTopology<Mesh>::FaceFace(mesh_[flag]);
    vcg::tri::UpdateTopology<Mesh>::VertexFace(mesh_[flag]);
    vcg::tri::UpdateTopology<Mesh>::EdgeEdge(mesh_[flag]);
    vcg::tri::UpdateTopology<Mesh>::VertexEdge(mesh_[flag]);
    vcg::tri::UpdateTopology<Mesh>::AllocateEdge(mesh_[flag]);
    vcg::tri::UpdateFlags<Mesh>::FaceBorderFromFF(mesh_[flag]);
    vcg::tri::UpdateFlags<Mesh>::VertexBorderFromFace(mesh_[flag]);
    vcg::tri::UpdateNormal<Mesh>::PerVertexNormalized(mesh_[flag]);
    vcg::tri::UpdateNormal<Mesh>::PerFaceNormalized(mesh_[flag]);
    EdgeBorderFromFace(mesh_[flag]);
  }
  
  bool RepairVertexTopology(int flag)
  {
    sqi::io::ConsoleMessage<std::string>::MessageText("Begin repair mesh.",
                                                      sqi::io::kInfo);
    // Finds all boundary vertices and their adjacent vertices.
    std::map<size_t, std::vector<size_t> > vert_adj;
    for (size_t i = 0; i < mesh_[flag].edge.size(); ++i)
    {
      const EdgeType &edge(mesh_[flag].edge[i]);
      if (edge.IsB()) 
      {
        size_t vert0 = vcg::tri::Index(mesh_[flag], edge.cV(0));
        size_t vert1 = vcg::tri::Index(mesh_[flag], edge.cV(1));
        if (!vert_adj.count(vert0)) vert_adj[vert0] = std::vector<size_t>();
        if (!vert_adj.count(vert1)) vert_adj[vert1] = std::vector<size_t>();
        vert_adj[vert0].push_back(vert1);
        vert_adj[vert1].push_back(vert0);
      }
    }
    // Extracts vertices as shared adjacency. 
    std::set<size_t> shared_vert_adj;
    std::set<size_t> split_vert_adj;
    for (std::map<size_t, std::vector<size_t> >::const_iterator iter = vert_adj.begin();
         iter != vert_adj.end(); ++iter) 
    {
      if (iter->second.size() != 2) 
        shared_vert_adj.insert(iter->first);
      else
        split_vert_adj.insert(iter->first);
    }
    if (!shared_vert_adj.size()) return (false);
    while(true)
    {
      // Fixes T-vertices. Skip if the number of adjacent vertices is greater
      // or less than 2, which mean the current vertex is a shared adjacency
      // or invalid boundary vertex.
      std::set<size_t> neib_faces;
      std::vector<size_t> face_deletion;
      std::set<size_t> shared_faces;
      std::set<size_t> next_round_split;
      for (std::set<size_t>::const_iterator iter = split_vert_adj.begin();
           iter != split_vert_adj.end(); ++iter) 
      {
        size_t vert = *iter;
        size_t adj0 = vert_adj[vert].front();
        size_t adj1 = vert_adj[vert].back();
        if (!shared_vert_adj.count(adj0) || !shared_vert_adj.count(adj1)) continue;
        
        // Finds common face of two adjacent vertices. 
        neib_faces.clear();
        for (VFIterator vf(&(mesh_[flag].vert[adj0])); !vf.End(); ++vf)
        {
          neib_faces.insert(vcg::tri::Index(mesh_[flag], vf.F()));
        }
        for(VFIterator vf(&(mesh_[flag].vert[adj1])); !vf.End(); ++vf) 
        {
          size_t face = vcg::tri::Index(mesh_[flag], vf.F());
          if (neib_faces.count(face)) 
          {
            size_t vert0 = vcg::tri::Index(mesh_[flag], vf.F()->V1(vf.I()));
            size_t vert1 = vcg::tri::Index(mesh_[flag], vf.F()->V2(vf.I()));
            bool direct = !(vert0 == adj0);
            bool deletion = !shared_faces.count(face);
            size_t common = direct ? vert0 : vert1;
            if (shared_vert_adj.count(common)) 
              shared_faces.insert(face);
            if (deletion) 
            {
              vcg::tri::Allocator<Mesh>::AddFaces(mesh_[flag], 1);
              mesh_[flag].face.back().V(0) = &(mesh_[flag].vert[common]);
              mesh_[flag].face.back().V(!direct + 1) = &(mesh_[flag].vert[adj0]);
              mesh_[flag].face.back().V(direct + 1) = &(mesh_[flag].vert[vert]);
              mesh_[flag].face.back().ClearAllF();
              vcg::tri::Allocator<Mesh>::AddFaces(mesh_[flag], 1);
              mesh_[flag].face.back().V(0) = &(mesh_[flag].vert[common]);
              mesh_[flag].face.back().V(!direct + 1) = &(mesh_[flag].vert[vert]);
              mesh_[flag].face.back().V(direct + 1) = &(mesh_[flag].vert[adj1]);
              mesh_[flag].face.back().ClearAllF();
              face_deletion.push_back(face);
            }
            else 
            {
              next_round_split.insert(vert);
            }
            break;
          }
        }
      }
      // Removes splitted faces and update topologies.
      for(size_t i = 0; i < face_deletion.size(); ++i)
      {
        size_t face = face_deletion[i];
        if (!mesh_[flag].face[face].IsD()) 
          vcg::tri::Allocator<Mesh>::DeleteFace(mesh_[flag], mesh_[flag].face[face]);
      }
      vcg::tri::UpdateTopology<Mesh>::FaceFace(mesh_[flag]);
      vcg::tri::UpdateTopology<Mesh>::VertexFace(mesh_[flag]);
      if (next_round_split.size() == 0) 
        break;
      split_vert_adj = next_round_split;
    }
//    RebuildMesh(flag);
//    UpdateMesh(flag);
    std::cout << "end repair vertex " << std::endl;
    return true;
  }
  
  bool RepairFaceTopology(int flag)
  {
    
    std::vector<bool> flags(mesh_[flag].en, false);
    // Finds non-manifold edges.
    for (size_t i = 0; i < mesh_[flag].fn; ++i)
    {
      const FaceType &face(mesh_[flag].face[i]);
      const VertexPointer vps[] = { face.cV(0), face.cV(1), face.cV(2) };
      for (int j = 0; j < 3; ++j) 
      {
        const EdgePointer ep = face.cFEp(j);
        int ei = vcg::tri::Index(mesh_[flag], face.cFEp(j));
        for (int k = 0; k < 3; ++k)
        {
          if (ep->cV(0) == vps[k]) 
          {
            flags[ei] = flags[ei] ^ (ep->cV(1) == vps[(k + 1) % 3]);
            break;
          }
        }
      }
    }
    // Adjusts flags for boundary edges.
    int count = 0;
    for (size_t i = 0; i < mesh_[flag].en; ++i) 
    {
      if (mesh_[flag].edge[i].IsB()) 
        flags[i] = true;
      if (!flags[i]) 
        ++count;
    }
    if (!count) return (false);
    
    // Computes accumulative normal.
    vcg::Point3f normal(0.0, 0.0, 0.0);
    for (size_t i = 0; i < mesh_[flag].fn; ++i) 
    {
      normal += mesh_[flag].face[i].cN() * ((mesh_[flag].face[i].cP(1) 
                                             - mesh_[flag].face[i].cP(0)) ^ 
                                            (mesh_[flag].face[i].cP(2) - 
                                             mesh_[flag].face[i].cP(0))).Norm();
    }
    normal.Normalize();
    // Traverses mesh and fix normals.
    std::vector<bool> visited(mesh_[flag].fn, false);
    
    while (true) 
    {
      std::queue<FacePointer> queue;
      
      for (size_t i = 0; i < mesh_[flag].fn; ++i) 
      {
        FacePointer fp = &(mesh_[flag].face[i]);
        for (int j = 0; j < 3; ++j)
        {
          EdgePointer ep = fp->FEp(j);
          int ei = vcg::tri::Index(mesh_[flag], ep);
          if (!flags[ei]) 
          {
            double fn = normal * fp->cN();
            double ffn = fp->FFp(j)->cN() * normal;
            if (fn > ffn) 
              queue.push(fp->FFp(j));
            else 
              queue.push(fp);
            break;
          }
        }
        if (!queue.empty()) break;
      }
      if (queue.empty()) break;
      while (!queue.empty()) 
      {
        FacePointer fp = queue.front();
        int fi = vcg::tri::Index(mesh_[flag], fp);
        queue.pop();
        if (visited[fi]) continue;
        std::swap(fp->V(1), fp->V(2));
        visited[fi] = true;
        
        for (int i = 0; i < 3; ++i) 
        {
          int ei = vcg::tri::Index(mesh_[flag], fp->FEp(i));
          if (mesh_[flag].edge[ei].IsB())
            continue;
          if (!(flags[ei] = !flags[ei])) 
            queue.push(fp->FFp(i));
        }
      }
    }
    
    return (true);
  }

  //store and access Vcg variable
  inline OpenMeshType& M(int flag)                         {return mesh_[flag];}
  inline Mesh::CoordType& VP()                             {return view_point_;}

  inline const int VNum(int flag) const                    {return mesh_[flag].vert.size();}
  inline const int FNum(int flag) const                    {return mesh_[flag].face.size();}
  inline const int ColorNum() const                        {return color_list_.size();}
  inline int& ScalarType()                                 {return scalar_type_;}
  
  inline FaceIterator FBeginIter(int flag)                 {return M(flag).face.begin();}
  inline FaceIterator FEndIter(int flag)                   {return M(flag).face.end();}
  
  inline VertexIterator VBeginIter(int flag)               {return M(flag).vert.begin();}
  inline VertexIterator VEndIter(int flag)                 {return M(flag).vert.end();}
  
  
  //store and access other variable
  inline std::vector<Color>& StdColor()                    {return color_list_;}
  inline std::vector<double>& IntegralReflList()           {return integral_refl_list_;}
  inline std::vector<std::string>& SelectFileList()        {return select_file_list_;}
  inline std::vector<std::set<int> >& SelectIdxList()      {return select_idx_list_;}
  
  inline bool& IsWinHas(int flag)                          {return win_flag_[flag];}
  inline bool& IsInitColor()                               {return is_color_init_;}
  inline bool& IsHasMeanCur(int flag)                      {return is_has_mean_curvature_[flag];}
  inline bool& IsHasGaussCur(int flag)                     {return is_has_gauss_curvature_[flag];}
  inline bool& IsHasPrinCur()                              {return is_has_prin_curvature_;}
  inline bool& IsHasFF(int flag)                           {return is_has_face_face_[flag];}
  inline bool& IsHasVF(int flag)                           {return is_has_vert_face_[flag];}
  inline bool& IsHasEE(int flag)                           {return is_has_edge_edge_[flag];}
  inline bool& IsHasFNormal(int flag)                      {return is_has_tri_normal_[flag];}
  inline bool& IsHasVNormal(int flag)                      {return is_has_vert_normal_[flag];}
  inline bool& IsHasIntegralReflList()                     {return is_has_integral_refl_list_;}
  

  inline bool IsHasPerF(int flag)                          {return vcg::tri::HasPerFaceNormal(mesh_[flag]);}
  
  inline MeshHandleB IsBackSide(int flag)                  {return is_back_side_[flag];}
  inline MeshHandleB PreIsBackSide(int flag)               {return pre_is_back_side_[flag];}
  inline MeshHandleB IsHasConvex(int flag)                 {return is_has_convex_[flag];}
  inline MeshHandleB IsFlipDir(int flag)                   {return is_flip_[flag];}

  inline FaceHandleD GetConcaveConvexInfo(int flag)        {return concave_convex_info_[flag];}  
  inline FaceHandleD GetConvexDiff(lq::WindowFlag flag)    {return convex_diff_[flag];}
  
  inline FaceHandleD GetGlobalIntegral()                   {return global_integral_;}
  inline FaceHandleD GetGlobalNormalCom()                  {return global_normal_com_;}
  inline FaceHandleD GetLocalIntegral()                    {return local_integral_;}
  inline FaceHandleD GetLocalNormalCom()                   {return local_normal_com_;}
  
  inline double &MaxE(lq::WindowFlag win = lq::kRight)     {return max_error_[win];}
  inline double &MinE(lq::WindowFlag win = lq::kRight)     {return min_error_[win];}
  inline double &MaxColor(lq::WindowFlag win = lq::kRight) {return max_scalar_[win];}
  inline double &MinColor(lq::WindowFlag win = lq::kRight) {return min_scalar_[win];}
  inline double &MinThreshold()                            {return min_threshold_;}
  inline double &LocalRadius()                             {return radius_;}
  inline double &PreLocalRadius()                          {return pre_radius_;}
  inline double &RadiusRatio()                             {return radius_ratio_;}
  inline double &HeightRadiusMulti()                       {return height_radius_multi_;}
  inline int    &SmoothStep()                              {return smooth_step_;}
  inline std::string &PredictFile()                        {return predict_file_;}
  inline std::string &PythonFilePath()                     {return python_file_path_;}
  
  inline lq::AlgType &AlorithmModel()                      {return alg_model_;}
  
  inline vcg::Matrix44f &GlobalAlignMat()                  {return global_align_mat_;}
  
  inline bool &IsHasGlobalMat()                            {return is_has_global_mat_;}  
  inline bool &IsFirstInputRadius()                        {return is_first_input_radius_;}
  
  inline void Reset(lq::WindowFlag flag)
  {
    win_flag_[flag] = false;
    ResetFlip(flag);
    IsBackSide(flag)() = false;
    PreIsBackSide(flag)() = false;
    is_has_global_mat_ = false;
    is_first_input_radius_ = true;
    mesh_[flag].Clear();
    radius_ = pre_radius_ = 0.0f;
    radius_ratio_ = 1.0f;
    height_radius_multi_ = 1.0f;
    predict_file_ = "";
    min_threshold_ = 0.0f;
    max_scalar_[lq::kLeft] = 0.0f;
    max_scalar_[lq::kRight] = 0.0f;
    smooth_step_ = 0;
    scalar_type_ = kIndependent;
    select_file_list_.clear();
  }
  
  inline void ResetFlip(lq::WindowFlag flag)
  {
    IsHasConvex(flag)() = false;
    is_has_tri_normal_[flag] = false;
    is_has_mean_curvature_[flag] = false;
    is_has_gauss_curvature_[flag] = false;
    is_has_prin_curvature_ = false;
  }

protected:
  
  //Vcg variable
  OpenMeshType mesh_[2];
  Mesh::CoordType view_point_;
  
  //other variable
  std::vector<Color> color_list_;
  std::vector<double> integral_refl_list_;
  std::vector<std::string> select_file_list_;
  std::vector<std::set<int> > select_idx_list_;
  
  double max_error_[2];
  double min_error_[2];
  double max_scalar_[2];
  double min_scalar_[2];
  double min_threshold_;
  double radius_;
  double pre_radius_;
  double radius_ratio_;
  double height_radius_multi_;
  int smooth_step_;
  std::string predict_file_;
  std::string python_file_path_;
  
  int scalar_type_;
  bool win_flag_[2];
  bool is_color_init_;
  bool is_has_mean_curvature_[2];
  bool is_has_gauss_curvature_[2];
  bool is_has_face_face_[2];
  bool is_has_vert_face_[2];
  bool is_has_edge_edge_[2];
  bool is_has_prin_curvature_;
  bool is_has_tri_normal_[2];
  bool is_has_vert_normal_[2];
  bool is_has_integral_refl_list_;
  lq::AlgType alg_model_;
  
  MeshHandleB is_has_convex_[2];
  MeshHandleB pre_is_back_side_[2];
  MeshHandleB is_back_side_[2];
  MeshHandleB is_flip_[2];
  FaceHandleD concave_convex_info_[2];
  FaceHandleD convex_diff_[2];
  FaceHandleD global_integral_;
  FaceHandleD global_normal_com_;
  FaceHandleD local_integral_;
  FaceHandleD local_normal_com_;
  
  vcg::Matrix44f global_align_mat_;
  bool is_has_global_mat_;
  bool is_first_input_radius_;
  
}; 
typedef MeshData<Mesh> MeshDataMgr;
typedef std::tr1::shared_ptr<MeshData<Mesh> > MeshDataPtr;

}

}


#endif
