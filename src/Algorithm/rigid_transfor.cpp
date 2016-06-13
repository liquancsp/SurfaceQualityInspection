/*
#include "rigid_transfor.h"
#include "rigid_transfor_function.h"
#include <iostream>

//#include "hjlib/optimizer/optimizer.h"
#include "zjucad/optimizer/optimizer.h"

namespace lq 
{

using namespace std;
using namespace std::tr1;
using namespace zjucad::matrix;
rigid_transfor::rigid_transfor():ring_number_(1)
{
  rigid_mat_ = zjucad::matrix::zeros(3, 3);
}

rigid_transfor::~rigid_transfor()
{

}

//------------------------------------------------------------------------------
void rigid_transfor::cal_tri_error(const std::vector<point> &vertex_list1,
                                   const std::vector<point> &vertex_list2,
                                   const std::vector<triangle> &tri_list1,
                                   const std::vector<triangle> &tri_list2,
                                   const point &view_point,
                                   std::vector<double> &tri_error)
{
  cout << "[#info]Begin rigid transfor algorithm" << endl;
  cal_ring_neighbor(vertex_list1, tri_list1, ring_number_, ring_type_, ring_neighbor_list_);
  if(ring_type_ == Vertex)
  {
  
  }
  else if(ring_type_ == Triangle)
  {
    double diff;
    point center1, center2;
    matrixd normal1, normal2;
    vertex_normal tri_normal1, tri_normal2;
    double max_error = 0.0;
    for(size_t i = 0; i < tri_list1.size(); ++i)
    {
      cout << "[#info]Before optimize." << endl;
      optimize_rigid_triansfor(vertex_list1, vertex_list2,
                               tri_list1, tri_list2, 
                               ring_neighbor_list_[i]);
      diff = 0.0;
      cout << rigid_mat_ << endl;
      get_center_point(vertex_list1, tri_list1[i], center1);
      get_center_point(vertex_list2, tri_list2[i], center2);
      point_to_mat(tri_list1[i].triangle_normal, normal1);
      point_to_mat(tri_list2[i].triangle_normal, normal2);
      normal1 = rigid_mat_ * normal1;
      normal2 = rigid_mat_ * normal2;
      mat_to_point(normal1, tri_normal1);
      mat_to_point(normal2, tri_normal2);
      get_diff(center1, tri_list1[i].triangle_normal,
               center2, tri_list2[i].triangle_normal,
               view_point, diff);
      if(is_larger(diff, max_error))
        max_error = diff;
      if(!is_zero(diff))
        tri_error.push_back(diff);
      else
        tri_error.push_back(0.0);
    }
    for(size_t i = 0; i < tri_error.size(); ++i)
      tri_error[i] /= max_error;
  }
}
//------------------------------------------------------------------------------

//begin protected member function 
//------------------------------------------------------------------------------
void rigid_transfor::optimize_rigid_triansfor(const std::vector<point> &vertex_list1,
                                              const std::vector<point> &vertex_list2,
                                              const std::vector<triangle> &tri_list1,
                                              const std::vector<triangle> &tri_list2,
                                              ring_neighbor &ring_neighbor_item)
{
  cout << "[#info]Begin optimize rigid triansfor." << endl;
  set_optimize_function(vertex_list1, vertex_list2, tri_list1,
                        tri_list2, ring_neighbor_item);
  func_ptr func_ptr_list(hj::function::new_catenated_function<double, int32_t>(rigid_func_list_));
  matrixd residual(func_ptr_list->dim_of_f(), 1);
  rigid_mat_ = zeros(3, 3);
  pt_.put("iter.desc","number of max iteration");
  pt_.put("iter.value", 500);
  pt_.put("package.desc","numerical package:<alglib,libLBFGS,hj,HLBFGS>");
  pt_.put("package.value", "alglib");
  pt_.put("alg.desc","algorithm <non-linear-cg,lbfgs>");
  pt_.put("alg.value", "lbfgs");
  pt_.put("lbfgs-len.value", "300");
  zjucad::optimize(*func_ptr_list, rigid_mat_, residual, pt_);
  cout << "[#info]Completed optimize rigid triansfor." << endl;
  
}
//------------------------------------------------------------------------------

void rigid_transfor::set_optimize_function(const std::vector<point> &vertex_list1,
                                           const std::vector<point> &vertex_list2,
                                           const std::vector<triangle> &tri_list1,
                                           const std::vector<triangle> &tri_list2,
                                           ring_neighbor &ring_neighbor_item)
{ 
  ring_neighbor_list_.clear();
  rigid_func_list_.clear();
  set<size_t>::iterator iter;
  if(ring_type_ == Triangle)
  {
    
    size_t idx_vertex, idx_tri;
    for(iter = ring_neighbor_item.adj_set.begin();
        iter != ring_neighbor_item.adj_set.end(); ++iter)
    {
      idx_tri = (*iter);
      for(size_t j = 0 ; j < tri_list1[idx_tri].vertex.size(); ++j)
      {
        idx_vertex = tri_list1[idx_tri].vertex[j] - 1;
        neighbor_list_.insert(idx_vertex);
      }
    }
   // for(size_t )
  }
  else if(ring_type_ == Vertex)
  {
    neighbor_list_.insert(ring_neighbor_item.adj_set.begin(), 
                          ring_neighbor_item.adj_set.end());
  }
  for(iter = neighbor_list_.begin();
      iter != neighbor_list_.end(); ++iter)
  {
    matrixd mat_p1;
    matrixd mat_p2;
    point p1, p2;
    p1 = vertex_list1[(*iter)];
    p2 = vertex_list2[(*iter)];
    point_to_mat(p1, mat_p1);
    point_to_mat(p2, mat_p2);
    
    auto_ptr<rigid_transfor_function> rtf(
          new rigid_transfor_function(mat_p1, mat_p2));
    rigid_func_list_.push_back(func_ptr(rtf.release()));
  }
  
}

//------------------------------------------------------------------------------


};
*/
