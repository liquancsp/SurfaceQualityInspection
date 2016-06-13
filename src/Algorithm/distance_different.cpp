#include "distance_different.h"
#include <fstream>
#include <iomanip>
#include <iostream>

namespace lq
{

using namespace std;

distance_different::distance_different()
{
   log_out_.open("operation_log.txt", fstream::out | fstream::app);
}

distance_different::~distance_different()
{

}

void distance_different::cal_tri_error(const std::vector<point> &vertex_list1,
                                       const std::vector<point> &vertex_list2,
                                       const std::vector<triangle> &tri_list1,
                                       const std::vector<triangle> &tri_list2,
                                       const point &view_point, const size_t flag,
                                       std::vector<double> &tri_error)
{
  
  cout << "==============================================================" << endl << endl;
  cout << "[#info-distance-algorithm]Begin to computer the different distance of two models." << endl << endl;
  cout << "==============================================================" << endl << endl;
  vector<double> vertex_error_list;
  double max_vertex_error = 0.0;
  double max_tri_error = 0.0;
  double vertex_error;
 
  size_t max_vertex_error_index;
  size_t max_tri_error_index = 0;
  for(size_t i = 0; i < vertex_list1.size(); ++i)
  {
    distance_two_point(vertex_list1[i], vertex_list2[i], vertex_error);
    if(is_zero(vertex_error))
    {
      vertex_error_list.push_back(0.0);
    }
    else
    {
      vertex_error_list.push_back(vertex_error);
    }
    if(is_larger(vertex_error, max_vertex_error))
    {
      max_vertex_error = vertex_error;
      max_vertex_error_index = i + 1;
    }
  }
  
  for(size_t i = 0; i < tri_list1.size(); ++i)
  {
    double triangle_error = 0.0;
    for(size_t j = 0; j < 3; ++j)
    {
      triangle_error += vertex_error_list[tri_list1[i].vertex[j] - 1];
    }
    if(is_larger(triangle_error, max_tri_error))
    {
      max_tri_error = triangle_error;
    }
    if(is_zero(triangle_error))
    {
      tri_error.push_back(0.0);
    }
    else
    {
      tri_error.push_back(triangle_error);
    }
  }
  tri_error.push_back(max_tri_error);
  
  cout << "[#info-distance-algorithm]---The maximum point distance is : " << 
          setiosflags(ios::fixed) << setprecision(8) << max_vertex_error << endl;
  cout << "[#info-distance-algorithm]---The point index is : " << 
          max_vertex_error_index << endl;
  cout << "[#info-distance-algorithm]---The maximum triangle error is : " <<
          setiosflags(ios::fixed) << setprecision(8) << max_tri_error << endl;
  cout << "[#info-distance-algorithm]---The triangle index is : " << 
          max_tri_error_index << endl;
}

void distance_different::distance_two_point(const point &p1, const point &p2,
                                            double &dist)
{
  dist = vec_norm(p1 - p2);
}

}
