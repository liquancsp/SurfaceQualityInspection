
#include "principal_curvature.h"
#include <ctime>
#include <iomanip>
#include <iostream>

namespace lq
{

using namespace std;

void principal_curvature::cal_tri_error(const std::vector<point> &vertex_list1,
                                        const std::vector<point> &vertex_list2,
                                        const std::vector<triangle> &tri_list1,
                                        const std::vector<triangle> &tri_list2,
                                        const point &view_point, const size_t flag,
                                        std::vector<double> &tri_error)
{
  cout << "====================================================================" << endl << endl;
  cout << "[#info-principal-algorithm]Begin to computer the different principal curvature of two models." << endl << endl;
  cout << "====================================================================" << endl << endl;

  vector<double> vertex_error;
  clock_t start, end;
  start = clock();
  cal_vertex_error_prin_cur(vertex_list1, vertex_list2, tri_list1, tri_list2, flag,
                            vertex_error);
  
  cout << "[#info-openmp]Thread number in principal is : " << 
          thread_num(vertex_list1.size(), MIN_ITERATOR_NUM) <<endl;
  double diff;
  double max_diff = 0.0f;
  size_t max_index;
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
  cout << "[#info-principal-algorithm]---The maximum triangle error is : " << 
          setiosflags(ios::fixed) << setprecision(10) << max_diff << endl;
//  cout << "The maximum triangle index is : " << setiosflags(ios::fixed) << setprecision(10) <<
//          max_index << endl;
  cout << "[#info-principal-algorithm]---The cost time  is : " <<
          setiosflags(ios::fixed) << setprecision(5) <<
          difftime(end, start) / CLOCKS_PER_SEC << " seconds." << endl;
}


void principal_curvature::cal_principal_curvature(const std::vector<point> &vertex_list,
                                                  const std::vector<triangle> &tri_list,
                                                  std::vector<principal_curvature_info> &prin_cur_list)
{
  curvature::cal_principal_curvature(vertex_list, tri_list, prin_cur_list);
}

void principal_curvature::cal_vertex_error_prin_cur(const std::vector<point> &vertex_list1,
                                                    const std::vector<point> &vertex_list2,
                                                    const std::vector<triangle> &tri_list1,
                                                    const std::vector<triangle> &tri_list2,
                                                    const size_t flag,
                                                    std::vector<double> &vertex_error)
{
  initialize();
  vector<double> mean_cur1, mean_cur2;
  vector<double> gauss_cur1, gauss_cur2;
  vector<principal_curvature_info> prin_cur_list1, prin_cur_list2;
//#pragma omp parallel
//  {
//#pragma omp sections
//    {
//#pragma omp section
//      {
        get_vertex_adj_tri(tri_list1, vertex_list1, adj_tri_list1);
        get_vertex_adj_tri(tri_list2, vertex_list2, adj_tri_list2);
//      }
//#pragma omp section
//      {
        get_tri_info(tri_list1, vertex_list1, tri_info_list1);
        get_tri_info(tri_list2, vertex_list2, tri_info_list2);
//      }
//    }
//  }
//#pragma omp sections
//    {
//#pragma omp section
//      {
        cal_mean_curvature(vertex_list1, tri_list1, adj_tri_list1, 
                           tri_info_list1, mean_cur1);
        cal_mean_curvature(vertex_list2, tri_list2, adj_tri_list2,
                           tri_info_list2, mean_cur2);
//      }
//#pragma omp section
//      {
        cal_gaussian_curvature(vertex_list1, tri_list1, adj_tri_list1,
                               tri_info_list1, gauss_cur1);
        cal_gaussian_curvature(vertex_list2, tri_list2, adj_tri_list2,
                               tri_info_list2, gauss_cur2);
//      }
//    }

//#pragma omp parallel 
 // {

//    }
//  }
//#pragma omp parallel
//  {
//#pragma omp sections
//    {
      
    
//#pragma omp section
 //     {
        prin_cur_list1.resize(vertex_list1.size());
#pragma omp parallel for num_threads(thread_num(vertex_list1.size(), MIN_ITERATOR_NUM))
        for(int i = 0; i < vertex_list1.size(); ++i)
        {
         //cout << "i : " << i << endl;
          double delta_x = 0.0f;
          principal_curvature_info tmp_cur1;
          delta_x = mean_cur1[i] * mean_cur1[i] - gauss_cur1[i];
          if(is_less(delta_x, 0.0))
          {
            delta_x = 0.0;
          }
          tmp_cur1.k1 = mean_cur1[i] + sqrt(delta_x);
          tmp_cur1.k2 = mean_cur1[i] - sqrt(delta_x);
          prin_cur_list1[i] = tmp_cur1;
        }
//        for(size_t i = 0; i < prin_cur_list1.size(); ++i)
//        {
//          double sum_prin = (prin_cur_list1[i].k1 + prin_cur_list1[i].k2) / 2;
//          if(!equal(sum_prin, mean_cur1[i]))
//          {
//            cout << "[#error-principal-model1]Principal k1 is error in " << i << endl;
//            cout << "[#error-principal-model1]right mean is : " << mean_cur1[i] << endl;
//            cout << "[#error-principal-model1]Wrong value is : " << sum_prin << endl;
//          }
//        }
 //     }
//#pragma omp section
//      {
        prin_cur_list2.resize(vertex_list2.size());
#pragma omp parallel for num_threads(thread_num(vertex_list1.size(), MIN_ITERATOR_NUM))
        for(int i = 0; i < vertex_list2.size(); ++i)
        {
          double delta_x = 0.0f;
          principal_curvature_info tmp_cur2;
          delta_x = mean_cur2[i] * mean_cur2[i] - gauss_cur2[i];
          if(is_less(delta_x, 0.0))
          {
            delta_x = 0.0;
          }
          if(i == 5297)
          {
            cout << "[#info]Delta_x is : " << delta_x << endl;
          }
          tmp_cur2.k1 = mean_cur2[i] + sqrt(delta_x);
          tmp_cur2.k2 = mean_cur2[i] - sqrt(delta_x);
          prin_cur_list2[i] = tmp_cur2;
        }
//        for(size_t i = 0; i < prin_cur_list2.size(); ++i)
//        {
//          double sum_prin = (prin_cur_list2[i].k1 + prin_cur_list2[i].k2) / 2;
//          if(!equal(sum_prin, mean_cur2[i]))
//          {
//            cout << "[#error-principal-model2]Principal is error in " << i << endl;
//            cout << "[#error-principal-model2]right mean is : " << mean_cur1[i] << endl;
//            cout << "[#error-principal-model2]Wrong sum_prin value is : " << sum_prin << endl;
//            cout << "[#error-principal-model2]Wrong k1 is : " << prin_cur_list2[i].k1 <<
//                    " k2 : " << prin_cur_list2[i].k2 << endl;
//            cout << "[#error=principal-modle2]Gaussian is " << gauss_cur2[i] << endl;
//            cout << "==========================================================================" << endl;
//          }
//        }
  //    }
 //   }
//  }
  vertex_error.resize(vertex_list1.size());
  
  if(flag == kPrincipalK1)
  {
//#pragma omp parallel for num_threads(thread_num(vertex_list1.size(), MIN_ITERATOR_NUM))
    for(size_t i = 0; i < vertex_list1.size(); ++i)
    {
      double diff;
      diff = fabs(prin_cur_list1[i].k1 - prin_cur_list2[i].k1);
      if(!is_zero(diff))
        vertex_error[i] = diff;
      else 
        vertex_error[i] = 0.0;
    }
  }
  else if(flag == kPrincipalK2)
  {
//#pragma omp parallel for num_threads(thread_num(vertex_list1.size(), MIN_ITERATOR_NUM))
    for(size_t i = 0; i < vertex_list1.size(); ++i)
    {
      double diff;
      diff = fabs(prin_cur_list1[i].k2 - prin_cur_list2[i].k2);
      if(!is_zero(diff))
        vertex_error[i] = diff;
      else 
        vertex_error[i] = 0.0;
    }
  }
}

};
