/*******************************************************************************
 *COPYRIGHT NOTICE  
 *Copyright (c) 2012, Zhe Jiang University 
 *All rights reserved.
 *
 *@file mean_curvature.h
 *@author li quan
 *@version 1.0
 *@date 2012-11
 *@ingroup diff
 *@brief the function of this class is to compute the different of two model use mean curvature.
 *
 *@history
 ********************************************************************************/

#ifndef MEAN_CURVATURE_H
#define MEAN_CURVATURE_H
#include "data_type.h"
#include "../3rd/zjucad/matrix/matrix.h"
#include "algorithm_base.h"
#include "curvature.h"
#include <fstream>

#include <map>

namespace lq
{

  class mean_curvature : public curvature
  {

 public:

    mean_curvature();
    ~mean_curvature();

//    void cal_tri_diff(const std::vector<point> &vertex_list1,
//                      const std::vector<point> &vertex_listt2,
//                      const std::vector<triangle> &tri_list1,
//                      const std::vector<triangle> &tri_list2,
//                      std::vector<double> &tri_error);



    void cal_tri_error(const std::vector<point> &vertex_list1,
                       const std::vector<point> &vertex_list2,
                       const std::vector<triangle> &tri_list1,
                       const std::vector<triangle> &tri_list2,
                       const point &view_point, const size_t flag,
                       std::vector<double> &tri_error);

    void cal_mean_curvature(const std::vector<point> &vertex_list,
                            const std::vector<triangle> &tri_list,
                            std::vector<double> &mean_cur);

//    void cal_gaussian_curvature(const std::vector<point> &vertex_list,
//                                const std::vector<triangle> &tri_list,
//                                std::vector<double> &gauss_cur_list);

//    void cal_principal_curvature(const std::vector<point> &vertex_list,
//                                 const std::vector<triangle> &tri_list,
//                                 std::vector<principal_curvature_info> &prin_cur_list);



 private:

    std::fstream log_out;    
    void cal_circumcircle_radius(const double edge_len[3], double &circumcircle_r);

    double get_curvature_diff(const vec &vec1, const vec &vec2);

    void output_curvature_vtk_file(const std::vector<point> &vertex_list,
                                   const std::vector<triangle> &tri_list,
                                   const std::vector<double> &tri_curvature);


  };
  

}
#endif
