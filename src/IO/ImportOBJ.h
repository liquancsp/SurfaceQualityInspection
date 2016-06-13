#ifndef IMPORTOBJ_H
#define IMPORTOBJ_H
#include "../Algorithm/data_type.h"
#include <ctime>
#include <iomanip>

namespace sqi {
namespace io {

class ImportOBJ{
  
public:
  
  static int Open(const std::string filename,
                  std::vector<lq::point> &vertex_list,
                  std::vector<lq::vec> &normal_list,
                  std::vector<lq::triangle> &triangle_list,
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
    lq::point tmp_vertex;
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
        lq::triangle tmp_triangle;
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
  
  static void cal_vertex_normal(const std::vector<lq::point> &vertex_list,
                                std::vector<lq::triangle> &triangle_list,
                                std::vector<lq::vec> &normal_list)
  {
    normal_list.resize(vertex_list.size());
  //#pragma omp parallel for num_threads(thread_num(triangle_list.size(), MIN_ITERATOR_NUM))
    for(size_t i = 0; i < triangle_list.size(); ++i)
    {
      lq::vec vec1, vec2, vec0;
      lq::point p1 = lq::point(vertex_list[triangle_list[i].vertex[0] - 1]);
      lq::point p2 = lq::point(vertex_list[triangle_list[i].vertex[1] - 1]);
      lq::point p3 = lq::point(vertex_list[triangle_list[i].vertex[2] - 1]);
      vec1 = p1 - p2;
      vec2 = p1 - p3;
      vec0 = cross_mult(vec1, vec2);
      triangle_list[i].triangle_normal = vec0;
    }
  }
};

}
}

#endif // IMPORTOBJ_H
