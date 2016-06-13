#ifndef OUT_VTK_H
#define OUT_VTK_H
#include "vtk.h"
#include "data_type.h"
#include <iostream>
#include <fstream>
#include <string>
char scalars[] = "error";
char lookup_table[] = "error_table" ;    
const size_t TRI_VERTEX_NUM = 3;
namespace lq {

  void trans_vertex_format(const std::vector<point> &vertex_list,
                         std::vector<double> &out_list)
  {
    for(size_t i = 0; i < vertex_list.size(); ++i)
    {
      out_list.push_back(vertex_list[i].x);
      out_list.push_back(vertex_list[i].y);
      out_list.push_back(vertex_list[i].z);
    }
  }

  void trans_tri_format(const std::vector<triangle> &tri_list,
                         std::vector<int> &out_tri)
  {
    for(size_t i = 0; i < tri_list.size(); ++i)
    {
      for(size_t j = 0 ; j < TRI_VERTEX_NUM; ++j)
        out_tri.push_back(tri_list[i].vertex[j] - 1);
    }
  }

  int out_vtk_file(const std::string filename,
                   const std::vector<point> &vertex_list,
                   size_t node_num,
                   const std::vector<triangle> &tri_list,
                   size_t tri_num,
                   const std::vector<double> &error_list)
  {
    std::ofstream fout;
    fout.open(filename.c_str());
    if(fout.fail())
    {
      std::cerr << "fail to open " << filename << std::endl;
      std::cerr << "error happen at :" << std::endl;
      std::cerr << __FILE__ << std::endl;
      std::cerr << __LINE__ << std::endl;
      return 1;
    }
    else
      std::cout << filename << " open successful" << std::endl;
    std::vector<double> out_list;
    std::vector<int> out_tri;
    trans_vertex_format(vertex_list, out_list);
    trans_tri_format(tri_list, out_tri);
    tri2vtk(fout, out_list, node_num, out_tri, tri_num);
    cell_data(fout, error_list, error_list.size(), scalars, lookup_table);
    fout.close();
    return 0;
  } 
}
#endif
