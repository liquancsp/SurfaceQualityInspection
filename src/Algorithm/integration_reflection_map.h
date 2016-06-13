/*******************************************************************************
 *COPYRIGHT NOTICE  
 *Copyright (c) 2012, Zhe Jiang University 
 *All rights reserved.
 *
 *@file integration_reflection_map.h
 *@author li quan
 *@version 1.0
 *@date 2013-4-7
 *@ingroup algorithm
 *@brief the function of this class is to compute the integrate different of two model and
 *   test the result.
 *
 *@history
 ********************************************************************************/
#ifndef INTEGRATION_REFLECTION_MAP_H
#define INTEGRATION_REFLECTION_MAP_H

#include "coordinate_rotation.h"
#include "data_type.h"
#include "algorithm_base.h"
#include <fstream>
namespace lq
{

  /**
   *@class integration_reflection_map
   *@ingroup algorithm
   *@brief the function of this class is to compute the integrate different of two model and
   *   test the result.
   *
   */
  class integration_reflection_map : public algorithm_base
  {

 public:
    
    integration_reflection_map();
    ~integration_reflection_map();

    void cal_tri_error(const std::vector<point> &vertex_list1,
                       const std::vector<point> &vertex_list2,
                       const std::vector<triangle> &tri_list1,
                       const std::vector<triangle> &tri_list2,
                       const point &view_point, const size_t flag,
                       std::vector<double> &tri_error);
    
    
    
    static void cal_tri_integrate_error(const vec &normal1,
                                        const vec &normal2,
                                        const double angle_theta,
                                        const double theta,
                                        double &value);
    
    static void get_normal_range_angle(const vec &normal1,
                                       const vec &normal2,
                                       double &theta);
    static void cal_func_value(const double r, const double theta, 
                               const vec &normal1, const vec &normal2,
                               double &result);
 private:

    


    std::vector<vertex_normal> tri_normal_;
    coordinate_rotation rotate_tool_;
    std::fstream log_out_;
  };

}

#endif
