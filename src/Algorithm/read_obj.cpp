#include "read_obj.h"
#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>

namespace lq {

using namespace std;

void output_center_point(const std::vector<point> &tri_center)
{
  std::ofstream fout;
  fout.open("tri_center.txt");
  for(size_t i = 0; i < tri_center.size(); ++i)
  {
      fout << tri_center[i].x << " " << tri_center[i].y
           << " " << tri_center[i].z << std::endl;
  }
  fout.close();
}

void output_tri_normal(const std::vector<triangle_normal> &tri_normal)
{
  std::ofstream fout;
  fout.open("tri_normal.txt");
  for(size_t i = 0; i < tri_normal.size(); ++i)
  {
    fout << tri_normal[i].normal(0, 0) << " " << tri_normal[i].normal(0, 1)
           << " " << tri_normal[i].normal(0, 2) << std::endl;
  }
  fout.close();
}
int read_obj::get_obj(const std::string filename,
                      std::vector<point> &vertex_list,
                      std::vector<vec> &normal_list,
                      std::vector<triangle> &triangle_list,
                      std::fstream &fout)
{
  cout << "============================================================================" << endl << endl;
  cout << "[#info-load-obj]Begin open the file : " << std::endl << filename << std::endl << endl; 
  cout << "============================================================================" << endl << endl;
  std::ifstream fin;
  clock_t start, end;
  //std::ofstream fout("test.txt");
  start = clock();
  fin.open(filename.c_str());
  if(fin.fail())
  {
    std::cerr << "error happen at:" << std::endl;
    std::cerr << "file: " << __FILE__ << std::endl;
    std::cerr << "line: " << __LINE__ << std::endl;
    return 1;
  }
  else
    std::cout << "file " << filename << " open successful!" << std::endl;
 
  std::string input;
  std::string sub_str;
  point tmp_vertex;
  int index1, index2, index3;
  while(getline(fin, input))
  {
    if(input[0] == 'v' && input[1] == 't')
      continue;
    else if(input[0] == 'v' && input[1] == 'n')
      continue;
    else if(input[0] == 'v')
    {
      sub_str = input.substr(1);
      sscanf(sub_str.c_str(), "%lf %lf %lf", &tmp_vertex.x, &tmp_vertex.y, &tmp_vertex.z);
      vertex_list.push_back(tmp_vertex);
    }
    else if(input[0] == 'f')
    {
      triangle tmp_triangle;
      // triangle tmp_triangle;
      int tmp_index, texture, begin = 0, end = 0;
      input = input.substr(2);
      end = input.find(" ");
      std::string tmp;
      tmp = input.substr(begin, end);
      int flag;
      flag = sscanf(tmp.c_str(), "%d/%d", &tmp_index, &texture);
      if(flag == 2)
      {
        while(end != std::string::npos)
        {
          sub_str = input.substr(begin, end);
          sscanf(sub_str.c_str(), "%d/%d", &tmp_index, &texture);
          tmp_triangle.vertex.push_back(tmp_index);
          input = input.substr(end + 1, input.length() - 1);
          end = input.find(" ");
        }
        sscanf(input.c_str(), "%d/%d", &tmp_index, &texture);
        tmp_triangle.vertex.push_back(tmp_index);
        triangle_list.push_back(tmp_triangle);
        tmp_triangle.vertex.clear();
      }
      else if(flag == 1)
      {
        sscanf(input.c_str(), "%d %d %d", &index1, &index2, &index3);
        tmp_triangle.vertex.push_back(index1);
        tmp_triangle.vertex.push_back(index2);
        tmp_triangle.vertex.push_back(index3);
        triangle_list.push_back(tmp_triangle);
      }
    }
    input.clear();
    sub_str.clear();
  }
  cal_vertex_normal(vertex_list, triangle_list, normal_list);
  end = clock();
  cout << "[#info-load-obj]---The point number of the model is : " << 
          vertex_list.size() << endl;
  cout << "[#info-load-obj]---The triangle number of the model is : " 
       << triangle_list.size() << endl;
  cout << "[#info-load-obj]---The cost time is : " <<
          setiosflags(ios::fixed) << setprecision(5) <<
          difftime(end, start) / CLOCKS_PER_SEC << " seconds." << endl;
  return 0;
}

int read_obj::get_obj(const std::string filename,
                      std::vector<point> &vertex_list,
                      std::vector<triangle_normal> &tri_normal,
                      std::vector<triangle> &triangle_list,
                      std::vector<point> &tri_center)
{
   std::ifstream fin;
  //std::ofstream fout("test.txt");
  fin.open(filename.c_str());
  if(fin.fail())
  {
    std::cerr << "error happen at:" << std::endl;
    std::cerr << "file: " << __FILE__ << std::endl;
    std::cerr << "line: " << __LINE__ << std::endl;
    return 1;
  }
  else
    std::cout << "file " << filename << " open successful!" << std::endl;
  std::string input;
  std::string sub_str;
  point tmp_vertex;
  int index1, index2, index3;
  while(getline(fin, input))
  {
    if(input[0] == 'v' && input[1] == 't')
      continue;
    else if(input[0] == 'v' && input[1] == 'n')
      continue;
    else if(input[0] == 'v')
    {
      sub_str = input.substr(1);
      sscanf(sub_str.c_str(), "%lf %lf %lf", &tmp_vertex.x, &tmp_vertex.y, &tmp_vertex.z);
      vertex_list.push_back(tmp_vertex);
    }
    else if(input[0] == 'f')
    {
      triangle tmp_triangle;
      // triangle tmp_triangle;
      int tmp_index, texture, begin = 0, end = 0;
      input = input.substr(2);
      end = input.find(" ");
      std::string tmp;
      tmp = input.substr(begin, end);
      int flag;
      flag = sscanf(tmp.c_str(), "%d/%d", &tmp_index, &texture);
      if(flag == 2)
      {
        while(end != std::string::npos)
        {
          sub_str = input.substr(begin, end);
          sscanf(sub_str.c_str(), "%d/%d", &tmp_index, &texture);
          tmp_triangle.vertex.push_back(tmp_index);
          input = input.substr(end + 1, input.length() - 1);
          end = input.find(" ");
        }
        sscanf(input.c_str(), "%d/%d", &tmp_index, &texture);
        tmp_triangle.vertex.push_back(tmp_index);
        triangle_list.push_back(tmp_triangle);
        tmp_triangle.vertex.clear();
      }
      else if(flag == 1)
      {
        sscanf(input.c_str(), "%d %d %d", &index1, &index2, &index3);
        tmp_triangle.vertex.push_back(index1);
        tmp_triangle.vertex.push_back(index2);
        tmp_triangle.vertex.push_back(index3);
        triangle_list.push_back(tmp_triangle);
      }
    }
    input.clear();
    sub_str.clear();
  }
  cal_tri_normal(vertex_list, triangle_list, tri_normal, tri_center);
  output_center_point(tri_center);
  output_tri_normal(tri_normal);
  return 0;
} 

int read_obj::output_diff_vertex(const std::vector<point> &vertex_list1,
                                 const std::vector<point> &vertex_list2) const
{
  std::ofstream fout("diff_vertex.obj");
  if(fout.fail())
  {
    std::cerr << "fail to open out.obj" << std::endl;
    std::cerr << "file: " << __FILE__ << std::endl;
    std::cerr << "line: " << __LINE__ << std::endl;
    return 1;
  }
  else
     std::cout << "diff_vertex.obj open successful!" << std::endl;
  for(size_t i = 0 ; i < vertex_list1.size(); ++i)
  {
    if(!is_zero(vertex_list1[i].x - vertex_list2[i].x) &&
       !is_zero(vertex_list1[i].y - vertex_list2[i].y) &&
       !is_zero(vertex_list1[i].z - vertex_list2[i].z))
      fout << i + 1 << std::endl;
  }
  return 0;
}
int read_obj::output_obj(const std::vector<point> &vertex_list,
                         const std::vector<vec> &normal_list,
                         const std::vector<triangle> &triangle_list)
{
  std::ofstream fout("out.obj");
  if(fout.fail())
  {
    std::cerr << "fail to open out.obj" << std::endl;
    std::cerr << "file: " << __FILE__ << std::endl;
    std::cerr << "line: " << __LINE__ << std::endl;
    return 1;
  }
  else
     std::cout << "out.obj open successful!" << std::endl;
  for(size_t i = 0; i < vertex_list.size(); ++i)
    fout << "v " << vertex_list[i].x << " " <<
        vertex_list[i].y << " " << vertex_list[i].z << std::endl;

  for(size_t i = 0; i < normal_list.size(); ++i)
    fout << "vn " << normal_list[i].x << " " <<
        normal_list[i].y << " " << normal_list[i].z << std::endl;

  for(size_t i = 0; i < triangle_list.size(); ++i)
    fout << "f " << triangle_list[i].vertex[0] << " " <<
        triangle_list[i].vertex[1] << " " <<
        triangle_list[i].vertex[2] << " " << std::endl;
  fout.close();
  return 0;
}

 
void read_obj::get_vector(const point &p1, const point &p2,
                          zjucad::matrix::matrix<double> &mat)
{
  point  tmp;
  mat = zjucad::matrix::zeros(1, 3);
  tmp = p1 - p2;
  mat(0, 0) = tmp.x;
  mat(0, 1) = tmp.y;
  mat(0, 2) = tmp.z;
}


void read_obj::sum(const std::vector<int> &list,
                   const std::vector<triangle> &triangle_list,
                   vec &vn)
{
  vec res(0.0, 0.0, 0.0);
  for(size_t i = 0; i < list.size(); ++i)
    res = res + triangle_list[list[i]].triangle_normal;
  vn = res;
}

bool read_obj::is_tri_normal_equal(const std::vector<triangle>
                                   &triangle_list1,
                                   const std::vector<triangle>
                                   &triangle_list2) const
{
  for(size_t i = 0; i < triangle_list1.size(); ++i)
  {
    if(equal(triangle_list1[i].triangle_normal,
             triangle_list2[i].triangle_normal))
      continue;
    else
      return false;
  }
  return true;
}

bool read_obj::is_normal_equal(const std::vector<vertex_normal> &normal_list1,
                               const std::vector<vertex_normal> &normal_list2) const
{
  for(size_t i = 0; i < normal_list1.size(); ++i)
  {
    if(equal(normal_list1[i], normal_list2[i]))
      continue;
    else
      return false;
  }
  return true;
}

void read_obj::get_bounding_box(const std::vector<point> &vertex_list, double bounding_box[6])
{
  //init the bounding_box
  double MAX = 10000.0f;
  for(size_t i = 0; i < 6; ++i)
  {
    if((i & 1) == 0)
    {
      bounding_box[i] = MAX;
    }
    else
    {
      bounding_box[i] = -MAX;
    }
  }
  point tmp;
  for(size_t i = 0; i < vertex_list.size(); ++i)
  {
    if(is_less(vertex_list[i].x, bounding_box[0]))
    {
      bounding_box[0] = vertex_list[i].x;
    }
    if(is_larger(vertex_list[i].x, bounding_box[1]))
    {
      bounding_box[1] = vertex_list[i].x;
    }
    if(is_less(vertex_list[i].y, bounding_box[2]))
    {
      bounding_box[2] = vertex_list[i].y;
    }
    if(is_larger(vertex_list[i].y, bounding_box[3]))
    {
      bounding_box[3] = vertex_list[i].y;
    }
    if(is_larger(bounding_box[4], vertex_list[i].z))
    {
      bounding_box[4] = vertex_list[i].z;
    }
    if(is_larger(vertex_list[i].z, bounding_box[5]))
    {
      bounding_box[5] = vertex_list[i].z;
    }
  }
}

//compute normal of each vertex
//it equal average of all face which include it.
void read_obj::cal_vertex_normal(const std::vector<point> &vertex_list,
                                 std::vector<triangle> &triangle_list,
                                 std::vector<vec> &normal_list)
{
  //std::vector<index> face_index;
  //face_index.resize(vertex_list.size());
  normal_list.resize(vertex_list.size());
//#pragma omp parallel for num_threads(thread_num(triangle_list.size(), MIN_ITERATOR_NUM))
  for(size_t i = 0; i < triangle_list.size(); ++i)
  {
    //zjucad::matrix::matrix<double> vec0, vec1, vec2;
    vec vec1, vec2, vec0;
//    face_index[triangle_list[i].vertex[0] - 1].face_num.push_back(i);
//    face_index[triangle_list[i].vertex[1] - 1].face_num.push_back(i);
//    face_index[triangle_list[i].vertex[2] - 1].face_num.push_back(i);
    point p1 = point(vertex_list[triangle_list[i].vertex[0] - 1]);
    point p2 = point(vertex_list[triangle_list[i].vertex[1] - 1]);
    point p3 = point(vertex_list[triangle_list[i].vertex[2] - 1]);
    vec1 = p1 - p2;
    vec2 = p1 - p3;
  //  get_vector(p1, p2, vec1);
//    get_vector(p1, p3, vec2);
//    vec0 = trans(cross(vec1, vec2));
    vec0 = cross_mult(vec1, vec2);
   //vec tmp_normal(vec0(0, 0), vec0(0, 1), vec0(0, 2));
    //vec0 = vec0 /vec_norm(vec0);
    triangle_list[i].triangle_normal = vec0;
  }
//  vec tmp_normal;
//  for(size_t i = 0; i < face_index.size(); ++i)
//  {
//    sum(face_index[i].face_num, triangle_list, tmp_normal);
//    normal_list[i] = tmp_normal / face_index[i].face_num.size();
//  }
}

void read_obj::cal_tri_normal(const std::vector<point> &vertex_list,
                              std::vector<triangle> &triangle_list,
                              std::vector<triangle_normal> &tri_normal,
                              std::vector<point> &tri_center)    
{
  point p1, p2, p3, center;
  int index1, index2, index3;
  zjucad::matrix::matrix<double> vec1 = zjucad::matrix::zeros(1, 3);
  zjucad::matrix::matrix<double> vec2 = zjucad::matrix::zeros(1, 3);
  triangle_normal tmp_normal;
  std::cout << "tri_size = " << triangle_list.size() << std::endl;
  for(size_t i = 0; i < triangle_list.size(); ++i)
  {
    p1 = vertex_list[triangle_list[i].vertex[0] - 1];
    p2 = vertex_list[triangle_list[i].vertex[1] - 1]; 
    p3 = vertex_list[triangle_list[i].vertex[2] - 1];
    center = (p1 + p2 + p3) / double(3.0);
    get_vector(p1, p2, vec1);
    get_vector(p1, p3, vec2);
    tmp_normal.normal = trans(cross(vec1, vec2));
    if(!is_zero(norm(tmp_normal.normal)))
      tmp_normal.normal /= norm(tmp_normal.normal);
    tri_normal.push_back(tmp_normal);
    tri_center.push_back(center);
  }
}


}
