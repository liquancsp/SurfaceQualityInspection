/************************************************************************************
 *COPYRIGHT NOTICE  
 *Copyright (c) 2012, Zhe Jiang University 
 *All rights reserved.  
 *
 *@file reflection_map.cpp
 *@author li quan
 *@version 1.0
 *@date 2012-11
 *@ingroup diff
 *@brief
 *
 *@history
 *************************************************************************************/

#include "reflection_map.h"
#include "zjucad/matrix/io.h"
#include "out_vtk.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <string>

namespace lq {
using namespace std;
//const double PI = 3.1415926;
reflection_map::reflection_map()
{
  out.open("operation_log.txt", fstream::out | fstream::app);
}


reflection_map::~reflection_map()
{
  out.close();
}

//Randomly generate view point 
void reflection_map::generate_view(std::vector<point> &view_point, const int &num)
{
  double ran_num;
  point tmp;
  srand((unsigned)time(0));
  for(size_t i = 0; i < num; ++i)
  {
    tmp.x = rand() / ((double)(RAND_MAX) - 0.5) * 10;
    tmp.y = rand() / ((double)(RAND_MAX) - 0.5) * 10;
    tmp.z = rand() / ((double)(RAND_MAX) - 0.5) * 10;
    view_point.push_back(tmp);
  }
}

//copy point to matrix
void reflection_map::copy_point(zjucad::matrix::matrix<double> &mat,
                                const point &p)
{
  mat = zjucad::matrix::zeros(3, 1);
  mat(0, 0) = p.x;
  mat(1, 0) = p.y;
  mat(2, 0) = p.z;
  //std::cout << " mat " << mat << std::endl;
  return;
}

//copy normal to matrix
void reflection_map::copy_normal(zjucad::matrix::matrix<double> &normal,
                                 const vec &vn)
{
  normal = zjucad::matrix::zeros(3, 1);
  normal(0, 0) = vn.x;
  normal(1, 0) = vn.y;
  normal(2, 0) = vn.z;
  //std::cout << "normal " << normal << std::endl;
}

//copy matrix to point
void reflection_map::copy_matrix(const zjucad::matrix::matrix<double> &vertex,
                                 point &p)
{
  p.x = vertex(0, 0);
  p.y = vertex(1, 0);
  p.z = vertex(2, 0);
}

/*******************************************************************************
 *@brief compute the symmetry position relative to a plane(v − 2nnT (v − r))
 *
 *@param view_point  the position of viewpoint
 *@param normal  the normal of the plane
 *@param vertex  a point on the plane
 *@param symmetry_pos  the symmetry position of vertex relative to the plane
 *
 *@see copy_point() , copy_normal()
 *
 *@return void
 ******************************************************************************/
void reflection_map::cal_symmetry_pos(const point &view_point,
                                      const vec &normal,
                                      const point &vertex, point &symmetry_pos)
{
  // std::cout << "view_point :" << view_point.x << " " << view_point.y << " "
  //           << view_point.z << std::endl;
  matrixd mat_view, mat_normal, mat_trans_normal, mat_sym_pos, mat_vertex;
  copy_point(mat_view, view_point);
  // std::cout << "mat_view " << mat_view << std::endl;
  copy_point(mat_vertex, vertex);
  copy_normal(mat_normal, normal);
  mat_trans_normal = trans(mat_normal);
  // std::cout << "trans normal " << mat_trans_normal << std::endl;
  // std::cout << "2*n*t " << 2 * mat_normal * mat_trans_normal<< std::endl;
  // std::cout << "diff" << (mat_view - mat_vertex) << std::endl;
  mat_sym_pos = mat_view - 2 * mat_normal * mat_trans_normal
      * (mat_view - mat_vertex);
  //  std::cout << "sym_pos " << mat_sym_pos << std::endl;
  copy_matrix(mat_sym_pos, symmetry_pos);
  return ;
}

//compute 
/**
 *@brief compute the reflected light direction (q(r) = (r - v'(r)) / (||r - v'(r)||))
 *
 *@param view_point  the position of viewpoint
 *@param normal the normal of the palne
 *@param vertex a point on the plane
 *@param reflect_vertex  store the direction vector of reflection
 *
 *@see cal_symmetry_pos()
 *
 */

void reflection_map::cal_reflect_map(const point &view_point,
                                     const vec &normal,
                                     const point &vertex, point &reflect_vertex)
{
  point symmetry_pos;
  double tmp;
  cal_symmetry_pos(view_point, normal, vertex, symmetry_pos);
  reflect_vertex = (vertex - symmetry_pos) / vec_norm(vertex - symmetry_pos);
  return ;
}

void reflection_map::get_reflection_vector(const point &view_point, const point &p,
                                           const vec &normal, vec &refl_vec)
{
  vec in_vec(p - view_point);
  in_vec = in_vec / vec_norm(in_vec);
  vec tmp(normal);
  tmp = tmp / vec_norm(tmp);
  refl_vec = in_vec - 2 * dot_mult(in_vec, tmp) * tmp;
}

/**
 *@brief get the value of the different of two model
 *d(q, q , r) = ||q(r) − q (m(r))||^2   
 *
 *@param vertex1  the point on first model
 *@param vertex2  the point on second model
 *@param normal1  the normal of first model
 *@param normal2  the normal of second model
 *@param view_point  the position of viewpoint
 *@param diff  return the different store in diff
 *
 *@see cal_reflect_map()
 *
 */
 
void reflection_map::get_diff(const point &vertex1, const vec &normal1,
                              const point &vertex2, const vec &normal2,
                              const point &view_point, double &diff)
{
  point reflect_map1, reflect_map2;
  if(equal(normal1, normal2))
  {
    diff = 0.0;
    return ;
  }
//  get_reflection_vector(view_point, vertex1, normal1, reflect_map1);
//  get_reflection_vector(view_point, vertex2, normal2, reflect_map2);
  cal_reflect_map(view_point, normal1, vertex1, reflect_map1);
  cal_reflect_map(view_point, normal2, vertex2, reflect_map2);
  diff = vec_norm(reflect_map1 - reflect_map2);
}

//output the diff and index of each vertex to the file 
int reflection_map::output_diff(const std::vector<point> &vertex_list1,
                                const std::vector<point> &vertex_list2,
                                const std::vector<vec> &normal_list1,
                                const std::vector<vec> &normal_list2,
                                const point &view_point)
{
  std::ofstream fout("diff.txt");
  if(fout.fail())
  {
    std::cerr << "fail to open diff.txt" << std::endl;
    std::cerr << "file:" << __FILE__ << std::endl;
    std::cerr << "line:" << __LINE__ << std::endl;
    return 1;
  }
  else
    std::cout << "diff.txt open successful." << std::endl;
  double diff;
  for(size_t i = 0; i < vertex_list1.size(); ++i)
  {
    get_diff(vertex_list1[i], normal_list1[i], vertex_list2[i],
             normal_list2[i], view_point, diff);
    if(!is_zero(diff))
      fout << diff << " " << i + 1 << std::endl;
  }
  fout.close();
  return 0;
}

int reflection_map::output_diff(const std::vector<point> &vertex_list1,
                                const std::vector<point> &vertex_list2,
                                const std::vector<vec> &normal_list1,
                                const std::vector<vec> &normal_list2,
                                const std::vector<point> &view_point)
{

  std::ofstream fout("sum_diff.txt");
  if(fout.fail())
  {
    std::cerr << "fail to open sum_diff.txt" << std::endl;
    std::cerr << "file:" << __FILE__ << std::endl;
    std::cerr << "line:" << __LINE__ << std::endl;
    return 1;
  }
  else
    std::cout << "sum_diff.txt open successful." << std::endl;
  for(size_t i = 0; i < vertex_list1.size(); ++i)
  {
    double diff = 0.0, sum_diff = 0.0;
    for(size_t j = 0; j < view_point.size(); ++j)
    {
      get_diff(vertex_list1[i], normal_list1[i], vertex_list2[i],
               normal_list2[i], view_point[j], diff);
      sum_diff += diff;
    }
    if(!is_zero(diff))
      fout << diff << " " << i + 1 << std::endl;
  }
  fout.close();
  return 0;
}


/**
 *@brief this function is compute the different of two triangle by comparing the
 *sum different of the triangle which summation the three vertices' error of the triangle.
 *
 *@param vertex_list1 the vertices of first model
 *@param vertex_list2 the vertices of second model
 *@param tri_list1 the triangle info of the first model
 *@param tri_list2 the triangle info of the second model
 *@param tri_error store the result of different of two model
 *@param vertex_error store the error of each point 
 *@param view_point the viewpoint
 *
 *@see get_diff()

 *@note this function compute the sum different of two mesh by summation of three vertices of 
 *each triangle
 */

void reflection_map::cal_tri_diff(const std::vector<point> &vertex_list1,
                                  const std::vector<point> &vertex_list2,
                                  const std::vector<vec> &normal_list1,
                                  const std::vector<vec> &normal_list2,
                                  const std::vector<triangle> &tri_list1,
                                  const std::vector<triangle> &tri_list2,
                                  std::vector<double> &tri_error,
                                  std::vector<double> &vertex_error,
                                  const point &view_point)
{
  out << "===================================" << endl;
  out << "Begin compute the different of two model." << endl;
  out << "The view point is : " << endl << " " << view_point.x << " " <<
      view_point.y << " " << view_point.z << endl;
  clock_t start, end;
  double diff;
  double max_error = 0.0;
  //std::ofstream fout("out_diff.txt");
  start = clock();
  for(size_t i = 0; i < vertex_list1.size(); ++i)
  {
    diff = 0.0;
    get_diff(vertex_list1[i], normal_list1[i], vertex_list2[i],
             normal_list2[i], view_point, diff);
    if(!is_zero(diff))
      vertex_error.push_back(diff);
    else
      vertex_error.push_back(0.0);
  }
  for(size_t i = 0; i < tri_list1.size(); ++i)
  {
    double sum_diff = 0.0;
    for(size_t j = 0; j < 3; ++j)
      sum_diff += vertex_error[tri_list1[i].vertex[j] - 1];
    if(is_larger(sum_diff, max_error))
      max_error = sum_diff;
    if(!is_zero(sum_diff))
      tri_error.push_back(sum_diff);
    else
      tri_error.push_back(0.0);
  }
  tri_error.push_back(max_error);
//  for(size_t i = 0; i < tri_error.size(); ++i)
//    tri_error[i] /= max_error;
  
  end = clock();
  
  out << "The maximum error is : " << setiosflags(ios::fixed) << setprecision(8) <<
      max_error <<  "  convert angle is " <<  360 *  asin(max_error / 2) / PI << endl;
  out << "The cost time of compute the different of two model is : " <<
      setiosflags(ios::fixed) << setprecision(5) <<
      difftime(end, start) / CLOCKS_PER_SEC << " seconds." << endl;
  // out.close();

  // for(size_t i = 0; i < tri_error.size(); ++i)
  // {
  //   if(equal(tri_error[i], 0.0))
  //     std::cout << i << std::endl;
  // }

  // for(size_t i = 0; i < tri_error.size(); ++i)
  // {
  //   if(equal(tri_error[i], 1.0))
  //     std::cout << "----" << std::endl;
  //   if(!is_zero(tri_error[i]))
  //     fout << tri_error[i] << " " << i + 1 << std::endl;
  // }
  // std::cout << "max_error " << max_error << std::endl;
  //fout.close();
 
}



/**
 *@brief this function is compute the different of two triangle by only compare
 *two triangle's normal.
 *
 *@param vertex_list1 the vertices of first model
 *@param vertex_list2 the vertices of second model
 *@param tri_list1 the triangle info of the first model
 *@param tri_list2 the triangle info of the second model
 *@param tri_error store the result of different of two model
 *@param view_point the viewpoint
 *
 *@see get_diff()

 *@note this function compute the different of two mesh only by the different 
 *normal of two mesh
 */

void reflection_map::cal_tri_error(const std::vector<point> &vertex_list1,
                                   const std::vector<point> &vertex_list2,
                                   const std::vector<triangle> &tri_list1,
                                   const std::vector<triangle> &tri_list2,
                                   const point &view_point, const size_t flag,
                                   std::vector<double> &tri_error)
{
  cout << "=====================================================================" << endl << endl;
  cout << "[#info-reflection-algorithm]Begin to computer the different reflection map of two models." << endl << endl;
  cout << "=====================================================================" << endl << endl;
  cout << "[#info]---The view point is : " << endl << " " << view_point.x << " " <<
          view_point.y << " " << view_point.z << endl;
  clock_t start, end;
 
  
  double max_error = 0.0;
  start = clock();
  tri_error.resize(tri_list1.size());
  omp_lock_t lock;
  omp_init_lock(&lock);
  size_t i;
  cout << "[#info-openmp]Thread number in reflection is : " << 
          thread_num(tri_list1.size(), MIN_ITERATOR_NUM) << endl;
#pragma omp parallel for num_threads(thread_num(tri_list1.size(), \
  MIN_ITERATOR_NUM))
  for(int i = 0; i < tri_list1.size(); ++i)
  {
    double diff = 0.0f;
    
    if(!equal(tri_list1[i].triangle_normal, tri_list2[i].triangle_normal))
    {
      point center1, center2;
      get_center_point(vertex_list1, tri_list1[i], center1);
      get_center_point(vertex_list2, tri_list2[i], center2);
      get_diff(center1, tri_list1[i].triangle_normal,
               center2, tri_list2[i].triangle_normal,
               view_point, diff);
    }
//    omp_set_lock(&lock);
//    if(is_larger(diff, max_error))
//      max_error = diff;
//    omp_unset_lock(&lock);
    if(!is_zero(diff))
      tri_error[i] = diff;
    else
      tri_error[i] = 0.0;
  }
  max_error = 0.0f;
  for(size_t i = 0 ; i < tri_error.size(); ++i)
  {
    if(is_larger(tri_error[i], max_error))
      max_error = tri_error[i];
  }
  tri_error.push_back(max_error);
  end = clock();
  double angle;
  angle = 180 * max_error / PI;
  cout << "[#info-reflection-algorithm]---The maximum error is : " << 
          setiosflags(ios::fixed) << setprecision(8) << max_error <<  
          "  convert angle is " <<  360 *  asin(max_error / 2) / PI << endl;
  cout << "[#info-reflection-algorithm]---The cost time of compute the different of two model is : " <<
          setiosflags(ios::fixed) << setprecision(5) <<
          difftime(end, start) / CLOCKS_PER_SEC << " seconds." << endl;
  // out.close();
}

