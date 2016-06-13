#include "integration_reflection_map.h"
#include <cmath>
#include <fstream>
#include <cassert>
#include <ctime>
#include <iomanip>
#include <iostream>


namespace lq
{
using namespace std;

integration_reflection_map::integration_reflection_map()
{
  log_out_.open("operation_log.txt", fstream::out | fstream::app);
}

integration_reflection_map::~integration_reflection_map()
{
  
}

void integration_reflection_map::cal_tri_error(const std::vector<point> &vertex_list1,
                                               const std::vector<point> &vertex_list2,
                                               const std::vector<triangle> &tri_list1,
                                               const std::vector<triangle> &tri_list2,
                                               const point &view_point, const size_t flag,
                                               std::vector<double> &tri_error)
{
  cout << "=====================================================================" << endl;
  cout << "[#info-integral-algorithm]Begin to computer the different integrate of two models." << endl;
  cout << "=====================================================================" << endl << endl;
  vec standard_axis(0, 0, 1);
  clock_t start, end;
  double max_error = 0.0;
  size_t index = 0;
  start = clock();
  tri_error.resize(tri_list1.size());
  omp_lock_t lock;
  omp_init_lock(&lock);
  std::cout << "[#info-openmp]Thread number in integration reflection : " << 
               thread_num(tri_list1.size(), MIN_ITERATOR_NUM) << endl;
  size_t i;
#pragma omp parallel for num_threads(thread_num(tri_list1.size(), MIN_ITERATOR_NUM))
  for(int i = 0; i < tri_list1.size(); ++i)
  {
    double value;
    value = 0.0;
    if(!equal(tri_list1[i].triangle_normal, tri_list2[i].triangle_normal))
    {
      vec normal1, normal2;
      normal1 = tri_list1[i].triangle_normal;
      normal2 = tri_list2[i].triangle_normal;
      normal1 = normal1 / vec_norm(normal1);
      normal2 = normal2 / vec_norm(normal2);
      double normal_theta;
      coordinate_rotation rotate_tool;
      rotate_tool.execute_rotation(normal1, normal2, standard_axis);
      get_normal_range_angle(normal1, normal2, normal_theta);
      cal_tri_integrate_error(normal1, normal2, normal_theta, PI / 2, value);
      value *= 4;
    }
   // omp_set_lock(&lock);
    if(is_larger(value, max_error))
    {
      max_error = value;
      index = i;
    }
    //omp_unset_lock(&lock);
    if(is_zero(value))
    {
      tri_error[i] = 0.0;
    }
    else
      tri_error[i] = value;
  }
  //set max_error at the end of vector
  tri_error.push_back(max_error);
  end = clock();
  cout << "[#info-integral-algorithm]---The maximum integrate error is : " <<
          setiosflags(ios::fixed) <<
          setprecision(8) << max_error << endl;
  cout << "[#info-integral-algorithm]---The index of maximun error is : " << 
          setiosflags(ios::fixed) <<
          setprecision(8) << index << endl;
//  cout << "[#info-integral-algorithm]---The maximun angle between two normal is : " << 
//          setiosflags(ios::fixed) <<
//          setprecision(8) << max_theta << endl;
  cout << "[#info-integral-algorithm]----The cost time is : " <<
          setiosflags(ios::fixed) << setprecision(5) <<
          difftime(end, start) / CLOCKS_PER_SEC << " seconds." << endl;
}


void integration_reflection_map::cal_tri_integrate_error(const vec &normal1,
                                                         const vec &normal2,
                                                         const double normal_theta,
                                                         const double theta,
                                                         double &value)
{
//  double tmp_theta = dot_mult(side1, side2);
  double r ;
//  assert(is_less_equal(tmp_theta, 1) && is_larger_equal(tmp_theta, -1.0) );
//  if(equal(tmp_theta, 1.0) || equal(tmp_theta, -1.0))
//  {
//    return;
//  }
//  else//get the radius of the circle of projection from spherical cap
//  {
//    r = sin(acos(tmp_theta) / 2);
//  }
  r = sin(normal_theta / 2.0f);
  if(is_zero(r))
  {
    return;
  }
  //cout << "r " << r << endl;
  size_t div = 50;
  //Discrete Sampling
  double delta_r = r / div;
  double delta_theta = theta / div;
  double polar_r, polar_theta;
  double tmp = 0.0;
  double start_theta = delta_theta / 2;
  double start_r = delta_r / 2;
  //log_out_ <<"[#info]Divide number is : " << div << endl;
  for(size_t i = 0; i < div; ++i)
  {
    for(size_t j = 0; j < div; ++j)
    {
      polar_r = start_r + i * delta_r;
      polar_theta =  start_theta + j * delta_theta;
      cal_func_value(polar_r, polar_theta, normal1, normal2, tmp);
      tmp = tmp * delta_r * delta_theta;
      //std::cout << "tmp : " << tmp * 180 / PI << std::endl;
      value += tmp;
    }
  }
  //std::cout << "value : " << value * 180 / PI << std::endl;
  
}

void integration_reflection_map::cal_func_value(const double r, const double theta, 
                                                const vec &normal1,
                                                const vec &normal2,
                                                double &result)
{
  double sin_theta, cos_theta;
  double const_r;
  sin_theta = sin(theta);
  cos_theta = cos(theta);
  assert(is_less(r, 1));
  const_r = sqrt(1 - r * r);

  vertex_normal polar_vec(r * cos_theta, r * sin_theta, const_r);
  assert(equal(vec_norm(polar_vec), 1.0));
  double const_n1_v, const_n2_v, const_n1_n2;

  const_n1_v = dot_mult(normal1, polar_vec);
  const_n2_v = dot_mult(normal2, polar_vec);
  const_n1_n2 = dot_mult(normal1, normal2);

  assert(is_larger_equal(const_n1_v * const_n1_v + const_n2_v * const_n2_v -
                         2 * const_n1_v * const_n2_v * const_n1_n2, 0.0));
  result = 2 * r * cos_theta * sqrt(const_n1_v * const_n1_v + const_n2_v * const_n2_v -
                                    2 * const_n1_v * const_n2_v * const_n1_n2) / const_r;
//  cout << " r " << r << endl;
//  cout << " theta : " << theta * 180 / PI << endl;
//  cout << " cos_theta " <<  cos_theta << endl;
  //std::cout << "result " << result << std::endl;
  assert(is_larger_equal(result, 0.0));

  
}

void integration_reflection_map::get_normal_range_angle(const vec &normal1,
                                                        const vec &normal2,
                                                        double &theta)
{
  double cos_theta = dot_mult(normal1, normal2);
  if(equal(cos_theta, 1.0) || equal(cos_theta, -1.0))
  {
    theta = 0.0;
    return ;
  }
  else
  {
    theta = PI - acos(cos_theta);
  }
}

}
