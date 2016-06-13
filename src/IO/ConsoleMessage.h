#pragma once
#ifndef CONSOLEMESSAGE_H
#define CONSOLEMESSAGE_H
#define PROJECT_DEV
#include <data_type.h>

#include <iostream>
#include <iomanip>

namespace sqi
{
namespace io
{

enum InfoType
{
  kImportSuccess = 0,
  kImportFail,
  kStartAlg,
  kEndAlg,
  kStartColor,
  kEndColor,
  kCostTime
};

enum InfoTextType
{
  kInfo = 0,
  kInfoAlg,
  kInfoFile,
  kInfoColor,
  kInfoTime,
  kInfoThread,
  kErrorFile,
  kError
};

template<typename T>
class ConsoleMessage
{
public:

  static void SendMessage(const T& str, InfoType type, 
                          lq::InspectFlag alg_type = lq::kReflectionMap, 
                          double max_error = 0.0f, double cost_time = 0.0f)
  {
#ifdef PROJECT_DEV
    if(type == kImportSuccess)
    {
      MessageText("Import file : ", kInfoFile, str);
    }
    else if(type == kImportFail)
    {
      MessageText("Fail to Import file : ", kErrorFile, str);
    }
    else if(type == kStartAlg)
    {
      if(!is_has_alg_type_text_)
      {
        InitAlgType();
        is_has_alg_type_text_ = true;
      }
      assert(alg_type < alg_type_text_.size());
      if(alg_type > alg_type_text_.size())
      {
        MessageError(__FILE__, __LINE__);
        return ;
      }
      OutDelimiter();
      MessageText(alg_type_text_[alg_type], kInfoAlg);
      OutDelimiter();
      std::cout << std::endl;
    }
    else if(type == kEndAlg)
    {
      std::cout << "[#Info-algorithm]The maximum error is : " << std::setiosflags(std::ios::fixed) 
                << std::setprecision(8) << max_error << std::endl; 
      MessageCostTime(cost_time);
      MessageText("Complete the algorithm.", kInfoAlg);
      OutDelimiter();
    }
#endif
  }
  
  static void MessageFailImportFile(const T& file_name, const T& error_file ,
                                    int line)
  {
#ifdef PROJECT_DEV
    MessageText("Fail to import file : ", kErrorFile, file_name);
    MessageError(__FILE__, __LINE__);
#endif
  }
  
  static void MessageError(const T& error_file, int line)
  {
#ifdef PROJECT_DEV
    if(!is_has_info_type_text_)
    {
      is_has_info_type_text_ = InitInfoText();
    }
    std::cout <<  info_type_text_[kError] << "Error happen in file : " << error_file <<
                  " in line : " << line << std::endl;
#endif
  }

  static void MessageCostTime(double cost_time)
  {
#ifdef PROJECT_DEV
    std::cout << "[#Info-time]The cost time is : " << std::setiosflags(std::ios::fixed) 
              << std::setprecision(5) << cost_time  << " seconds." << std::endl;
#endif
  }
  
  static void MessageText(const T& text, InfoTextType type)
  {
#ifdef PROJECT_DEV
    if(!is_has_info_type_text_)
    {
      is_has_info_type_text_ = InitInfoText();
    }
    assert(type < info_type_text_.size());
    if(type >= info_type_text_.size())
    {
      MessageError(__FILE__, __LINE__);
      return ;
    }
    std::cout << info_type_text_[type] << text << std::endl;
#endif
  }
  template<typename VALUE>
  static void MessageText(const T& text, InfoTextType type, VALUE value)
  {
#ifdef PROJECT_DEV
    if(!is_has_info_type_text_)
    {
      is_has_info_type_text_ = InitInfoText();
    }
    if(type >= info_type_text_.size())
    {
      MessageError(__FILE__, __LINE__);
      return ;
    }
    std::cout << info_type_text_[type] << text << value << std::endl;
#endif
  }
  
  static bool InitAlgType()
  {
    alg_type_text_.clear();
    alg_type_text_.push_back("reflection map algorithm");
    alg_type_text_.push_back("integrate reflection algorithm");
    alg_type_text_.push_back("Global align integral reflection map");
    alg_type_text_.push_back("Globla align normal component");
    alg_type_text_.push_back("Local align integral reflection map");
    alg_type_text_.push_back("Locak align normal component");
    alg_type_text_.push_back("convex different");
    alg_type_text_.push_back("mixed reflection and convex");
    alg_type_text_.push_back("mixed integral and convex");
    alg_type_text_.push_back("mean curvature algorithm");
    alg_type_text_.push_back("gaussian curvature algorithm");
    alg_type_text_.push_back("principal K1 algorithm");
    alg_type_text_.push_back("principal K2 algorithm");
    alg_type_text_.push_back("different normal vector algorithm");
    alg_type_text_.push_back("different distance algorithm");
    alg_type_text_.push_back("Mixed mean curvature and integral");
    alg_type_text_.push_back("Mixed gaussian curvature and integral");
    alg_type_text_.push_back("Mixed K1 and integral");
    alg_type_text_.push_back("Mixed K2 and integral");
    alg_type_text_.push_back("Defect Fitting");
    return true;
  }          

  static bool InitInfoText()
  {
    info_type_text_.clear();
    info_type_text_.push_back("[#Info]");
    info_type_text_.push_back("[#Info-algorithm]");
    info_type_text_.push_back("[#Info-file]");
    info_type_text_.push_back("[#Info-color]");
    info_type_text_.push_back("[#Info-time]");
    info_type_text_.push_back("[#Info-threads]");
    info_type_text_.push_back("[#Error-file]");
    info_type_text_.push_back("[#Error]");
    return true;
  }
  
protected:

  static void OutDelimiter()
  {
#ifdef PROJECT_DEV
    std::cout << "=================================================================" << std::endl;
#endif
  }
  
  static bool is_has_alg_type_text_;
  static bool is_has_info_type_text_;
  static std::vector<std::string> alg_type_text_;
  static std::vector<std::string> info_type_text_;
  
                   
};

template<typename T>
std::vector<std::string> ConsoleMessage<T>::alg_type_text_;

template<typename T>
std::vector<std::string> ConsoleMessage<T>::info_type_text_;

template<typename T>
bool  ConsoleMessage<T>::is_has_alg_type_text_ = false;
template<typename T>
bool  ConsoleMessage<T>::is_has_info_type_text_ = false;

}
}


#endif // CONSOLEMESSAGE_H
