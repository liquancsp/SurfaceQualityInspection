#ifndef ALGORITHM_BASE_H
#define ALGORITHM_BASE_H

#include "data_type.h"

#include <fstream>

namespace lq
{

  class algorithm_base
  {
 public:
    
    algorithm_base();
    ~algorithm_base();
    virtual void cal_tri_error(const std::vector<point> &vertex_list1,
                               const std::vector<point> &vertex_list2,
                               const std::vector<triangle> &tri_list1,
                               const std::vector<triangle> &tri_list2,
                               const point &view_point, const size_t flag,
                               std::vector<double> &tri_error);
    
    void cal_1_ring_neighbor(const std::vector<point> &vertex_list,
                             const std::vector<triangle> &tri_list,
                             const size_t flag,
                             std::vector<ring_neighbor> &ring_neighbor_list);
    
    void cal_ring_neighbor(const std::vector<point> &vertex_list,
                           const std::vector<triangle> &tri_list,
                           const size_t ring_number,
                           const size_t flag,
                           std::vector<ring_neighbor> &ring_neighbor_list);
    
    
    void output_ring_neighbor(const std::vector<ring_neighbor> &ring_neighbor_list);
    
    
    void cal_edge_adj_tri(const std::vector<triangle> &tri_list,
                          std::map<std::string, edge_adj_tri> &edge_flag);
    
  protected:

    void tri_center_point(const std::vector<triangle> &tri_list,
                          const std::vector<point> &vertex_list,
                          std::vector<point> &center_list);

    
    
 //Store the adjacency triangle of each edge
    std::map<std::string, edge_adj_tri> edge_flag_; 

 private:

    std::fstream log_out_;

  };

}


#endif
