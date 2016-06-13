#include "coordinate_rotation.h"
#include <cmath>
#include <cassert>
#include "zjucad/matrix/io.h"
namespace lq
{


coordinate_rotation::coordinate_rotation()
{
  rotate_matrix_ =  zjucad::matrix::zeros(3, 3);
  rotate_matrix_2d_ = zjucad::matrix::zeros(3, 3);
}

/*******************************************************************************
  *@brief this function is static function of this class. It function is to 
  *rotate a point or vector p around axis with angle degree.
  *
  *@param axis : rotate axis
  *@param angle :  The angle of rotation
  *@param p : the point or vector that want to ratate
  *
  *return void
  *****************************************************************************/
void coordinate_rotation::execute_rotation(const vec &axis, const double angle,
                                           vec &p)
{
  p = p * cos(angle) + cross_mult(axis, p) * sin(angle) + 
      dot_mult(axis, p) * (1 - cos(angle)) * axis; 
}


/*************************************************************************************
 *@brief rotate two vectors normal1, normal2
 *
 *@param normal1 the vector which want to be rotated
 *@param normal2 the vector which want to be rotated
 *@param standard_axis the end vector of rotation
 *
 *return void
 ************************************************************************************/

void coordinate_rotation::execute_rotation(vec &normal1, vec &normal2,
                                           const vec &standard_axis)
{

  vec mid_normal = normal1 + normal2;
  assert(!is_zero(vec_norm(normal1)) && !is_zero(vec_norm(normal2)));
  normal1 = normal1 / vec_norm(normal1);
  normal2 = normal2 / vec_norm(normal2);
  mid_normal = mid_normal / vec_norm(mid_normal);
  double cos_angle_theta = dot_mult(normal1, normal2);
  double cos_theta = dot_mult(mid_normal, standard_axis);
  if(equal(cos_theta, 1.0) || equal(cos_theta, -1.0))
  {
    return;
  }
  if(equal(cos_angle_theta, 1.0) || equal(cos_angle_theta, -1.0))
  {
    return;
  }
  double angle_theta = acos(cos_angle_theta) / 2;
  //angle_theta = PI / 2 - angle_theta;
  rotate_matrix_normal_2d_(standard_axis, normal1, angle_theta);
  rotate_matrix_normal_2d_(standard_axis, normal2, 0 - angle_theta);
  assert(is_zero(normal1.x) && is_zero(normal2.x));
}

void coordinate_rotation::rotation_axis_with_angle(const vec &std_axis,
                                                   const double angle, 
                                                   vec &normal)
{
  rotate_matrix_normal_2d_(std_axis, normal, angle);
}

/******************************************************************************
 *@brief input two vectors and generate the rotation matrix between them
 *
 *@param mid_normal the first vector
 *@param standard_axis the second vector
 *
 *
 *return void 
 *
 *****************************************************************************/

void coordinate_rotation::generate_rotate_matrix(const vec &mid_normal,
                                                 const vec &standard_axis)
{
  assert(equal(vec_norm(mid_normal), 1) && equal(vec_norm(standard_axis), 1));
  double theta;
  double cos_theta;
  double sin_theta;
  cos_theta = dot_mult(mid_normal, standard_axis);

  vec rotate_axis = cross_mult(mid_normal, standard_axis);
  rotate_axis = rotate_axis / vec_norm(rotate_axis);

  theta = acos(cos_theta);
  sin_theta = sin(theta);
  
  rotate_matrix_(0, 0) = cos_theta + rotate_axis.x * rotate_axis.x *(1 - cos_theta);
  rotate_matrix_(0, 1) = rotate_axis.x * rotate_axis.y * (1 - cos_theta) -
                         rotate_axis.z * sin_theta;
  rotate_matrix_(0, 2) = rotate_axis.x * rotate_axis.z * (1 - cos_theta) +
                         rotate_axis.y * sin_theta;
  rotate_matrix_(1, 0) = rotate_axis.y * rotate_axis.x * (1 - cos_theta) +
                         rotate_axis.z * sin_theta;
  rotate_matrix_(1, 1) = cos_theta + rotate_axis.y * rotate_axis.y * (1 - cos_theta);
  rotate_matrix_(1, 2) = rotate_axis.y * rotate_axis.z * (1 - cos_theta) -
                         rotate_axis.x * sin_theta;
  rotate_matrix_(2, 0) = rotate_axis.z * rotate_axis.x * (1 - cos_theta) -
                         rotate_axis.y * sin_theta;
  rotate_matrix_(2, 1) = rotate_axis.z * rotate_axis.y * (1 - cos_theta) +
                         rotate_axis.x * sin_theta;
  rotate_matrix_(2, 2) = cos_theta + rotate_axis.z * rotate_axis.z * (1 - cos_theta);
}

void coordinate_rotation::generate_rotate_matrix_2d(const double theta)
{
  double sin_theta = sin(theta);
  double cos_theta = cos(theta);
  rotate_matrix_2d_(0, 0) = 0.0;
  rotate_matrix_2d_(0, 1) = 0.0;
  rotate_matrix_2d_(0, 2) = 0.0;
  rotate_matrix_2d_(1, 0) = 0.0;
  rotate_matrix_2d_(2, 0) = 0.0;
  rotate_matrix_2d_(1, 1) = cos_theta;
  rotate_matrix_2d_(1, 2) = 0 - sin_theta;
  rotate_matrix_2d_(2, 1) = sin_theta;
  rotate_matrix_2d_(2, 2) = cos_theta;
}

void coordinate_rotation::rotate_matrix_normal_2d_(const vec &standard_axis,
                                                   vec &normal,
                                                   const double theta)
{
  matrixd mat = zjucad::matrix::zeros(3, 1);
  normal_to_matrix(mat, standard_axis);
 // std::cout << "theta " << theta << std::endl;
  generate_rotate_matrix_2d(theta);
  //std::cout << "mat " << rotate_matrix_2d_<< std::endl;
  matrix_to_normal(rotate_matrix_2d_ * mat, normal);
}

void coordinate_rotation::normal_to_matrix(matrixd &mat, const vec &normal)
{
  assert(mat.size(1) == 3);
  mat(0, 0) = normal.x;
  mat(1, 0) = normal.y;
  mat(2, 0) = normal.z;
}

void coordinate_rotation::matrix_to_normal(const matrixd &mat, vec &normal)
{
  assert(mat.size(1) == 3);
  normal.x = mat(0, 0);
  normal.y = mat(1, 0);
  normal.z = mat(2, 0);
}




}
