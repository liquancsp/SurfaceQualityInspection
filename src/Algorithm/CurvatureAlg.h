#ifndef CURVATUREALG_H
#define CURVATUREALG_H

#include <vcg/simplex/face/topology.h>
#include <vcg/simplex/face/pos.h>
#include <vcg/complex/algorithms/point_sampling.h>

#include <vcg/complex/algorithms/update/normal.h>
#include <vcg/complex/algorithms/point_sampling.h>
#include <vcg/complex/algorithms/intersection.h>
#include <MeshData.h>
#include <fstream>
#include <cstdlib>
namespace sqi {

namespace alg {

template<class MeshType>
class CurvatureAlg {
  
public:
  typedef typename MeshType::FaceType FaceType;
  typedef typename MeshType::FacePointer FacePointer;
  typedef typename MeshType::FaceIterator FaceIterator;
  typedef typename MeshType::VertexIterator VertexIterator;
  typedef typename MeshType::VertContainer VertContainer;
  typedef typename MeshType::VertexType VertexType;
  typedef typename MeshType::VertexPointer VertexPointer;
  typedef vcg::face::VFIterator<FaceType> VFIteratorType;
  typedef typename MeshType::CoordType CoordType;
  typedef typename CoordType::ScalarType ScalarType;
  
  class AreaData
  {
  public:
    float A;
  };
  
  static void GenerateConcaveConvex(MeshType& m)
  {
    sqi::meshdata::MeshHandleB is_have_convex = 
        vcg::tri::Allocator<sqi::meshdata::Mesh>::GetPerMeshAttribute<bool>(m, std::string("IsHaveConvex"));
    if(!is_have_convex())
      MeanAndGaussian(m);
    sqi::meshdata::FaceHandleD value_convex_list = 
        vcg::tri::Allocator<sqi::meshdata::Mesh>::GetPerFaceAttribute<double>(m, std::string("ValueConvex"));
    sqi::meshdata::VertexHandleD mixed_area = 
        vcg::tri::Allocator<sqi::meshdata::Mesh>::GetPerVertexAttribute<double>(m, std::string("MixedArea"));
    sqi::meshdata::VertexHandleCoord mean_cur_opt = 
        vcg::tri::Allocator<sqi::meshdata::Mesh>::GetPerVertexAttribute<CoordType>(m, std::string("MeanCurvatureOpt"));
    FaceIterator fi;
    for(fi = m.face.begin(); fi != m.face.end(); ++fi)
    {
      //      value_convex_list[*fi] = mean_cur_opt[fi->V(0)] * fi->V(0)->cN() * mixed_area[fi->V(0)] + 
      //          mean_cur_opt[fi->V(1)] * fi->V(1)->cN() * mixed_area[fi->V(1)] +
      //          mean_cur_opt[fi->V(2)] * fi->V(2)->cN() * mixed_area[fi->V(2)];
      value_convex_list[*fi] = (mean_cur_opt[fi->V(0)] * fi->V(0)->cN() + 
          mean_cur_opt[fi->V(1)] * fi->V(1)->cN() +
          mean_cur_opt[fi->V(2)] * fi->V(2)->cN()) / 3.0f;
    }
  }
  
  static void CalConvexDiff(MeshType& m1, MeshType& m2)
  {
    sqi::meshdata::MeshHandleD min_convex = 
        vcg::tri::Allocator<sqi::meshdata::Mesh>::GetPerMeshAttribute<double>(m2, std::string("MinValueConvex"));
    sqi::meshdata::MeshHandleD max_convex = 
        vcg::tri::Allocator<sqi::meshdata::Mesh>::GetPerMeshAttribute<double>(m2, std::string("MaxValueConvex"));
    sqi::meshdata::FaceHandleD convex_diff = 
        vcg::tri::Allocator<sqi::meshdata::Mesh>::GetPerFaceAttribute<double>(m2, std::string("ConvexDiff"));
    sqi::meshdata::FaceHandleD value_convex_list1 = 
        vcg::tri::Allocator<sqi::meshdata::Mesh>::GetPerFaceAttribute<double>(m1, std::string("ValueConvex"));
    sqi::meshdata::FaceHandleD value_convex_list2 = 
        vcg::tri::Allocator<sqi::meshdata::Mesh>::GetPerFaceAttribute<double>(m2, std::string("ValueConvex"));
    
    max_convex() = -10000.00f;
    min_convex() = 10000.00f;
    GenerateConcaveConvex(m1);
    GenerateConcaveConvex(m2);
    for(FaceIterator fi2 = m2.face.begin(), fi1 = m1.face.begin() 
        ;fi1 != m1.face.end(), fi2 != m2.face.end(); ++fi1, ++fi2)
    {
      convex_diff[*fi2] = (value_convex_list2[*fi2] - value_convex_list1[*fi1]);
      if(lq::is_larger(convex_diff[*fi2], max_convex()))
        max_convex() = convex_diff[*fi2];
      if(lq::is_less(convex_diff[*fi2], min_convex()))
        min_convex() = convex_diff[*fi2];
      
    }
  }
  
