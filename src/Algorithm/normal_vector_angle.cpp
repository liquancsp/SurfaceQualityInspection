#include "normal_vector_angle.h"
#include <iomanip>
#include <iostream>
namespace lq
{

using namespace std;

normal_vector_angle::normal_vector_angle()
{
  log_out_.open("operation_log.txt", fstream::out | fstream::app);
}

normal_vector_angle::~normal_vector_angle()
{

}

void normal_vector_angle::cal_tri_error(const std::vector<point> &vertex_list1,
                                        const std::vector<point> &vertex_list2,
                                        const std::vector<triangle> &tri_list1,
                                        const std::vector<triangle> &tri_list2,
                                        const point &view_point, const size_t flag,
                                        std::vector<double> &tri_error)
{
  cout << "=============================================================" << endl << endl;
  cout << "[#info-normal-angle-algorithm]Begin to compute the different normal vector angle of two models." << endl << endl;
  cout << "=============================================================" << endl << endl;
  vec normal1, normal2;
  double max_error = 0.0;
  double error;
  size_t max_index;
  double cos_theta;
  for(size_t i = 0; i < tri_list1.size(); ++i)
  {
    normal1 = tri_list1[i].triangle_normal;
    normal2 = tri_list2[i].triangle_normal;
    normal1 = normal1 / vec_norm(normal1);
    normal2 = normal2 / vec_norm(normal2);
    cos_theta = dot_mult(normal1, normal2);
    if(equal(cos_theta, 1.0))
    {
      error = 0.0;
    }
    else
    {
      error = acos(cos_theta) * 180 / PI;
    }
    if(is_larger(error, max_error))
    {
      max_error = error;
      max_index = i + 1;
    }
    tri_error.push_back(error);
  }
  tri_error.push_back(max_error);
//  for(size_t i = 0; i < tri_error.size(); ++i)
//  {
//    if(!is_zero(max_error))
//      tri_error[i] /= max_error;
//  }
  cout << "[#info-normal-angle-algorithm]---The maximum error is : " << 
          setiosflags(ios::fixed) << setprecision(8) << max_error << endl;
  cout << "[#info-normal-angle-algorithm]---The maximu triangle index is : " << 
          max_index << endl;
}

}
