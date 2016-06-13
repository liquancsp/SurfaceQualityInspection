#ifndef GAUSSIAN_CURVATURE_H
#define GAUSSIAN_CURVATURE_H

#include "curvature.h"

namespace lq
{

class gaussian_curvature : public curvature
{

public:
  gaussian_curvature();

  ~gaussian_curvature()
  {}

  void cal_tri_error(const std::vector<point> &vertex_list1,
                     const std::vector<point> &vertex_list2,
                     const std::vector<triangle> &tri_list1,
                     const std::vector<triangle> &tri_list2,
                     const point &view_point, const size_t flag,
                     std::vector<double> &tri_error);

  void cal_gaussian_curvature(const std::vector<point> &vertex_list,
                              const std::vector<triangle> &tri_list,
                              std::vector<double> &gauss_cur_list);


private:
  std::fstream log_out;
};

};

#endif // GAUSSIAN_CURVATURE_H
