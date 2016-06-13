#pragma once
#ifndef VTKPROPMANAGE_H
#define VTKPROPMANAGE_H
#include <fstream>
#include <string>
#include <cstring>
#include <iostream>
namespace sqi {

namespace VtkData {

enum ColorType
{
  kBackGround = 0,
  kCellEdge,
  kCell,
  kLeftActor,
  kRightActor,
  kBackFaceActor
};

typedef ColorType  ColorEditId;

struct Light
{
  Light(double d, double s, double sp) : diffuse(d), specular(s),
    specular_power(sp){}
  double diffuse;
  double amibent;
  double specular;
  double specular_power;
};

class VtkPropManage 
{
public :
  VtkPropManage()
  {
    //VtkPropManage::DefaultColor();
  }
  
  ~VtkPropManage()
  {}
  
  static int* BackGround()                  {return color_list_[kBackGround];}
  static int* CellEdge()                    {return color_list_[kCellEdge];}
  static int* Cell()                        {return color_list_[kCell];}
  static int* ActorColor(ColorType flag)    {return color_list_[flag];}
  static int* BackFaceColor()               {return color_list_[kBackFaceActor];}
  
  static double& DiffuseValue()             {return light.diffuse;}
  static double& AmbientValue()             {return light.amibent;}
  static double& SpecularValue()            {return light.specular;}
  static double& SpecularPower()            {return light.specular_power;}
  static Light& LightStruct()               {return light;}
  
  static void SetBackground(int r, int g, int b)
  {
    color_list_[kBackGround][0] = r;
    color_list_[kBackGround][1] = g;
    color_list_[kBackGround][2] = b;
  }
  
  static void SetCellEdge(int r, int g, int b)
  {
    color_list_[kCellEdge][0] = r;
    color_list_[kCellEdge][1] = g;
    color_list_[kCellEdge][2] = b;
  }
  
  static void SetCell(int r, int g, int b)
  {
    color_list_[kCell][0] = r;
    color_list_[kCell][1] = g;
    color_list_[kCell][2] = b;
  }
  
  static void SetLeftActorColor(int r, int g, int b) 
  {
    color_list_[kLeftActor][0] = r;
    color_list_[kLeftActor][1] = g;
    color_list_[kLeftActor][2] = b;
  }
  
  
  static void SetRightActorColor(int r, int g, int b) 
  {
    color_list_[kRightActor][0] = r;
    color_list_[kRightActor][1] = g;
    color_list_[kRightActor][2] = b;
  }
  
  static void SetBackFaceActorColr(int r, int g, int b)
  {
    color_list_[kBackFaceActor][0] = r;
    color_list_[kBackFaceActor][1] = g;
    color_list_[kBackFaceActor][2] = b;
  }
  
  static void DefaultColor()
  {
    int back_ground[] = {0, 0, 0};
    int edge[] = {0, 0, 255};
    int cell[] = {255, 255, 255};
    int actor_color[] = {92, 64, 51};
    int back_face_actor[] = {192, 192, 192};
    for(size_t i = 0; i < 3; ++i)
    {
      color_list_[kBackGround][i] = back_ground[i];
      color_list_[kCellEdge][i] = edge[i];
      color_list_[kCell][i] = cell[i];
      color_list_[kLeftActor][i] = actor_color[i];
      color_list_[kRightActor][i] = actor_color[i];
      color_list_[kBackFaceActor][i] = back_face_actor[i];
    }
    SetLightParameter();
  }
  
  static void SetLightParameter()
  {
    light.diffuse = 0.5;
    light.amibent = 0.2;
    light.specular = 0.0;
    light.specular_power = 10.0;
  }
  
  static void open_config_file()
  {
    std::ifstream fin_config("./config.txt");
    if(fin_config.fail())
    {
      VtkPropManage::DefaultColor();
    }
    else
    {
      std::string input;
      SetLightParameter();
      while(std::getline(fin_config, input))
      {
        std::string config_type, config_value;
        int flag = input.find(" ");
        config_type = input.substr(0, flag);
        config_value = input.substr(flag + 1);
        int r, g, b;
        if(strcmp(config_type.c_str(), "Background") == 0)
        {
          sscanf(config_value.c_str(), "%d %d %d", &r, &g, &b);
          SetBackground(r, g, b);
        }
        else if(strcmp(config_type.c_str(), "EdgeColor") == 0)
        {
          sscanf(config_value.c_str(), "%d %d %d", &r, &g, &b);
          SetCellEdge(r, g, b);
        }
        else if(strcmp(config_type.c_str(), "CellColor") == 0)
        {
          sscanf(config_value.c_str(), "%d %d %d", &r, &g, &b);
          SetCell(r, g, b);
        }
        else if(strcmp(config_type.c_str(), "LeftActorColor") == 0)
        {
          sscanf(config_value.c_str(), "%d %d %d", &r, &g, &b);
          SetLeftActorColor(r, g, b);
        }
        else if(strcmp(config_type.c_str(), "RightActorColor") == 0)
        {
          sscanf(config_value.c_str(), "%d %d %d", &r, &g, &b);
          SetRightActorColor(r, g, b);
        }
        else if(strcmp(config_type.c_str(), "FaceBackActorColor") == 0)
        {
          sscanf(config_value.c_str(), "%d %d %d", &r, &g, &b);
          SetBackFaceActorColr(r, g, b);
        }
      }
    }
  }
  
  static void save_config_file()
  {
    std::ofstream fout_config("./config.txt");
    fout_config << "Background " << color_list_[kBackGround][0] << " " << color_list_[kBackGround][1] 
                << " " << color_list_[kBackGround][2] << std::endl;
    fout_config << "EdgeColor " << color_list_[kCellEdge][0] << " " << color_list_[kCellEdge][1] 
                << " " << color_list_[kCellEdge][2] << std::endl;
    fout_config << "CellColor " << color_list_[kCell][0] << " " << color_list_[kCell][1] << 
                   " " << color_list_[kCell][2] << std::endl;
    fout_config << "LeftActorColor " << color_list_[kLeftActor][0] << " " << 
                   color_list_[kLeftActor][1] << " " << color_list_[kLeftActor][2] <<
                   std::endl;
    fout_config << "RightActorColor " << color_list_[kRightActor][0] << " " << 
                   color_list_[kRightActor][1] << " " << color_list_[kRightActor][2] <<
                   std::endl;
    fout_config << "FaceBackActorColor " << color_list_[kBackFaceActor][0] << " " << 
                   color_list_[kBackFaceActor][1] << " " << color_list_[kBackFaceActor][2] <<
                   std::endl;
    fout_config.close();
  }
  
protected:
  
  static int color_list_[16][3];
  static Light light;
};

//int* VtkPropManage::color_list_[16];

}
}
#endif // VTKEPROPMANAGE_H