  static int UpdateConcavAndConvex(MeshType& m1, MeshType& m2)
  {
    sqi::meshdata::MeshHandleB is_back_side = 
        vcg::tri::Allocator<sqi::meshdata::Mesh>::GetPerMeshAttribute<bool>(m2, 
                                                                            std::string("IsBackSide"));
    sqi::meshdata::MeshHandleB pre_is_back_side = 
        vcg::tri::Allocator<sqi::meshdata::Mesh>::GetPerMeshAttribute<bool>(m2, 
                                                                            std::string("PreIsBackSide"));
    sqi::meshdata::MeshHandleB is_have_convex1 = 
        vcg::tri::Allocator<sqi::meshdata::Mesh>::GetPerMeshAttribute<bool>(m1, std::string("IsHaveConvex"));
    sqi::meshdata::MeshHandleB is_have_convex2 = 
        vcg::tri::Allocator<sqi::meshdata::Mesh>::GetPerMeshAttribute<bool>(m2, std::string("IsHaveConvex"));
    
    sqi::meshdata::FaceHandleD convex_diff = 
        vcg::tri::Allocator<sqi::meshdata::Mesh>::GetPerFaceAttribute<double>(m2, std::string("ConvexDiff"));
    sqi::meshdata::MeshHandleD min_convex = 
        vcg::tri::Allocator<sqi::meshdata::Mesh>::GetPerMeshAttribute<double>(m2, std::string("MinValueConvex"));
    sqi::meshdata::MeshHandleD max_convex = 
        vcg::tri::Allocator<sqi::meshdata::Mesh>::GetPerMeshAttribute<double>(m2, std::string("MaxValueConvex"));
    sqi::meshdata::MeshHandleB is_flip = 
        vcg::tri::Allocator<sqi::meshdata::Mesh>::GetPerMeshAttribute<bool>(m2, std::string("IsFlip"));
    double flag;
    flag = (is_back_side() == pre_is_back_side()) ? 1.0 : - 1.0;
    if(!is_have_convex1() || !is_have_convex2())
    {
      MeanAndGaussian(m1);
      MeanAndGaussian(m2);
    }
    else if(is_have_convex1() && is_have_convex2() && is_back_side() != pre_is_back_side())
    {
      MeanAndGaussian(m1);
      MeanAndGaussian(m2);
      for(FaceIterator fi = m2.face.begin(); fi != m2.face.end(); ++fi)
      {
        convex_diff[*fi] = 0.0 -  convex_diff[*fi];
      }
      double tmp;
      tmp = max_convex();
      max_convex() = 0.0 - min_convex();
      min_convex() = 0 - tmp;
      is_flip() = true;
      pre_is_back_side() = is_back_side();
      return 0;
    }
    else
    {
      CalConvexDiff(m1, m2);
      return 0;
    }
    CalConvexDiff(m1, m2);
    if(pre_is_back_side() != is_back_side())
    {
      is_flip() = true; 
      pre_is_back_side() = is_back_side();
    }
    return 0;
  }
  
