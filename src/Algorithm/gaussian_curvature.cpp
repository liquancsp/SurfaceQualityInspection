#include "gaussian_curvature.h"
#include <ctime>
#include <iomanip>
#include <iostream>
namespace lq
{

using namespace std;

gaussian_curvature::gaussian_curvature() : curvature()
{
  log_out.open("operation_log.txt", fstream::out | fstream::app);
}

void gaussian_curvature::cal_tri_error(const std::vector<point> &vertex_list1,
                                       const std::vector<point> &vertex_list2,
                                       const std::vector<triangle> &tri_list1,
                                       const std::vector<triangle> &tri_list2,
                                       const point &view_point, const size_t flag,
                                       std::vector<double> &tri_error)
{
  cout << "====================================================================" << endl << endl;
  cout << "[#info-gaussian-algorithm]Begin to computer  the different gaussian curvature of two models." << endl << endl;
  cout << "====================================================================" << endl << endl;
  double diff, max_error;
  clock_t start, end;
  //std::ofstream fout;
  vector<double> vertex_error;
  initialize();
  start = clock();
  curvature::get_vertex_adj_tri(tri_list1, vertex_list1, curvature::adj_tri_list1);
  curvature::get_vertex_adj_tri(tri_list2, vertex_list2, curvature::adj_tri_list2);
  curvature::get_tri_info(tri_list1, vertex_list1, curvature::tri_info_list1);
  curvature::get_tri_info(tri_list2, vertex_list2, curvature::tri_info_list2);
  max_error = 0.0;
  size_t num1 = 0;
  size_t num2 = 0;
  size_t max_index;
  double gauss1, gauss2;
  double sum_gauss1 = 0.0f;
  double sum_gauss2 = 0.0f;
  max_error = -10000.0f;
  //ofstream fout("gauss_curvature_error.txt");
  for(size_t i = 0; i < vertex_list1.size(); ++i)
  {
    curvature::cal_gaussian_curvature(i, adj_tri_list1[i], tri_info_list1,
                                       gauss1, sum_gauss1);
    curvature::cal_gaussian_curvature(i, adj_tri_list2[i], tri_info_list2,
                                      gauss2, sum_gauss2);
//    fout << setiosflags(ios::fixed) << setprecision(10) << gauss1 << " " << gauss2 << endl;
    diff = fabs(gauss1 - gauss2);
//    fout << setiosflags(ios::fixed) << setprecision(10) << " diff : " << diff << endl;
    if(is_larger(diff, max_error))
    {
      max_error = diff;
      max_index = i;
    }
    if(!is_zero(diff))
    {
      num1++;
      vertex_error.push_back(diff);
    }
    else
      vertex_error.push_back(0.0);
  }
  double max_diff = 0.0;
  for(size_t i = 0; i < tri_list1.size(); ++i)
  {
    diff = 0 ;
    for(size_t j = 0; j < 3; ++j)
      diff += vertex_error[tri_list1[i].vertex[j] - 1];
    if(is_larger(diff, max_diff))
      max_diff = diff;
    if(!is_zero(diff))
    {
      num2++;
      tri_error.push_back(diff);
    }
    else
      tri_error.push_back(0.0);
  }
  tri_error.push_back(max_diff);
  vertex_error.clear();
  curvature::reset();
  end = clock();
  cout << "The maximum vertex index is : " << max_index << endl;
  cout << "The maximum vertex error is : " << setiosflags(ios::fixed) << 
          setprecision(10) << max_error << endl;
  cout << "The maximum triangle error is : " << setiosflags(ios::fixed) << 
          setprecision(10) << max_diff << endl;
  cout << "The number of error point  is : " << num1 << endl;
  cout << "The number of error triangle  is : " << num2 << endl;
  cout << "The cost time of compute the different of two model is : " <<
          setiosflags(ios::fixed) << setprecision(5) <<
          difftime(end, start) / CLOCKS_PER_SEC << " seconds." << endl;
}

void gaussian_curvature::cal_gaussian_curvature(const std::vector<point> &vertex_list,
                                                const std::vector<triangle> &tri_list,
                                                std::vector<double> &gauss_cur_list)
{
  curvature::cal_gaussian_curvature(vertex_list, tri_list, gauss_cur_list);
}

};
