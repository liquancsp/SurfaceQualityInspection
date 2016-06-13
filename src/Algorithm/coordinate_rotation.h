/*************************************************************************************
 *
 *COPYRIGHT NOTICE
 *Copyright (c) 2013, Zhe Jiang University
 *All rights reserved.
 *
 *@file coordinate_rotation.h
 *@author li quan
 *
 *version 1.0
 *data 2013-4-4
 *ingroup algorithm/coordinate_rotation.h
 *
 *@brief  the function of this class is to deal with coordinate rotation by rotate matrix
 *
 *************************************************************************************/
#ifndef COORDINATE_ROTATION_H
#define COORDINATE_ROTATION_H

#include "data_type.h"

namespace lq
{

   /***************************************************************************************
   *@class coordinate_rotation
   *@brief   the function of this class is to deal with coordinate rotation by rotate matrix
   *
   *@detail
   *
   *@data 2013-4-4
   *@version 1.0
   ****************************************************************************************/
  class coordinate_rotation
  {
    
 public:

    coordinate_rotation();
    void execute_rotation(const vec &axis, const double angle,
                                 vec &p);
    void execute_rotation(vec &normal1, vec &normal2,
                                 const vec &standard_axis);
    
    void rotation_axis_with_angle(const vec& std_axis, const double angle, vec& normal);
    
//    template<typename T1, typename T2>
//    static void rotate_vector(const T1& vec1, T1&vec2, T2& angle)
//    {
      
//    }
    
//    template<typename T>
//    static void rotate_2_vec_symmetry(const T& std_axis, T& vec1, T& vec2)
//    {
//      T mid_normal = vec1 + vec2;
//      assert(mid_normal.Norm() != 0.0f);
//      mid_normal.normalized();
//      vec1.normalized();
//      vec2.normalized();
//      std_axis.normalized();
//      double cos_angle_theta = vec1 * vec2;
//      double cos_theta = mid_normal * std_axis;
//      if(equal(cos_theta, 1.0) || equal(cos_theta, -1.0))
//      {
//        return;
//      }
//      if(equal(cos_angle_theta, 1.0) || equal(cos_angle_theta, -1.0))
//      {
//        return;
//      }
//      double angle_theta = acos(cos_angle_theta) / 2;
////      vec mid_normal = normal1 + normal2;
////      assert(!is_zero(vec_norm(normal1)) && !is_zero(vec_norm(normal2)));
////      normal1 = normal1 / vec_norm(normal1);
////      normal2 = normal2 / vec_norm(normal2);
////      mid_normal = mid_normal / vec_norm(mid_normal);
////      double cos_angle_theta = dot_mult(normal1, normal2);
////      double cos_theta = dot_mult(mid_normal, standard_axis);
////      if(equal(cos_theta, 1.0) || equal(cos_theta, -1.0))
////      {
////        return;
////      }
////      if(equal(cos_angle_theta, 1.0) || equal(cos_angle_theta, -1.0))
////      {
////        return;
////      }
////      double angle_theta = acos(cos_angle_theta) / 2;
////      //angle_theta = PI / 2 - angle_theta;
////      rotate_matrix_normal_2d_(standard_axis, normal1, angle_theta);
////      rotate_matrix_normal_2d_(standard_axis, normal2, 0 - angle_theta);
////      assert(is_zero(normal1.x) && is_zero(normal2.x));
//    }
    
 private:

    void generate_rotate_matrix(const vec &mid_normal, const vec &standard_axis);
    void generate_rotate_matrix_2d(const double theta);
    matrixd rotate_matrix_;
    matrixd rotate_matrix_2d_;
    void normal_to_matrix(matrixd &mat, const vec &normal);
    void matrix_to_normal(const matrixd &mat, vec &normal);

    void rotate_matrix_normal_2d_(const vec &standard_axis,
                                  vec &normal,
                                  const double theta);

  };

}


#endif
