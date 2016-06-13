#ifndef DISTANCE_DIFFERENT_H
#define DISTANCE_DIFFERENT_H

#include "data_type.h"
#include "algorithm_base.h"

namespace lq
{

  class distance_different : public algorithm_base
  {
 public:
    distance_different();
    ~distance_different();
    void cal_tri_error(const std::vector<point> &vertex_list1,
                       const std::vector<point> &vertex_list2,
                       const std::vector<triangle> &tri_list1,
                       const std::vector<triangle> &tri_list2,
                       const point &view_point, const size_t flag,
                       std::vector<double> &tri_error);

 private:

    std::fstream log_out_;
    void distance_two_point(const point &p1, const point &p2,
                            double &dist);
    
  };

}

#endif
