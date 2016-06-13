#ifndef PRINCIPAL_CURVATURE_H
#define PRINCIPAL_CURVATURE_H

#include "curvature.h"

namespace lq
{

class principal_curvature : public curvature
{

public:

  principal_curvature():curvature()
  {}

  ~principal_curvature()
  {}

  void cal_tri_error(const std::vector<point> &vertex_list1,
                     const std::vector<point> &vertex_list2,
                     const std::vector<triangle> &tri_list1,
                     const std::vector<triangle> &tri_list2,
                     const point &view_point, const size_t flag,
                     std::vector<double> &tri_error);

  void cal_principal_curvature(const std::vector<point> &vertex_list,
                               const std::vector<triangle> &tri_list,
                               std::vector<principal_curvature_info> &prin_cur_list);
  
private:
  
  void cal_vertex_error_prin_cur(const std::vector<point> &vertex_list1,
                                 const std::vector<point> &vertex_list2,
                                 const std::vector<triangle> &tri_list1,
                                 const std::vector<triangle> &tri_list2,
                                 const size_t flag,
                                 std::vector<double> &vertex_error);
  

};

};

#endif // PRINCIPAL_CURVATURE_H
