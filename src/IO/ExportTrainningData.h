#ifndef EXPORTTRAINNINGDATA_H
#define EXPORTTRAINNINGDATA_H

#include <iostream>
#include <iomanip>
#include <vector>

namespace sqi  {

namespace io {

enum ClassifyType {
  kCubeCorner = 1,
  kSphere, //2
  kFandiskCorner, //3
  kNearCurve, //4
  kNearEdge, // 5
  kNearFandiskCorner, //6
  kPlane, // 7
  kOnEdge, // 8
  kFlake, // 9
  kShoulder, // 10
  kHorn // 11
};


class ExportTrainningData {
public :
  static void ExportZerniker(const std::string& file_name, ClassifyType type,
                             std::vector<std::vector<double> >& zerniker_coeff)
  {
    std::ofstream fout;
    fout.open(file_name.c_str());
    if(fout.fail())
      return;
    
    for(int i = 0; i < zerniker_coeff.size(); ++i)
    {
      fout << type << " ";
      for(int j = 0; j < zerniker_coeff[i].size() - 1; ++j)
        fout << (j + 1) << ":" << zerniker_coeff[i][j] << " ";
      fout << zerniker_coeff.size() << ":" << zerniker_coeff[i][zerniker_coeff[i].size() - 1] << std::endl;
    }
    fout.close();
  }
  
  static void ExportZerniker(const std::string& file_name, ClassifyType type,
                             std::vector<std::vector<double> >& zerniker_coeff,
                             std::vector<int>& idx_list)
  {
    std::ofstream fout;
    fout.open(file_name.c_str());
    if(fout.fail())
      return;
    for(int i = 0; i < idx_list.size(); ++i)
    {
      int idx = idx_list[i];
      fout << type << " ";
      for(int j = 0; j < zerniker_coeff[idx].size() - 1; ++j)
      {
        fout << j + 1 << ":" << zerniker_coeff[idx][j] << " ";
      }
      fout << zerniker_coeff[idx].size() << ":" << zerniker_coeff[idx][zerniker_coeff[idx].size() - 1] << endl;
//      ExportTrainningData::PlusVector<double>(zerniker_coeff[idx], avg_res);
    }
//    ExportTrainningData::DivVector<double>(avg_res, 25.0f);
//    fout << type << " ";
//    for(int i = 0; i < avg_res.size() - 1; ++i)
//    {
//      fout << i + 1 << ":" << avg_res[i] << " ";
//    }
//    fout << avg_res.size() << ":" << avg_res[avg_res.size() - 1] << endl;
    fout.close();
  }
  
  static void ExportZernikerTestData(const std::string& file_name,
                                     std::vector<std::vector<double> >& zerniker_coeff)
  {
    std::ofstream fout;
    fout.open(file_name.c_str());
    if(fout.fail())
      return;
    for(int i = 0; i < zerniker_coeff.size(); ++i)
    {
      fout << -1 << " ";
      for(int j = 0; j < zerniker_coeff[i].size() - 1; ++j)
        fout << (j + 1) << ":" << zerniker_coeff[i][j] << " ";
      fout << zerniker_coeff[i].size() << ":" << zerniker_coeff[i][zerniker_coeff[i].size() - 1] << std::endl;
    }
    fout.close();
  }
  
  static void ExportCurvatureTestData(const std::string& file_name,
                                      std::vector<double>& curvature)
  {
    std::ofstream fout;
    fout.open(file_name.c_str());
    if(fout.fail())
      return;
    for(int i = 0; i < curvature.size(); ++i)
      fout << -1 << " " << 1 << ":" << curvature[i] << std::endl;
    fout.close();
  }

  static void ExportZernikerTestData(const std::string& file_name,
                                     std::vector<std::vector<double> >& zerniker_coeff,
                                     std::vector<int>& idx_list)
  {
    std::ofstream fout;
    fout.open(file_name.c_str());
    if(fout.fail())
      return;
    for(int i = 0; i < idx_list.size(); ++i)
    {
      int idx = idx_list[i];
      fout << 1 << " ";
      for(int j = 0; j < zerniker_coeff[idx].size() - 1; ++j)
      {
        fout << j + 1 << ":" << zerniker_coeff[idx][j] << " ";
      }
      fout << zerniker_coeff[idx].size() << ":" << zerniker_coeff[idx][zerniker_coeff[idx].size() - 1] << endl;
    }
    fout.close();
  }
  
private:
  
  template<typename T>
  static void PlusVector(std::vector<T>& vec1, std::vector<T>& vec2)
  {
    for(int i = 0; i < vec1.size(); ++i)
      vec2[i] += vec1[i];
  }
  
  template<typename T>
  static void DivVector(std::vector<T>& vec, T num)
  {
    if(num == 0)
      return;
    for(int i = 0; i < vec.size(); ++i)
      vec[i] /= num;
  }
  
};

}
}

#endif // EXPORTTRAINNINGDATA_H
