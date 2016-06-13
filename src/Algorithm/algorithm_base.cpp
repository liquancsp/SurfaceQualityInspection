#include "algorithm_base.h"
#include <iostream>
#include <sstream>

namespace lq
{

using namespace  std;

algorithm_base::algorithm_base()
{
  log_out_.open("operation_log.txt", fstream::out | fstream::app);
  
}

algorithm_base::~algorithm_base()
{
  
}

void algorithm_base::cal_tri_error(const std::vector<point> &vertex_list1,
                                   const std::vector<point> &vertex_list2,
                                   const std::vector<triangle> &tri_list1,
                                   const std::vector<triangle> &tri_list2,
                                   const point &view_point, const size_t flag,
                                   std::vector<double> &tri_error)
{
  
}

void algorithm_base::tri_center_point(const vector<triangle> &tri_list,
                                      const vector<point> &vertex_list,
                                      vector<point> &center_list)
{
  point tmp;
  center_list.clear();
  for(size_t i = 0; i < tri_list.size(); ++i)
  {
    get_center_point(vertex_list, tri_list[i], tmp);
    center_list.push_back(tmp);
  }
}

//------------------------------------------------------------------------------
/*******************************************************************************
 *@brief calculate i-ring neighbor of each vertex or triangle
 *
 *@param vertex_list : the list of vertex
 *@param tri_list : all triangles
 *@param flag : : ring neighbor type Vertex(ring neighbor of vertex), Triangle(
 *ring neighbor of triangle)
 *@param ring_neighbor_list : store the result
 *
 *@return void
 *
 ******************************************************************************/
void algorithm_base::cal_1_ring_neighbor(const std::vector<point> &vertex_list,
                                         const std::vector<triangle> &tri_list,
                                         const size_t flag,
                                         std::vector<ring_neighbor> &ring_neighbor_list)
{
  cout << "====================================================================" << endl << endl;
  cout << "[#info-algorithm-base]Begin cal 1-ring neighbor" << endl << endl;
  cout << "====================================================================" << endl << endl;
  ring_neighbor_list.clear();
  if(flag == Vertex)
  {
    ring_neighbor_list.resize(vertex_list.size());
    for(size_t i = 0; i < tri_list.size(); ++i)
    {
      size_t idx1, idx2, idx_vertex;
      for(size_t j = 0; j < tri_list[i].vertex.size(); ++j)
      {
        idx1 = tri_list[i].vertex[(j + 1) % 3] - 1;
        idx2 = tri_list[i].vertex[(j + 2) % 3] - 1;
        idx_vertex = tri_list[i].vertex[j] - 1;
        ring_neighbor_list[idx_vertex].adj_set.insert(idx1);
        ring_neighbor_list[idx_vertex].adj_set.insert(idx2);
        ring_neighbor_list[idx_vertex].adj_set.insert(idx_vertex);
      }
    }
  }
  else if(flag == Triangle)
  {
    stringstream format_stream;
    cal_edge_adj_tri(tri_list, edge_flag_);
    ring_neighbor_list.resize(tri_list.size());
    for(size_t i = 0; i < tri_list.size(); ++i)
    {
      size_t idx1, idx2;
      for(size_t j = 0; j < 3; ++j)
      {
        for(size_t k = j + 1; k < 3; ++k)
        {
          idx1 = tri_list[i].vertex[j] - 1;
          idx2 = tri_list[i].vertex[k] - 1;
          format_stream.clear();
          format_stream.str("");
          if(idx1 < idx2)
            swap(idx1, idx2);
          format_stream << idx1 << "," << idx2;
          for(size_t t = 0; t < edge_flag_[format_stream.str()].tri.size(); ++t)
          {
            size_t idx = edge_flag_[format_stream.str()].tri[t];
            ring_neighbor_list[i].adj_set.insert(idx);
          }
        }
      }
    }
  }
  else 
  {
    cerr << "[#Error]Input wrong ring neighbor type. It allow 0-Vertex, 1-Tiangle." << endl;
    cerr << "[#Error]---File : " << __FILE__ << endl;
    cerr << "[#Error]---Line : " << __LINE__ << endl;
    return ;
  }
}

//------------------------------------------------------------------------------
void algorithm_base::output_ring_neighbor(const std::vector<ring_neighbor> &ring_neighbor_list)
{
  ofstream fout;
  fout.open("ring_neighbor.txt");
  set<size_t>::iterator iter;
  //#pragma omp parallel for
  for(size_t i = 0; i < ring_neighbor_list.size(); ++i)
  {
    fout << "index is : " << i << " ";
    for(iter = ring_neighbor_list[i].adj_set.begin(); 
        iter != ring_neighbor_list[i].adj_set.end(); ++iter)
    {
      fout << *iter << " " ;
    }
    fout << endl;
  }
}

//------------------------------------------------------------------------------
/*******************************************************************************
 *@brief Calculate the ring neighbor of each vertex or triangle store in 
 *ring_neighbor_list
 *
 *@param vertex_list : the list of vertex
 *@param tri_list : all triangles
 *@param ring_number : the ring neighbor number of user want to get
 *@param flag : ring neighbor type Vertex(ring neighbor of vertex), Triangle(
 *ring neighbor of triangle)
 *@param ring_neighbor_list : store the result
 *
 *@return void
 *
 ******************************************************************************/
void algorithm_base::cal_ring_neighbor(const std::vector<point> &vertex_list,
                                       const std::vector<triangle> &tri_list,
                                       const size_t ring_number,
                                       const size_t flag,
                                       std::vector<ring_neighbor> &ring_neighbor_list)
{
  cout << "====================================================================" << endl << endl;
  cout << "[#info-algorithm-base]Begin cal ring neighbor" << endl << endl;
  cout << "====================================================================" << endl << endl;
  if(ring_number <= 0)
  {
    cerr << "[#Error]ring_number is error. It should greater than zero." << std::endl;
    cerr << "[#Error]---File : " << __FILE__ << std::endl;
    cerr << "[#Error]---Line : " << __LINE__ << std::endl;
    return;
  }
  std::vector<ring_neighbor> tmp_ring_list;
  cal_1_ring_neighbor(vertex_list, tri_list, flag, tmp_ring_list);
  ring_neighbor_list.clear();
  ring_neighbor_list.resize(vertex_list.size());
  
  for(size_t i = 1; i <= ring_number; ++i)
  {
    set<size_t>::iterator iter;
    for(size_t j = 0; j < ring_neighbor_list.size(); ++j)
    {
      size_t idx;
      if(ring_neighbor_list[j].adj_set.empty())
      {
        ring_neighbor_list[j].adj_set.insert(tmp_ring_list[j].adj_set.begin(),
                                             tmp_ring_list[j].adj_set.end());
      }
      else 
      {
        set<size_t> tmp(ring_neighbor_list[j].adj_set);
        for(iter = tmp.begin(); iter != tmp.end(); ++iter)
        {
          idx = (*iter);
          ring_neighbor_list[j].adj_set.insert(tmp_ring_list[idx].adj_set.begin(),
                                               tmp_ring_list[idx].adj_set.end());
        }
      } 
    }
  } 
}
//------------------------------------------------------------------------------

void  algorithm_base::cal_edge_adj_tri(const std::vector<triangle> &tri_list,
                                       std::map<std::string, edge_adj_tri> &edge_flag)
{
  stringstream format_stream;
  string tmp, tmp2;
  for(size_t i = 0; i < tri_list.size(); ++i)
  {
    size_t idx1, idx2;
    for(size_t j = 0; j < 3; ++j)
    {
      for(size_t k = j + 1; k < 3; ++k)
      {
        tmp.clear();
        tmp.clear();
        idx1 = tri_list[i].vertex[j] - 1;
        idx2 = tri_list[i].vertex[k] - 1;
        if(idx1 < idx2)
          swap(idx1, idx2);
        //clear the string stream
        format_stream.str("");
        format_stream << idx1 << "," << idx2;
        if(edge_flag.find(format_stream.str()) == edge_flag.end())
        {
          edge_flag[format_stream.str()].num = 1;  
        }
        else 
        {
          edge_flag[format_stream.str()].num++;
        }
        edge_flag[format_stream.str()].tri.push_back(i);
       
      }
    }
  }
  
//  map<string, edge_adj_tri>::iterator iter;
//  for(iter = edge_flag.begin(); iter != edge_flag.end(); ++iter)
//  {
    
//    cout << "edge index : " << iter->first;
//    for(size_t i = 0; i < iter->second.tri.size(); ++i)
//    {
//      cout << " " << iter->second.tri[i];
//    }
//    cout << endl;
    
//  }
  
}
//------------------------------------------------------------------------------
}
