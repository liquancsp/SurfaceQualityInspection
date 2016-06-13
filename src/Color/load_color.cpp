#include "load_color.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <string>
namespace lq {
using namespace std;
const double COLOR_ACCURACY = 1e-2;
const double EXPANSION = 1e+5;
std::vector<double> load_color::init_scalar_;

load_color::load_color()
{
  out.open("operation_log.txt", fstream::out | fstream::app);
  is_scale = false;
  pre_scalar_ = 1.0f;
}

load_color::~load_color()
{
  out.close();
}

bool cmp(const color & c1, const color &c2)
{
  return (c1.scalar < c2.scalar);
}

bool is_equal(const color &c1, const color &c2)
{
  return (fabs(c1.scalar - c2.scalar) < COLOR_ACCURACY);
}

int load_color::init_color(std::vector<color> &color_list)
{
 
  ifstream fin;
  fin.open("../dat/color.data");
  if(fin.fail())
  {
    std::cout << "-=-=-=-=-=-" << std::endl;
    return 1;
  }
  else
  {
    string input, substr;
    color tmp_color;
    while(getline(fin, input))
    {
      sscanf(input.c_str(), "%lf,%lf,%lf,%lf", &tmp_color.scalar,
             &tmp_color.R, &tmp_color.G, &tmp_color.B);
      color_list.push_back(tmp_color);
      init_scalar_.push_back(tmp_color.scalar);
      input.clear();
    }
    fin.close();
  }
  return 0;
}

void load_color::generate_error_color(const vector<color> &reference_color,
                                      const vector<double> &tri_error,
                                      const int &value,
                                      const int &default_value,
                                      vector<color> &sub_error_color,
                                      vector<int> &index)
{
  double tmp_error;
  color tmp_color;
  sub_error_color.clear();
  for(size_t i = 0; i < tri_error.size(); ++i)
  {
    //tmp_error = tri_error[i] * EXPANSION * value / default_value;
    tmp_error = tri_error[i] * value / 100;
    if(is_zero(tmp_error))
      index.push_back(0);
    else if(tmp_error > 1.0)
    {
      index.push_back(reference_color.size() - 1);
      sub_error_color.push_back(reference_color[reference_color.size() - 1]);      
    }
    else
    {
      for(size_t j = 1; j < reference_color.size() - 1; ++j)
      {
        if(fabs(tmp_error - reference_color[j].scalar) < COLOR_ACCURACY)
        {
          index.push_back(j);
          tmp_color.scalar = reference_color[j].scalar;
          tmp_color.R = reference_color[j].R;
          tmp_color.G = reference_color[j].G;
          tmp_color.B = reference_color[j].B;
          sub_error_color.push_back(tmp_color);
          break;
        }
        
      }//end of if
    }// end of else
  }//end of for
}

//generate error color by map

void load_color::generate_error_color(vector<color> &reference_color,
                                      vector<double> &tri_error,
                                      const size_t flag, const double scalar,
                                      vector<int> &index) 
{
  cout << "=====================================================================" << endl << endl;
  cout << "[#info-load-color]Begin load error color." << endl << endl;
  cout << "=====================================================================" << endl << endl;
  clock_t start, end;
  size_t cnt = 0;
  start = clock();
  //ofstream fout("color_error.txt");
  double max_error = tri_error[tri_error.size() - 1];

  std::cout << "[#info-load-color]pre_scalar : " << pre_scalar_ << std::endl;
  if(!equal(pre_scalar_, scalar))
  {
    pre_scalar_ = scalar;
    scale_color_list(scalar, 0, reference_color);
  }
  if(flag == Uniform)
  {
    tri_error[tri_error.size() - 1] = scalar;
  }
  size_t cnt_max = 0;
  size_t cnt_othre_error = 0;
  index.resize(tri_error.size());
  cout << "[#info-openmp]Thread number in load color generate_error_color function is : " <<
          thread_num(tri_error.size(), MIN_ITERATOR_NUM) << endl;
#pragma omp parallel for num_threads(thread_num(tri_error.size(), MIN_ITERATOR_NUM))
  for(int i = 0; i < tri_error.size() - 1; ++i)
  {
    double tmp_error;
    tmp_error = tri_error[i];
    if(flag == Independent)
    {
      if(!is_zero(max_error))
        tmp_error /= max_error;
    }
   // cout << tmp_error << endl;
    if(is_zero(tmp_error))
    {
      index[i] = 0;
      continue;
    }
    else if(is_larger_equal(tmp_error, reference_color[reference_color.size() - 1].scalar))
    {
      index[i] = reference_color.size() - 1;
      cnt++;
      cnt_max++;
    }
    else
    {
      cnt++;
      for(size_t j = 1; j < reference_color.size(); ++j)
      {
        if(is_larger(fabs(tmp_error), reference_color[j - 1].scalar) &&
           is_less_equal(fabs(tmp_error), reference_color[j].scalar))
        {
          index[i] = j;
          cnt_othre_error++;
          break;
        }
      }
    }
  }
  end = clock();
  std::cout << "[#info-color]---scalar type is : " << flag << std::endl;
  std::cout << "[#info-color]---scalar value is : " << scalar << std::endl;
//  std::cout << "[#info-load-color]---cnt_max = " << cnt_max << std::endl;
//  std::cout << "[#info-load-color]---cnt_othere_error = " << cnt_othre_error << std::endl;
  std::cout << "[#info-load-color]---index number " << index.size() << std::endl;
  cout << "[#info-load-color]---The cost time is : " << setiosflags(ios::fixed) <<
          setprecision(6) << difftime(end, start) / CLOCKS_PER_SEC << " seconds." << endl;
}

void load_color::update_color_list(const vector<color> &color_list,
                                   const vector<color> &sub_error_color,
                                   vector<color> &change_color_list,
                                   const int &value,
                                   const int &reference)
{
  double tmp_error;
  color tmp_color;
  change_color_list.clear();
  for(size_t i = 0; i < color_list.size(); ++i)
  {
    tmp_error = double(color_list[i].scalar * value / reference);
    for(size_t j = 0; j < color_list.size(); ++j)
    {
      if(fabs(tmp_error - color_list[j].scalar) < COLOR_ACCURACY)
      {
        tmp_color.scalar = color_list[j].scalar;
        tmp_color.R = color_list[j].R;
        tmp_color.G = color_list[j].G;
        tmp_color.B = color_list[j].B;
        change_color_list.push_back(tmp_color);
        break;
      }
    }
   
  }
  change_color_list.insert(change_color_list.end(), sub_error_color.begin(),
                           sub_error_color.end());
  sort(change_color_list.begin(), change_color_list.end(), cmp);
  change_color_list.erase(unique(change_color_list.begin(),
                                 change_color_list.end(), is_equal),
                          change_color_list.end());
}

void load_color::scale_color_list(double max_error, double min_error,
                                  std::vector<color> &color_list)
{
  double len = max_error - min_error;
#pragma omp parallel for num_threads(thread_num(color_list.size(), MIN_ITERATOR_NUM))
  for(int i = 0; i < color_list.size(); ++i)
  {
    color_list[i].scalar = init_scalar_[i] * len;
    //std::cout << color_list[i].scalar << std::endl;
  }
}

}
