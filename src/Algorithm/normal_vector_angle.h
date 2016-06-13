#ifndef NORMAL_VECTOR_ANGLE_H
#define NORMAL_VECTOR_ANGLE_H

#include "data_type.h"
#include "algorithm_base.h"

namespace lq
{


  class normal_vector_angle : public algorithm_base
  {
    
 public:
    
    normal_vector_angle();
    ~normal_vector_angle();
    void cal_tri_error(const std::vector<point> &vertex_list1,
                       const std::vector<point> &vertex_list2,
                       const std::vector<triangle> &tri_list1,
                       const std::vector<triangle> &tri_list2,
                       const point &view_point, const size_t flag,
                       std::vector<double> &tri_error);

 private:
    std::fstream log_out_;
    
  };

}

#endif
