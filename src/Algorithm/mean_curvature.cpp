#include "mean_curvature.h"
#include "coordinate_rotation.h"
#include <ctime>
#include <iomanip>
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
//#include <algorithm>
namespace lq {
using namespace std;

mean_curvature::mean_curvature()
{
  log_out.open("operation_log.txt", fstream::out | fstream::app);
}
 
mean_curvature::~mean_curvature()
{
  log_out.close();
}

void mean_curvature::cal_tri_error(const vector<point> &vertex_list1,
                                   const vector<point> &vertex_list2,
                                   const vector<triangle> &tri_list1,
                                   const vector<triangle> &tri_list2,
                                   const point &view_point, const size_t flag,
                                   vector<double> &tri_error)
{
  cout << "====================================================================" << endl << endl;
  cout << "[#info-mean-algorithm]Begin to computer the different mean curvature of two models." << endl << endl;
  cout << "====================================================================" << endl << endl;
  double diff, max_error = -10000.0f;
  clock_t start, end;
  //ofstream fout("mean_curvature_error.txt");
  vector<double> vertex_error;
  double sum_area;
  initialize();
  curvature::get_vertex_adj_tri(tri_list1, vertex_list1, curvature::adj_tri_list1);
  curvature::get_vertex_adj_tri(tri_list2, vertex_list2, curvature::adj_tri_list2);
  curvature::get_tri_info(tri_list1, vertex_list1, curvature::tri_info_list1);
  curvature::get_tri_info(tri_list2, vertex_list2, curvature::tri_info_list2);
  start = clock();
  max_error = 0.0;
  size_t num1 = 0;
  size_t num2 = 0;
  size_t max_index = 0;
  vertex_error.resize(vertex_list1.size());
  cout << "[#info-openmp]Thread number in mean curvature is : " << 
          thread_num(tri_list1.size(), MIN_ITERATOR_NUM) << endl;
#pragma omp parallel for num_threads(thread_num(vertex_list1.size(), MIN_ITERATOR_NUM)) \
  private(diff)
  for(int i = 0; i < vertex_list1.size(); ++i)
  {
    vec cur_vec1(0, 0, 0);
    vec cur_vec2(0, 0, 0);
    double diff;
    curvature::cal_mean_curvature(i, adj_tri_list1[i], tri_info_list1, 
                                  vertex_list1, cur_vec1, sum_area);
    curvature::cal_mean_curvature(i, adj_tri_list2[i], tri_info_list2, 
                                  vertex_list2, cur_vec2, sum_area);
    //fout << setiosflags(ios::fixed) << setprecision(10) << vec_norm(cur_vec1) << " " << vec_norm(cur_vec2) << endl;
    diff = fabs(vec_norm(cur_vec1) - vec_norm(cur_vec2));
    diff /= 2;
    //fout << setiosflags(ios::fixed) << setprecision(10) << " diff : " << diff << endl;
    //std::cout << diff << std::endl;
    if(is_larger(diff, max_error))
    {
      max_error = diff;
      max_index = i;
    }
    
    if(!is_zero(diff))
    {
      num1++;
      vertex_error[i] = diff;
    }
    else
      vertex_error[i] = 0.0;
  }
  std::cout << "[#info]Vertex error vector size is : " << vertex_error.size() << std::endl;
  double max_diff = 0.0;
  tri_error.resize(tri_list1.size());
  
#pragma omp parallel for num_threads(thread_num(tri_list1.size(), MIN_ITERATOR_NUM))\
  private(diff)
  for(int i = 0; i < tri_list1.size(); ++i)
  {
    diff = 0.0;
    for(size_t j = 0; j < 3; ++j)
    {
      
      diff += vertex_error[tri_list1[i].vertex[j] - 1];
    }
    if(is_larger(diff, max_diff))
    {
      max_diff = diff;
      max_index = i;
    }
    if(!is_zero(diff))
    {
      num2++;
      tri_error[i] = diff;
    }
    else
    {
      tri_error[i] = 0.0;
     
    }
  }
  tri_error.push_back(max_diff);
  curvature::reset();
  end = clock();
  cout << "[#info-mean-algorithm]---Triagnle error vector size is : " << 
          tri_error.size() << std::endl;
  cout << "[#info-mean-algorithm]---The maximum triangle index is : " << 
          max_index << endl;
  cout << "[#info-mean-algorithm]---The maximum vertex error is : " << 
          setiosflags(ios::fixed) << setprecision(10) <<
          max_error << endl;
  cout << "[#info-mean-algorithm]---The maximum triangle error is : " << 
          setiosflags(ios::fixed) << setprecision(10) <<
          max_diff << endl;
  cout << "[#info-mean-algorithm]---The number of error point  is : " <<
          num1 << endl;
  cout << "[#info-mean-algorithm]---The number of error triangle  is : " << 
          num2 << endl;
  cout << "[#info-mean-algorithm]---The cost time is : " <<
          setiosflags(ios::fixed) << setprecision(5) <<
          difftime(end, start) / CLOCKS_PER_SEC << " seconds." << endl;
}

void mean_curvature::cal_mean_curvature(const std::vector<point> &vertex_list,
                                        const std::vector<triangle> &tri_list,
                                        std::vector<double> &mean_cur)
{
  curvature::cal_mean_curvature(vertex_list, tri_list, mean_cur);
}


/*********************************************************************************************
 *Begin private function
 */


void mean_curvature::cal_circumcircle_radius(const double edge_len[3], double &circumcircle_r)
{
  double num1 ,num2, num3, num4;
  num1 = edge_len[0] + edge_len[1] + edge_len[2];
  num2 = edge_len[1] + edge_len[2] - edge_len[0];
  num3 = edge_len[0] + edge_len[2] - edge_len[1];
  num4 = edge_len[0] + edge_len[1] - edge_len[2];
  circumcircle_r = edge_len[0] * edge_len[1] * edge_len[2] / sqrt(num1 * num2 * num3 * num4);
}

double mean_curvature::get_curvature_diff(const vec &vec1, const vec &vec2)
{
  return vec_norm(vec1 - vec2);
}

}
