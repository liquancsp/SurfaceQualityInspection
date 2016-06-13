#ifndef READ_OBJ_H
#define READ_OBJ_H
#include "data_type.h"
#include "../3rd/zjucad/matrix/matrix.h"
#include "../3rd/zjucad/matrix/io.h"
#include <fstream>

namespace lq {

  class read_obj {
    
 public:
    
    struct index {
      std::vector<int> face_num; 
    };
    
    int get_obj(const std::string filename,
                std::vector<point> &vertex_list,
                std::vector<vec> &normal_list,
                std::vector<triangle> &triangle_list,
                std::fstream &fout);

    int get_obj(const std::string filename,
                std::vector<point> &vertex_list,
                std::vector<triangle_normal> &normal_list,
                std::vector<triangle> &triangle_list,
                std::vector<point> &tri_center);

    int output_diff_vertex(const std::vector<point> &vertex_list1,
                           const std::vector<point> &vertex_list2) const;
    

    bool is_tri_normal_equal(const std::vector<triangle> &triangle_list1,
                             const std::vector<triangle> &triangle_list2) const;
    
    bool is_normal_equal(const std::vector<vertex_normal> &normal_list1,
                         const std::vector<vertex_normal> &normal_list2) const;
    void get_bounding_box(const std::vector<point> &vertex_list,
                          double bounding_box[6]);
    
    static void cal_vertex_normal(const std::vector<point> &vertex_list,
                                  std::vector<triangle> &triangle_list,
                                  std::vector<vec> &normal_list);
    
    static int output_obj(const std::vector<point> &vertex_list,
                          const std::vector<vec> &normal_list,
                          const std::vector<triangle> &triangle_list);
    
  private:
    
    void get_vector(const point &p1, const point &p2,
                    zjucad::matrix::matrix<double> &mat);
    
    void get_face_normal();



    vertex_normal sum(const std::vector<int> &list,
                      const std::vector<triangle> &triangle_list);
    
    void sum(const std::vector<int> &list,
                       const std::vector<triangle> &triangle_list,
                       vec &vn);
    

    void cal_tri_normal(const std::vector<point> &vertex_list,
                        std::vector<triangle> &triangle_list,
                        std::vector<triangle_normal> &tri_normal,
                        std::vector<point> &tri_center);

    
  };
}
#endif
