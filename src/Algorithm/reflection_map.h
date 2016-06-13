/*******************************************************************************
 *COPYRIGHT NOTICE  
 *Copyright (c) 2012, Zhe Jiang University 
 *All rights reserved.
 *
 *@file reflection_map.h
 *@author li quan
 *@version 1.0
 *@date 2012-11
 *@ingroup diff
 *@brief the function of this class is to compute the different of two model and
 *   test the result.
 *
 *@history
 ********************************************************************************/


#ifndef REFLECTION_MAP_H
#define REFLECTION_MAP_H
#include "data_type.h"
#include "algorithm_base.h"
#include "../3rd/zjucad/matrix/matrix.h"
#include <fstream>
namespace lq
{
  
/**
 *@class reflection_map
 *@ingroup diff
 *@brief the function of this class is to compute the different of two model and
 *   test the result.
 *
 */
  class reflection_map : public algorithm_base
  {

  public:
    reflection_map();
    ~reflection_map();
    void generate_view(std::vector<point> &view_point,
                       const int &num);
    
    int output_diff(const std::vector<point> &vertex_list1,
                    const std::vector<point> &vertex_list2,
                    const std::vector<vec> &normal_list1,
                    const std::vector<vec> &normal_list2,
                    const point &view_point);

    int output_diff(const std::vector<point> &vertex_list1,
                    const std::vector<point> &vertex_list2,
                    const std::vector<vec> &normal_list1,
                    const std::vector<vec> &normal_list2,
                    const std::vector<point> &view_point);

    void output_diff(const std::string &filename,
                    const std::vector<point> &vertex_list,
                    const std::vector<triangle> &tri_list,
                    const std::vector<double> &tri_error);

    void cal_tri_diff(const std::vector<point> &vertex_list1,
                      const std::vector<point> &vertex_list2,
                      const std::vector<vec> &normal_list1,
                      const std::vector<vec> &normal_list2,
                      const std::vector<triangle> &tri_list1,
                      const std::vector<triangle> &tri_list2,
                      std::vector<double> &tri_error,
                      std::vector<double> &vertex_error,
                      const point &view_point);

    void cal_tri_error(const std::vector<point> &vertex_list1,
                      const std::vector<point> &vertex_list2,
                      const std::vector<triangle> &tri_list1,
                      const std::vector<triangle> &tri_list2,
                      const point &view_point, const size_t flag,
                      std::vector<double> &tri_error);

    
    void get_zebra_map(const std::vector<triangle> &triangle_list,
                       const std::vector<point> &vertex_list,
                       const point &vec_plane,
                       const point &vertex_plane,
                       const point &view_point,
                       std::vector<point> &cross_point);
    
  protected:
    
    void get_diff(const point &vertex1, const vec &normal1,
                  const point &vertex2, const vec &normal2,
                  const point &view_point, double &diff);
    
  private:
    
    zjucad::matrix::matrix<double> mat_view;
    zjucad::matrix::matrix<double> mat_normal;
    zjucad::matrix::matrix<double> mat_trans_normal;
    zjucad::matrix::matrix<double> mat_vertex;
    zjucad::matrix::matrix<double> mat_sym_pos;
    zjucad::matrix::matrix<double> mat_reflect_map;
    std::fstream out;
    
    void cal_symmetry_pos(const point &view_point,
                          const vec &normal,
                          const point &vertex, point &symmetry_pos );

    void cal_reflect_map(const point &view_point,
                         const vec &normal,
                         const point &vertex, point &reflect_vertex);

    void get_reflection_vector(const point &view_point, const point &p,
                               const vec &normal, vec &refl_vec);
    
    void copy_point(zjucad::matrix::matrix<double> &mat,
                    const point &p);

    void copy_normal(zjucad::matrix::matrix<double> &normal,
                     const vec &vn);

    void copy_matrix(const zjucad::matrix::matrix<double> &vertex,
                     point &p);
    
    void line_plane_cross_point(const point &vec_line,
                                const point &vertex_line,
                                const point &vec_plane,
                                const point &vertex_plane,
                                point &cross_point);
    
  };
} 

#endif
