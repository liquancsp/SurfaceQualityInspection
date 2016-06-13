#ifndef CURVATURE_H
#define CURVATURE_H

#include "data_type.h"
#include "algorithm_base.h"

namespace lq
{

class curvature : public algorithm_base
{

public:

  curvature()
  {}
  ~curvature()
  {}
  void cal_mean_curvature(const std::vector<point> &vertex_list,
                          const std::vector<triangle> &tri_list,
                          std::vector<double> &mean_cur);
  
  void cal_mean_curvature(const std::vector<point> &vertex_list,
                          const std::vector<triangle> &tri_list,
                          const std::vector<vertex_adj_tri> adj_tri_list,
                          const std::vector<triangle_info> tri_info_list,
                          std::vector<double> &mean_cur);
  
  void cal_gaussian_curvature(const std::vector<point> &vertex_list,
                              const std::vector<triangle> &tri_list,
                              std::vector<double> &gauss_cur_list);
  
  void cal_gaussian_curvature(const std::vector<point> &vertex_list,
                              const std::vector<triangle> &tri_list,
                              const std::vector<vertex_adj_tri> adj_tri_list,
                              const std::vector<triangle_info> tri_info_list,
                              std::vector<double> &gauss_cur_list);

  void cal_principal_curvature(const std::vector<point> &vertex_list,
                               const std::vector<triangle> &tri_list,
                               std::vector<principal_curvature_info> &prin_cur_list);
  
  
 
  static void initialize()
  {
    adj_tri_list1.clear();
    adj_tri_list2.clear();
    tri_info_list1.clear();
    tri_info_list2.clear();
  }

  static void reset()
  {
    initialize();
  }

protected:

  void cal_tri_angle(const double edge_len[3], double angle[3],
                     size_t &tri_type);
  void get_vertex_adj_tri(const std::vector<triangle> &tri_list,
                          const std::vector<point> &vertex_list,
                          std::vector<vertex_adj_tri> &adj_tri_list);
  
  void get_tri_info(const std::vector<triangle> &tri_list,
                    const std::vector<point> &vertex_list,
                    std::vector<triangle_info> &tri_info);

  void cal_voronoi_area(const triangle_info &tri_info, size_t index, double &area);

  void cal_single_curve(const triangle_info &info_tri, const size_t index,
                        const std::vector<point> &vertex_list, vec &cur_vec);

  void sum_adj_tri_angle(const triangle_info &info_tri, const size_t index,
                         double &sum_angle);

  void cal_mean_curvature(const size_t vertex_index,
                          const vertex_adj_tri &info_vertex,
                          const std::vector<triangle_info> &info_tri,
                          const std::vector<point> &vertex_list, vec &cur_normal,
                          double &sum_area);

  void cal_gaussian_curvature(size_t vertex_index,
                              const vertex_adj_tri &info_vertex,
                              const std::vector<triangle_info> &info_tri,
                              double &gaussian, double &sum_gaussian);

  void output_curvature_vtk_file(const std::vector<point> &vertex_list,
                                 const std::vector<triangle> &tri_list,
                                 const std::vector<double> &tri_curvature);

  static std::vector<vertex_adj_tri> adj_tri_list1;
  static std::vector<vertex_adj_tri> adj_tri_list2;
  static std::vector<triangle_info> tri_info_list1;
  static std::vector<triangle_info> tri_info_list2;
  static bool is_cal_tri_info_;
  
private:


};


};

#endif // CURVATURE_H
