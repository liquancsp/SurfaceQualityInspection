#ifndef RIGID_TRANSFOR_H
#define RIGID_TRANSFOR_H
/*
#include "data_type.h"
#include "reflection_map.h"
#include "rigid_transfor_function.h"
#include "../3rd/zjucad/ptree/ptree.h"
#include <tr1/memory>

namespace lq {
using namespace hj::function;
class rigid_transfor : public reflection_map
{

public:
  
  rigid_transfor();
  ~rigid_transfor();
  size_t get_ring_number()
  {
    return ring_number_;
  }

  size_t get_ring_tyoe()
  {
    return this->ring_type_;
  }
  
//  void get_ring_ptr(std::tr1::shared_ptr<std::vector<ring_neighbor> > &ptr)
//  {
//    ptr = this->ring_neighbor_ptr_;
//  }
  
  void set_ring_number(size_t ring_number)
  {
    this->ring_number_ = ring_number;
  }
  
  
  void set_ring_type(size_t ring_type)
  {
    this->ring_type_ = ring_type;
  }
  
//  void set_ring_ptr(const std::tr1::shared_ptr<std::vector<ring_neighbor> > &ring_list)
//  {
//    this->ring_neighbor_ptr_ = ring_list;
//  }
  
  void cal_tri_error(const std::vector<point> &vertex_list1,
                     const std::vector<point> &vertex_list2,
                     const std::vector<triangle> &tri_list1,
                     const std::vector<triangle> &tri_list2,
                     const point &view_point,
                     std::vector<double> &tri_error);
  
protected:
  
  void optimize_rigid_triansfor(const std::vector<point> &vertex_list1,
                                const std::vector<point> &vertex_list2,
                                const std::vector<triangle> &tri_list1,
                                const std::vector<triangle> &tri_list2,
                                ring_neighbor &ring_neighbor_item);
  
  void set_optimize_function(const std::vector<point> &vertex_list1,
                             const std::vector<point> &vertex_list2,
                             const std::vector<triangle> &tri_list1,
                             const std::vector<triangle> &tri_list2,
                             ring_neighbor &ring_neighbor_item);
  
private:
  zjucad::ptree pt_;
  boost::property_tree::ptree pt;
  size_t ring_number_;
  size_t ring_type_;
  std::set<size_t> neighbor_list_;
  matrixd rigid_mat_;
  std::vector<ring_neighbor> ring_neighbor_list_;
  typedef std::tr1::shared_ptr<const hj::function::function_t<double, int32_t> > func_ptr;
  std::vector<func_ptr> rigid_func_list_;
 
  
};


};*/
#endif // RIGID_TRANSFOR_H
