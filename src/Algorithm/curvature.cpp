#include "curvature.h"
#include <iomanip>
#include <iostream>
namespace lq
{

using namespace std;


//the static member variable, must be defined.
bool curvature::is_cal_tri_info_ = false;
std::vector<vertex_adj_tri> curvature::adj_tri_list1;
std::vector<vertex_adj_tri> curvature::adj_tri_list2;
std::vector<triangle_info> curvature::tri_info_list1;
std::vector<triangle_info> curvature::tri_info_list2;

void curvature::cal_mean_curvature(const std::vector<point> &vertex_list,
                                   const std::vector<triangle> &tri_list,
                                   std::vector<double> &mean_cur)
{
    if(!is_cal_tri_info_)
    {
      initialize();
      get_vertex_adj_tri(tri_list, vertex_list, adj_tri_list1);
      get_tri_info(tri_list, vertex_list, tri_info_list1);
      is_cal_tri_info_ = true;
    }
    std::vector<double> tri_curvature;
    double curvature;
    double sum_area = 0.0;
    double sum_area2 = 0.0;
    cout << "[#info]tri_info number:" << tri_info_list1.size() << endl;
    for(size_t i = 0; i < vertex_list.size(); ++i)
    {
      vec cur_vec(0, 0, 0);
      cal_mean_curvature(i, adj_tri_list1[i], tri_info_list1, vertex_list, cur_vec, sum_area);
      mean_cur.push_back(vec_norm(cur_vec) / 2);
    }

    for(size_t i = 0; i < tri_list.size(); ++i)
    {
      double sum_cur = 0.0;
      for(size_t j = 0; j < 3; ++j)
      {
        sum_cur += mean_cur[tri_list[i].vertex[j] - 1];
      }
      tri_curvature.push_back(sum_cur);
    }
    output_curvature_vtk_file(vertex_list, tri_list, tri_curvature);
    ofstream fout;
    fout.open("mean_curvature.txt");
    double mean_mean_cur = 0.0f;
    size_t cnt = 0;
    for(size_t i = 0; i < mean_cur.size(); ++i)
    {
      if(is_less(mean_cur[i], 5))
      {
        mean_mean_cur += mean_cur[i] ;
        cnt++;
      }
      fout << mean_cur[i] << endl;
    }
    cout << "[#info]mean mean curvature is : " << setiosflags(ios::fixed) << setprecision(8) <<
            mean_mean_cur / cnt <<endl;
    fout.close();
}
void curvature::cal_mean_curvature(const std::vector<point> &vertex_list,
                                   const std::vector<triangle> &tri_list,
                                   const std::vector<vertex_adj_tri> adj_tri_list,
                                   const std::vector<triangle_info> tri_info_list,
                                   std::vector<double> &mean_cur)
{
  std::vector<double> tri_curvature;
  double curvature;
  double sum_area = 0.0;
  cout << "[#info-algorithm]---Cal_mean_curvature " << endl;
  mean_cur.resize(vertex_list.size());
//#pragma omp parallel num_threads(thread_num(vertex_list.size(), MIN_ITERATOR_NUM))
  for(size_t i = 0; i < vertex_list.size(); ++i)
  {
    vec cur_vec(0, 0, 0);
    cal_mean_curvature(i, adj_tri_list[i], tri_info_list, vertex_list, cur_vec, sum_area);
    mean_cur[i] = (vec_norm(cur_vec) / 2);
  }
}

void curvature::cal_gaussian_curvature(const std::vector<point> &vertex_list,
                                       const std::vector<triangle> &tri_list,
                                       std::vector<double> &gauss_cur_list)
{
  vector<double> tri_curvature;
  double sum_gaussian = 0.0f;
  if(!is_cal_tri_info_)
  {
    initialize();
    get_vertex_adj_tri(tri_list, vertex_list, adj_tri_list1);
    get_tri_info(tri_list, vertex_list, tri_info_list1);
    is_cal_tri_info_ = true;
  }
  for(size_t i = 0; i < vertex_list.size(); ++i)
  {
    double gaussian = 0.0;
    curvature::cal_gaussian_curvature(i, adj_tri_list1[i], tri_info_list1,
                                      gaussian, sum_gaussian);
    gauss_cur_list.push_back(gaussian);
  }
  for(size_t i = 0; i < tri_list.size(); ++i)
  {
    double sum_cur = 0.0;
    for(size_t j = 0; j < 3; ++j)
    {
      sum_cur += gauss_cur_list[tri_list[i].vertex[j] - 1];
    }
    tri_curvature.push_back(sum_cur);
  }

  output_curvature_vtk_file(vertex_list, tri_list, tri_curvature);
  ofstream fout;
  fout.open("gaussian_curvature.txt");
  double each_gaussian = 0.0f;
  size_t cnt = 0;
  for(size_t i = 0; i < gauss_cur_list.size(); ++i)
  {

    if(is_less_equal(gauss_cur_list[i], 30))
    {
      each_gaussian += gauss_cur_list[i];
      cnt++;
    }
    fout << gauss_cur_list[i] << endl;

  }
  cout << "[#info]Mean gaussian curvature on each point : " <<
          setiosflags(ios::fixed) << setprecision(8) <<
          each_gaussian / cnt << endl;
  cout << "[#info]sum gaussian curvature : " << setiosflags(ios::fixed) << setprecision(8) <<
           sum_gaussian << endl;

  fout.close();
}

void curvature::cal_gaussian_curvature(const std::vector<point> &vertex_list,
                                       const std::vector<triangle> &tri_list,
                                       const std::vector<vertex_adj_tri> adj_tri_list,
                                       const std::vector<triangle_info> tri_info_list,
                                       std::vector<double> &gauss_cur_list)
{
  double sum_gaussian = 0.0f;
  gauss_cur_list.resize(vertex_list.size());
  cout << "[#info-curvature-algorithm]---Cal_gaussian_curvature " << endl;
//#pragma omp parallel num_threads(thread_num(vertex_list.size(), MIN_ITERATOR_NUM))
  for(size_t i = 0; i < vertex_list.size(); ++i)
  {
    double gaussian = 0.0;
    curvature::cal_gaussian_curvature(i, adj_tri_list[i], tri_info_list,
                                      gaussian, sum_gaussian);
    gauss_cur_list[i] = gaussian;
  }
}


void curvature::cal_principal_curvature(const std::vector<point> &vertex_list,
                                        const std::vector<triangle> &tri_list,
                                        std::vector<principal_curvature_info> &prin_cur_list)
{
  if(!is_cal_tri_info_)
  {
    initialize();
    get_vertex_adj_tri(tri_list, vertex_list, adj_tri_list1);
    get_tri_info(tri_list, vertex_list, tri_info_list1);
    is_cal_tri_info_ = true;
  }
  vector<double> mean_cur;
  vector<double> gauss_cur;
  cal_mean_curvature(vertex_list, tri_list, mean_cur);
  cal_gaussian_curvature(vertex_list, tri_list, gauss_cur);
  principal_curvature_info tmp_cur;
  double delta_x = 0.0f;
  vector<double> k1;

  for(size_t i = 0; i < vertex_list.size(); ++i)
  {
    delta_x = mean_cur[i] * mean_cur[i] - gauss_cur[i];
//    cout << "mean_cur : " << mean_cur[i] << " gauss_cur : " << gauss_cur[i] <<
//             " delta_x " << endl;
    if(is_less(delta_x, 0.0))
    {
      delta_x = 0.0;
    }
    tmp_cur.k1 = mean_cur[i] + sqrt(delta_x);

    tmp_cur.k2 = mean_cur[i] - sqrt(delta_x);
    k1.push_back(tmp_cur.k1);
    prin_cur_list.push_back(tmp_cur);
  }
  vector<double> tri_curvature;
  for(size_t i = 0; i < tri_list.size(); ++i)
  {
    double sum_cur = 0.0;
    for(size_t j = 0; j < 3; ++j)
    {
      sum_cur += k1[tri_list[i].vertex[j] - 1];
    }
    tri_curvature.push_back(sum_cur);
  }
  output_curvature_vtk_file(vertex_list, tri_list, tri_curvature);
  ofstream fout;
  fout.open("principal_curvature.txt");
  double sum_k1 = 0.0f;
  double sum_k2 = 0.0f;
  size_t cnt = 0;
  for(size_t i = 0; i < prin_cur_list.size(); ++i)
  {
    if(is_less(prin_cur_list[i].k1, 2.0))
    {
      sum_k1 += prin_cur_list[i].k1;
      sum_k2 += prin_cur_list[i].k2;
      cnt++;
    }
    fout << prin_cur_list[i].k1 << "  " << prin_cur_list[i].k2 << endl;
  }
  cout << "[#info]mean k1 : " << setiosflags(ios::fixed) << setprecision(8) <<
          sum_k1 / cnt << " k2 : " <<
          sum_k2 / cnt << endl;
  fout.close();
}



void curvature::cal_tri_angle(const double edge_len[3], double angle[3],
                              size_t &tri_type)
{
  bool flag1 = false;
  bool flag2 = false;
  double cos_theta;
  double side1, side2, subtense;
  for(size_t i = 0; i < 3; ++i)
  {
    subtense = edge_len[i];
    side1 = edge_len[(i + 1) % 3];
    side2 = edge_len[(i + 2) % 3];
    cos_theta = (side1 * side1 + side2 * side2 - subtense * subtense) / (2 * side1 * side2);
    angle[i] = acos(cos_theta);

    if(!flag1 && equal(angle[i], RIGHT_ANGLE))
    {
      tri_type = RIGHT;
      flag1 = true;
    }
    else if(!flag2 && is_larger(angle[i], RIGHT_ANGLE))
    {
      tri_type = OBTUSE;
      flag2 = true;
    }
  }

}


//void curvature::initialize()
//{
  
//}

//void curvature::reset()
//{
//  initialize();
//}

/**
 *@brief compute all the triangle adjacent with the vertex
 *
 *@param tri_list  the triangle set of the model
 *@param vertex_list  the vertex set of model
 *@param adj_tri_list  store the result
 *
 */

void curvature::get_vertex_adj_tri(const vector<triangle> &tri_list,
                                   const vector<point> &vertex_list,
                                   vector<vertex_adj_tri> &adj_tri_list)
{
  adj_tri_list.resize(vertex_list.size());
  
//#pragma omp parallel for num_threads(thread_num(tri_list.size(), MIN_ITERATOR_NUM))
  for(size_t i = 0; i < tri_list.size(); ++i)
  {
    for(size_t j = 0; j < 3; ++j)
    {
      size_t index;
      index = tri_list[i].vertex[j] - 1;
      adj_tri_list[index].adj_tri.push_back(i);
    }
  }
}

void curvature::get_tri_info(const vector<triangle> &tri_list,
                             const vector<point> &vertex_list,
                             vector<triangle_info> &tri_info)
{

  point p1, p2;
  double index[3];
  double edge_len[3];
  double angle[3];
  size_t tri_type;
  bool flag1, flag2;
  for(size_t i = 0; i < tri_list.size(); ++i)
  {
    triangle_info tmp_tri_info;
    // flag1 = false;
    //flag2 = false;
    tri_type = ACUTE;
    for(size_t j = 0; j < 3; ++j)
      index[j] = tri_list[i].vertex[j] - 1;
    //compute length of subtense
    for(size_t j = 0; j < 3; ++j)
    {
      p1 = vertex_list[index[(j + 1) % 3]];
      p2 = vertex_list[index[(j + 2) % 3]];
      edge_len[j] = vec_norm(p1 - p2);
    }
    cal_tri_angle(edge_len, angle, tri_type);
    double circumcircle_r;
    // cal_circumcircle_radius(edge_len, circumcircle_r);
    for(size_t j = 0; j < 3; ++j)
    {
      vertex_info tmp_vertex_info;
      tmp_vertex_info.index = index[j];
      tmp_vertex_info.angle = angle[j];
      tmp_vertex_info.subtense = edge_len[j];
      tmp_tri_info.info.insert(pair<size_t, vertex_info>(index[j], tmp_vertex_info));
    }
    tmp_tri_info.type = tri_type;
    tmp_tri_info.area = (edge_len[1] * edge_len[0] * sin(angle[2]))/ 2; // a*b*sin(C) / 2
    tri_info.push_back(tmp_tri_info);
  }
}

/*******************************************************************************
 *@brief calculate the voronoi area for tri_list[index], the result add to area
 *
 *@param tri_info the information of triangle(tri_list[index]) 
 *@param area  the A_mixed  
 *
 *@return void
 ******************************************************************************/
void curvature::cal_voronoi_area(const triangle_info &tri_info,
                                 size_t index, double &area)
{
  map<size_t, vertex_info>::const_iterator iter;
  if(tri_info.type == OBTUSE)
  {
    iter = tri_info.info.find(index);
    assert(iter != tri_info.info.end());
    if(is_larger((*iter).second.angle, RIGHT_ANGLE))
      area += tri_info.area / 2;
    else
      area += tri_info.area / 4;
  }
  else
  {
    for(iter = tri_info.info.begin(); iter != tri_info.info.end(); ++iter)
    {

      if((*iter).first != index && !equal((*iter).second.angle, RIGHT_ANGLE))
      {
        area += (1 / tan((*iter).second.angle)) * (*iter).second.subtense *
                (*iter).second.subtense / 8;
      }
    }
  }

}

void curvature::cal_single_curve(const triangle_info &info_tri, const size_t index,
                                 const vector<point> &vertex_list, vec &cur_vec)
{

  map<size_t, vertex_info>::const_iterator iter;
  map<size_t, vertex_info>::const_iterator index_iter;
  index_iter = info_tri.info.find(index);
  assert(index_iter != info_tri.info.end());
  double angle;
//   fout << "  --tri_vertex: " << endl;
//   fout << "    --index point: " << index + 1 << endl;
//   fout << "    --index_iter point: " << (*index_iter).first + 1 << endl;
//   for(iter = info_tri.info.begin(); iter != info_tri.info.end(); ++iter)
//   {
//     fout << "     --:" << (*iter).first + 1<< "  angle: " <<  (*iter).second.angle * 180 / PI
//          << " subtense: " << (*iter).second.subtense << endl;
//   }
  for(iter = info_tri.info.begin(); iter != info_tri.info.end(); ++iter)
  {
    if((*iter).first != index)
    {
      angle = PI - (*iter).second.angle - (*index_iter).second.angle;
      assert(!is_zero(angle));
      if(!equal(angle, RIGHT_ANGLE) && !equal(angle, 0))
      {
//         fout << "      --point index: " << (*iter).first + 1 << endl;
//         vec tmp = (vertex_list[index]- vertex_list[(*iter).first]);
        cur_vec = cur_vec + (vertex_list[index] - vertex_list[(*iter).first]) *
                  (1 / tan(angle));
//         tmp = tmp * (1/ tan(angle));
//         fout << "      --vec : " <<  setiosflags(ios::fixed) << setprecision(16) <<  tmp.x << " " << tmp.y << " " <<  tmp.z << endl;
//         fout << "      --cot_angle :" << 1 / tan(PI - (*iter).second.angle - (*index_iter).second.angle) << endl;
//         fout << "      --cur_vec: " << cur_vec.x << " " << cur_vec.y << " " << cur_vec.z << endl;
      }
    }
   }
}

void curvature::sum_adj_tri_angle(const triangle_info &info_tri, const size_t index,
                                  double &sum_angle)
{
  map<size_t, vertex_info>::const_iterator iter;
  iter = info_tri.info.find(index);
  sum_angle += (*iter).second.angle;
}


void curvature::cal_mean_curvature(const size_t vertex_index,
                                   const vertex_adj_tri &info_vertex,
                                   const vector<triangle_info> &info_tri,
                                   const vector<point> &vertex_list, vec &cur_normal,
                                   double &sum_area)
{
  size_t flag;
  //double tmp_area;
  double area = 0.0;
  vec cur_vec(0, 0, 0);
//  std::ofstream fout2;
//  fout2.open("cur_vec", fstream::out | fstream::app);
//  std::ofstream fout_area;
//  fout_area.open("area.txt", fstream::out | fstream::app);
//  fout_area << "vertex index:" << vertex_index + 1 << endl;
//  fout2 << "vertex_index: " << vertex_index + 1 << endl;
  for(size_t i = 0; i < info_vertex.adj_tri.size(); ++i)
  {
    flag = info_vertex.adj_tri[i];
    cal_voronoi_area(info_tri[flag], vertex_index, area);
    cal_single_curve(info_tri[flag], vertex_index, vertex_list, cur_vec);
  }
  //cout << vertex_index << endl;
  //assert(!equal(area, 0));
  cur_normal = cur_vec / (2 * area);
  sum_area += area;
}

void curvature::cal_gaussian_curvature(size_t vertex_index,
                                       const vertex_adj_tri &info_vertex,
                                       const std::vector<triangle_info> &info_tri,
                                       double &gaussian, double &sum_gaussian)
{
  double area_mixed = 0.0;
  double sum_angle = 0.0;
  for(size_t i = 0; i < info_vertex.adj_tri.size(); ++i)
  {
    size_t flag = info_vertex.adj_tri[i];
    cal_voronoi_area(info_tri[flag], vertex_index, area_mixed);
    sum_adj_tri_angle(info_tri[flag], vertex_index, sum_angle);
  }
  if(equal(area_mixed, 0.0))
    cout << vertex_index << endl;
  if(is_larger_equal(sum_angle, 2 * PI))
  {
    gaussian = 0.0f;
  }
  else
  {
    sum_gaussian += 2 * PI - sum_angle;
    gaussian = (2 * PI - sum_angle) / area_mixed;
  }
//  if(fabs(gaussian - 0.0) < 1e-12)
//    gaussian = 0.0f;
}

void curvature::output_curvature_vtk_file(const std::vector<point> &vertex_list,
                                          const std::vector<triangle> &tri_list,
                                          const std::vector<double> &tri_curvature)
{
  ofstream fout;
  fout.open("mean_curvature.vtk");
  fout << "# vtk DataFile Version 2.0" << endl;
  fout << "mean_curvature" << endl;
  fout << "ASCII" << endl;
  fout << "DATASET UNSTRUCTURED_GRID" << endl;
  fout << "POINTS " << vertex_list.size() << " float" << endl;
  for(size_t i = 0; i < vertex_list.size(); ++i)
  {
    fout << vertex_list[i].x << " " << vertex_list[i].y << " " <<
            vertex_list[i].z << endl;
  }
  fout << "CELLS " << tri_list.size() << " " << tri_list.size() * 4 << endl;
  for(size_t i = 0; i < tri_list.size(); ++i)
  {
    fout << "3 " << tri_list[i].vertex[0] - 1<< " " << tri_list[i].vertex[1] - 1<<
            " " << tri_list[i].vertex[2] - 1<< endl;
  }
  fout << "CELL_TYPES " << tri_list.size() << endl;
  for(size_t i = 0; i < tri_list.size(); ++i)
  {
    fout << "5" << endl;
  }
  fout << "CELL_DATA " << tri_list.size() << endl;
  fout << "SCALARS mean_curvature float" << endl;
  fout << "LOOKUP_TABLE my_table" << endl;
  for(size_t i = 0; i < tri_list.size(); ++i)
  {
    fout << tri_curvature[i] <<endl;
  }
}

};