  static int MeanAndGaussian(MeshType& m)
  {
    vcg::tri::RequireFFAdjacency(m);
    float area0, area1, area2, angle0, angle1, angle2;
    FaceIterator fi;
    VertexIterator vi;
    typename MeshType::CoordType  e01v ,e12v ,e20v;
    
    vcg::SimpleTempData<VertContainer, AreaData> TDAreaPtr(m.vert);
    vcg::SimpleTempData<VertContainer, typename MeshType::CoordType> TDContr(m.vert);
    vcg::tri::UpdateNormal<MeshType>::PerVertexNormalized(m);
    //Compute AreaMix in H (vale anche per K)
    for(vi=m.vert.begin(); vi!=m.vert.end(); ++vi) if(!(*vi).IsD())
    {
      (TDAreaPtr)[*vi].A = 0.0;
      (TDContr)[*vi]  =typename MeshType::CoordType(0.0,0.0,0.0);
      (*vi).Kh() = 0.0;
      (*vi).Kg() = (float)(2.0 * M_PI);
    }
    for(fi=m.face.begin();fi!=m.face.end();++fi) if( !(*fi).IsD())
    {
      // angles
      angle0 = vcg::math::Abs(Angle(	(*fi).P(1)-(*fi).P(0),(*fi).P(2)-(*fi).P(0) ));
      angle1 = vcg::math::Abs(Angle(	(*fi).P(0)-(*fi).P(1),(*fi).P(2)-(*fi).P(1) ));
      angle2 = M_PI-(angle0+angle1);
      
      if((angle0 < M_PI/2) && (angle1 < M_PI/2) && (angle2 < M_PI/2))  // triangolo non ottuso
      {
        
        float e01 = vcg::SquaredDistance( (*fi).V(1)->cP() , (*fi).V(0)->cP() );
        float e12 = vcg::SquaredDistance( (*fi).V(2)->cP() , (*fi).V(1)->cP() );
        float e20 = vcg::SquaredDistance( (*fi).V(0)->cP() , (*fi).V(2)->cP() );
        
        area0 = ( e20*(1.0/tan(angle1)) + e01*(1.0/tan(angle2)) ) / 8.0;
        area1 = ( e01*(1.0/tan(angle2)) + e12*(1.0/tan(angle0)) ) / 8.0;
        area2 = ( e12*(1.0/tan(angle0)) + e20*(1.0/tan(angle1)) ) / 8.0;
        
        (TDAreaPtr)[(*fi).V(0)].A  += area0;
        (TDAreaPtr)[(*fi).V(1)].A  += area1;
        (TDAreaPtr)[(*fi).V(2)].A  += area2;
      }
      else // obtuse
      {
        if(angle0 >= M_PI/2)
        {
          (TDAreaPtr)[(*fi).V(0)].A += vcg::DoubleArea<typename MeshType::FaceType>((*fi)) / 4.0;
          (TDAreaPtr)[(*fi).V(1)].A += vcg::DoubleArea<typename MeshType::FaceType>((*fi)) / 8.0;
          (TDAreaPtr)[(*fi).V(2)].A += vcg::DoubleArea<typename MeshType::FaceType>((*fi)) / 8.0;
        }
        else if(angle1 >= M_PI/2)
        {
          (TDAreaPtr)[(*fi).V(0)].A += vcg::DoubleArea<typename MeshType::FaceType>((*fi)) / 8.0;
          (TDAreaPtr)[(*fi).V(1)].A += vcg::DoubleArea<typename MeshType::FaceType>((*fi)) / 4.0;
          (TDAreaPtr)[(*fi).V(2)].A += vcg::DoubleArea<typename MeshType::FaceType>((*fi)) / 8.0;
        }
        else
        {
          (TDAreaPtr)[(*fi).V(0)].A += vcg::DoubleArea<typename MeshType::FaceType>((*fi)) / 8.0;
          (TDAreaPtr)[(*fi).V(1)].A += vcg::DoubleArea<typename MeshType::FaceType>((*fi)) / 8.0;
          (TDAreaPtr)[(*fi).V(2)].A += vcg::DoubleArea<typename MeshType::FaceType>((*fi)) / 4.0;
        }
      }
    }
    size_t cnt = 0;
    for(fi=m.face.begin();fi!=m.face.end();++fi) if( !(*fi).IsD() )
    {
      angle0 = vcg::math::Abs(Angle(	(*fi).P(1)-(*fi).P(0),(*fi).P(2)-(*fi).P(0) ));
      angle1 = vcg::math::Abs(Angle(	(*fi).P(0)-(*fi).P(1),(*fi).P(2)-(*fi).P(1) ));
      angle2 = M_PI - ( angle0 + angle1);
      
      // Skip degenerate triangles.
      if(angle0==0 || angle1==0 || angle1==0) continue;
      
      e01v = ( (*fi).V(1)->cP() - (*fi).V(0)->cP() ) ;
      e12v = ( (*fi).V(2)->cP() - (*fi).V(1)->cP() ) ;
      e20v = ( (*fi).V(0)->cP() - (*fi).V(2)->cP() ) ;
      
      TDContr[(*fi).V(0)] += ( e20v * (1.0/tan(angle1)) - e01v * (1.0/tan(angle2)) ) / 2.0;
      TDContr[(*fi).V(1)] += ( e01v * (1.0/tan(angle2)) - e12v * (1.0/tan(angle0)) ) / 2.0;
      TDContr[(*fi).V(2)] += ( e12v * (1.0/tan(angle0)) - e20v * (1.0/tan(angle1)) ) / 2.0;
      
      (*fi).V(0)->Kg() -= angle0;
      (*fi).V(1)->Kg() -= angle1;
      (*fi).V(2)->Kg() -= angle2;
      
      for(int i=0;i<3;i++)
      {
        if(vcg::face::IsBorder((*fi), i))
        {
          CoordType e1,e2;
          vcg::face::Pos<FaceType> hp(&*fi, i, (*fi).V(i));
          vcg::face::Pos<FaceType> hp1=hp;
          
          hp1.FlipV();
          e1=hp1.v->cP() - hp.v->cP();
          hp1.FlipV();
          hp1.NextB();
          e2=hp1.v->cP() - hp.v->cP();
          (*fi).V(i)->Kg() -= vcg::math::Abs(Angle(e1,e2));
        }
      }
    }
    sqi::meshdata::VertexHandleD mixed_area = 
        vcg::tri::Allocator<sqi::meshdata::Mesh>::GetPerVertexAttribute<double>(m, std::string("MixedArea"));
    sqi::meshdata::VertexHandleCoord mean_cur_opt = 
        vcg::tri::Allocator<sqi::meshdata::Mesh>::GetPerVertexAttribute<CoordType>(m, std::string("MeanCurvatureOpt"));
    
    for(vi=m.vert.begin(); vi!=m.vert.end(); ++vi) 
    {
      if(!(*vi).IsD() && !(*vi).IsB())
      {
        if((TDAreaPtr)[*vi].A <= std::numeric_limits<ScalarType>::epsilon())
        {
          (*vi).Kh() = 0.0f;
          (*vi).Kg() = 0.0f;
          mixed_area[*vi] = 0.0f;
          mean_cur_opt[*vi] = CoordType(0, 0, 0);
        }
        else
        {
          mixed_area[*vi] = (TDAreaPtr)[*vi].A;
          mean_cur_opt[*vi] = (TDContr)[*vi] / (TDAreaPtr)[*vi].A;
          (*vi).Kh()  = (((TDContr)[*vi].dot((*vi).cN())>0)?1.0:-1.0)*(mean_cur_opt[*vi] / 2.0f).Norm();
          (*vi).Kg() /= (TDAreaPtr)[*vi].A;
        }
      }
      if((*vi).IsB())
      {
        (*vi).Kg() = 0.0f;
        (*vi).Kh() = 0.0f;
        mixed_area[*vi] = (TDAreaPtr)[*vi].A;
        mean_cur_opt[*vi] = CoordType(0, 0, 0);
      }
      //      if(lq::is_zero(vi->Kg(), sqi::meshdata::EPS_E6))
      //        vi->Kg() = 0.0f;
      //      if(lq::is_zero(vi->Kh(), sqi::meshdata::EPS_E6))
      //        vi->Kh() = 0.0f;
    }
    sqi::meshdata::Mesh::PerMeshAttributeHandle<bool> is_have_convex = 
        vcg::tri::Allocator<sqi::meshdata::Mesh>::GetPerMeshAttribute<bool>(m, std::string("IsHaveConvex"));
    is_have_convex() = true;
    return 0;
  }
  
