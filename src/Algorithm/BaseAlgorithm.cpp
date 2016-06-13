
#include "BaseAlgorithm.h"

#include <iostream>
#include <fstream>
namespace sqi
{
namespace alg
{
using namespace lq;
using namespace std;

BaseAlgorithm::BaseAlgorithm()
{
  
}

BaseAlgorithm::~BaseAlgorithm()
{
  
}

void BaseAlgorithm::PrintInfo()
{
	cout << "in base algorithm" << endl;
}

void BaseAlgorithm::cal_tri_error(meshdata::MeshDataPtr& mesh_data,
                                  lq::InspectFlag flag)
{
}

void BaseAlgorithm::CalSingleProperty(meshdata::MeshDataPtr &mesh_data,
                                      lq::InspectFlag flag, lq::WindowFlag win)
{
  
}


double BaseAlgorithm::GenerateTriError(meshdata::MeshDataPtr &mesh_data, 
                                       const std::vector<double>& vert_error,
                                       lq::WindowFlag win)
{
  assert(mesh_data.use_count() > 0);
  size_t error_num = 0;
  double max_error = -10000.0f;
  double min_error = 10000.0f;
  for(meshdata::Mesh::FaceIterator fi = mesh_data->M(win).face.begin();
      fi != mesh_data->M(win).face.end(); ++fi)
  {
    double tmp_error = 0.0f;
    for(size_t i = 0; i < 3; ++i)
    {
      tmp_error += vert_error[mesh_data->GetIndexVertex((*fi).V(i), win)];
    }
    //tmp_error /= 3.0;
//    if(!is_zero(tmp_error, sqi::meshdata::EPS_E6))
//    {
//      (*fi).Q() = tmp_error;
//      error_num++;
//    }
//    else
//    {
//      (*fi).Q() = 0.0f;
//      tmp_error = 0.0f;
//    }
    fi->Q() = tmp_error;
    if(is_larger(tmp_error, max_error))
    { 
      max_error = tmp_error;
    }
    if(is_less(tmp_error, min_error))
    {
      min_error = tmp_error;
    }
  }
  mesh_data->MaxE(win) = max_error;
  mesh_data->MinE(win) = min_error;
  return max_error;
}

void BaseAlgorithm::GenerateFinalScalarField(meshdata::MeshDataPtr &mesh_data, 
                                             lq::InspectFlag flag,
                                             lq::WindowFlag win)
{
  double max_error = -10000.00f;
  double min_error = 10000.00f;
  for(meshdata::Mesh::FaceIterator fi = mesh_data->M(win).face.begin();
      fi != mesh_data->M(win).face.end(); ++fi)
  {
    
//    if(flag >= lq::kMeanCurvature && flag <= lq::kPrincipalK2)
//    {
//      if(mesh_data->IsBackSide(win)())
//      {
//        (*fi).Q() = 0.0 - (*fi).Q();
//      }
//    }
//    else 
//    {
//      //(*fi).Q() *= convex_info[*fi] * 1e1;
//    }
    if(lq::is_larger((*fi).Q(), max_error))
      max_error = (*fi).Q();
    if(lq::is_less((*fi).Q(), min_error))
      min_error = (*fi).Q();
    
  }
  mesh_data->MaxE(win) = max_error;
  mesh_data->MinE(win) = min_error;
}

void BaseAlgorithm::MixAlgWithConvex(meshdata::MeshDataPtr &mesh_data,
                                     lq::WindowFlag win)
{
  sqi::meshdata::FaceHandleD convex_diff = mesh_data->GetConvexDiff(win);
  sqi::meshdata::FaceIterator fi1, fi2;
  double max_error = -10000.0f;
  double min_error = 10000.0f;
  for(fi1 = mesh_data->FBeginIter(kLeft), fi2 = mesh_data->FBeginIter(kRight);
      fi1 = mesh_data->FEndIter(kLeft), fi2 != mesh_data->FEndIter(kRight);
      ++fi1, ++fi2)
  {
    double tmp ;
    if(is_less(convex_diff[*fi2], sqi::meshdata::EPS_E6))
      tmp = -1.0f * fi2->Q();
    else if(is_zero(convex_diff[*fi2], sqi::meshdata::EPS_E6))
      tmp = 0.0f;
    else
      tmp = 1.0f * fi2->Q();
    if(is_zero(tmp, sqi::meshdata::EPS_E6))
      fi2->Q() = 0.0f;
    else
      fi2->Q() = tmp;
    if(is_less(fi2->Q(), min_error))
      min_error = fi2->Q();
    if(is_larger(fi2->Q(), max_error))
      max_error = fi2->Q();
  }
  mesh_data->MaxE(win) = max_error;
  mesh_data->MinE(win) = min_error;
}

void BaseAlgorithm::MixAlgWithConvexVertex(meshdata::MeshDataPtr &mesh_data, WindowFlag win)
{
  
}

void BaseAlgorithm::MixCurvatureWithInte(meshdata::MeshDataPtr &mesh_data, 
                                         InspectFlag flag, WindowFlag win)
{
  if(mesh_data->AlorithmModel() != lq::kCalTwoDiff)
    return ;
  if(!(flag >= lq::kMixMeanInte && flag <= lq::kMixK2Inte))
    return ;
  double max_error = -10000.00f;
  double min_error = 10000.00f;
  for(size_t i = 0; i < mesh_data->FNum(win); ++i)
  {
    meshdata::FaceIterator fi1 = mesh_data->FBeginIter(lq::kLeft);
    meshdata::FaceIterator fi2 = mesh_data->FBeginIter(lq::kRight);
    fi1 += i;
    fi2 += i;
    double diff_cur = 0.0f;
    if(flag == lq::kMixMeanInte)
      diff_cur = (fi2->V(0)->Kh() + fi2->V(1)->Kh() + fi2->V(2)->Kh()) / 3.0f - 
          (fi1->V(0)->Kh() + fi1->V(1)->Kh() + fi1->V(2)->Kh()) / 3.0f;
    else if(flag == lq::kMixGaussInte)
      diff_cur = (fi2->V(0)->Kg() + fi2->V(1)->Kg() + fi2->V(2)->Kg()) / 3.0f - 
          (fi1->V(0)->Kg() + fi1->V(1)->Kg() + fi1->V(2)->Kg()) / 3.0f;
    else if(flag == lq::kMixK1Inte)
          diff_cur = (fi2->V(0)->K1() + fi2->V(1)->K1() + fi2->V(2)->K1()) / 3.0f - 
              (fi1->V(0)->K1() + fi1->V(1)->K1() + fi1->V(2)->K1()) / 3.0f;
    else if(flag == lq::kMixK2Inte)
      diff_cur = (fi2->V(0)->K2() + fi2->V(1)->K2() + fi2->V(2)->K2()) / 3.0f - 
          (fi1->V(0)->K2() + fi1->V(1)->K2() + fi1->V(2)->K2()) / 3.0f;
    fi2->Q() *= diff_cur;
    if(lq::is_larger((*fi2).Q(), max_error))
      max_error = (*fi2).Q();
    if(lq::is_less((*fi2).Q(), min_error))
      min_error = (*fi2).Q();
  }
  mesh_data->MaxE(win) = max_error;
  mesh_data->MinE(win) = min_error;
}

}
}