//output the .vtk file which can show the error by different color in paraview
void reflection_map::output_diff(const std::string &filename,
                                const std::vector<point> &vertex_list,
                                const std::vector<triangle> &tri_list,
                                const std::vector<double> &tri_error)
{
  
  if(out_vtk_file(filename, vertex_list, vertex_list.size(),
                  tri_list, tri_list.size(), tri_error) == 1)
    std::cout << "fail to output .vtk file." << std::endl;
  else
    std::cout << filename << " file have been exported." << std::endl;
}

//get intersection of reflection line and zebra plane
void reflection_map::get_zebra_map(const std::vector<triangle> &triangle_list,
                                   const std::vector<point> &vertex_list,
                                   const point &vec_plane,
                                   const point &vertex_plane,
                                   const point &view_point,
                                   std::vector<point> &cross_point)
{
  point symmetry_pos;
  point tri_center;
  point vec_line;
  point tmp_cross;
  for(size_t i = 0; i < triangle_list.size(); ++i)
  {
    cal_tri_center(triangle_list[i], vertex_list, tri_center);
    cal_symmetry_pos(view_point, triangle_list[i].triangle_normal,
                     tri_center, symmetry_pos);
    vec_line = (symmetry_pos - tri_center);
    line_plane_cross_point(vec_line, tri_center, vec_plane, vertex_plane, tmp_cross);
    cross_point.push_back(tmp_cross);
  }
  // std::ofstream fout;
  // fout.open("zebra_cross_point.txt");
  // for(size_t i = 0; i < cross_point.size(); ++i)
  // {
  //   fout << cross_point[i].x << " " << cross_point[i].y << " "
  //        << cross_point[i].z << std::endl;
  // }
  // fout.close();
}

//compute the cross point of line and plane
void reflection_map::line_plane_cross_point(const point &vec_line,
                                            const point &vertex_line,
                                            const point &vec_plane,
                                            const point &vertex_plane,
                                            point &cross_point)
{
  double t;
  point tmp(vertex_plane - vertex_line);
  t = (dot_mult(vec_plane, tmp)) / (dot_mult(vec_plane, vec_line));
  cross_point.x = vec_line.x * t + vertex_line.x;
  cross_point.y = vec_line.y * t + vertex_line.y;
  cross_point.z = vec_line.z * t + vertex_line.z;
  
}    
    
};