  static void PrincipalCurvatureAndDirection(MeshType& m)
  {
    //vcg::tri::UpdateCurvature<MeshType>::PrincipalDirections(m);
    //    sqi::meshdata::MeshHandleB is_have_convex = 
    //        vcg::tri::Allocator<sqi::meshdata::Mesh>::GetPerMeshAttribute<bool>(m, std::string("IsHaveConvex"));
    //    sqi::meshdata::MeshHandleB is_back_side = 
    //        vcg::tri::Allocator<sqi::meshdata::Mesh>::GetPerMeshAttribute<bool>(m, 
    //                                                                      std::string("IsBackSide"));
    //    sqi::meshdata::MeshHandleB pre_is_back_side = 
    //        vcg::tri::Allocator<sqi::meshdata::Mesh>::GetPerMeshAttribute<bool>(m, 
    //                                                                      std::string("PreIsBackSide"));
    //    if(!is_have_convex() || is_back_side() != pre_is_back_side())
    //    {
    //      is_have_convex() = true;
    //      MeanAndGaussian(m);
    //      pre_is_back_side() = is_back_side();
    //    }
    meshdata::Mesh::VertexIterator vi;
    for(vi = m.vert.begin(); vi != m.vert.end(); ++vi)
    {
      double delta = 0.0f;
      delta = (*vi).Kh() * (*vi).Kh() - (*vi).Kg();
      if(lq::is_less_equal(delta, 0.0f))
        delta = 0.0f;
      (*vi).K1() = (*vi).Kh() + sqrt(delta);
      (*vi).K2() = (*vi).Kh() - sqrt(delta);
    }
  }
  
  static void PrincipalCurWithDirection(MeshType& m)
  {
    vcg::tri::UpdateCurvature<meshdata::Mesh>::PrincipalDirections(m);
    for(meshdata::VertexIterator vi = m.vert.begin(); vi != m.vert.end(); ++vi)
    {
      vi->PD1().Normalize();
      vi->PD2().Normalize();
    }
  }
};

}

}


#endif // CURVATUREALG_H
