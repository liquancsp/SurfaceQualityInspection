#ifndef COLOROPERATE_H
#define COLOROPERATE_H


#include <vcg/space/color4.h>
#include <vcg/space/colorspace.h>

#include <MeshData.h>
#include <data_type.h>
#include <ConsoleMessage.h>

#include <string>

#if defined(_MSC_VER)
#include <memory>
#endif

#if defined(__GNUC__)
#include <tr1/memory>
#endif


namespace sqi
{
namespace color
{

template<typename OpenMeshType>
class ColorOperate 
{
  
public:
  ColorOperate(){}
  ~ColorOperate(){}
  
  static int InitStdColor(OpenMeshType& mesh)
  {
    vcg::Color4b gen_color;
    double min = 255.0;
    double max = 0.0;
    for(size_t i = 0; i <= 255; ++i)
    {
      gen_color.SetColorRamp(max, min, i);
      sqi::meshdata::Color tmp;
      tmp.R = gen_color[0];
      tmp.G = gen_color[1];
      tmp.B = gen_color[2];
      tmp.Scalar = static_cast<double>(i / 255.0);
      mesh->StdColor().push_back(tmp);
    }
    io::ConsoleMessage<std::string>::MessageText("The number of color type is : ", 
                                                 io::kInfoColor, mesh->ColorNum());
    io::ConsoleMessage<std::string>::MessageText("Finish standard color initialize.",
                                                 io::kInfoColor);
    return lq::kOK;
  }
  
  static int InitBilinearColor(OpenMeshType& mesh)
  {
    
    vcg::Color4d red(255.0, 0, 0, 0);
    vcg::Color4d green(0.0, 255.0, 0.0, 0.0);
    vcg::Color4d green_blue(128.0, 255.0, 255.0, 0.0);
    vcg::Color4d yellow(255.0, 255.0, 0.0, 0.0);
    vcg::Color4d blue(0.0, 0.0, 255.0, 0.0);
    vcg::Color4d tmp;
    sqi::meshdata::Color res_color;
    int minus_max = -128;
    int pos_max = 128;
    int i;
    mesh->StdColor().clear();
    for(i = -128; i < 0; ++i)
    {
      tmp =  (green_blue - blue) * i / (0 - minus_max) + green_blue;
      res_color.R = static_cast<unsigned char>(tmp[0]);
      res_color.G = static_cast<unsigned char>(tmp[1]);
      res_color.B = static_cast<unsigned char>(tmp[2]);
      res_color.Scalar = static_cast<double>(i / 255.0);
      mesh->StdColor().push_back(res_color);
//      tmp =  (yellow - red) * i / (0 - minus_max) + yellow;
//      res_color.R = static_cast<unsigned char>(tmp[0]);
//      res_color.G = static_cast<unsigned char>(tmp[1]);
//      res_color.B = static_cast<unsigned char>(tmp[2]);
//      res_color.Scalar = static_cast<double>(i / 255.0);
//      mesh->StdColor().push_back(res_color);
    }
    for(; i <= 128; ++i)
    {
      tmp =  (red - yellow) * i / (pos_max) + yellow;
      res_color.R = static_cast<unsigned char>(tmp[0]);
      res_color.G = static_cast<unsigned char>(tmp[1]);
      res_color.B = static_cast<unsigned char>(tmp[2]);
      res_color.Scalar = static_cast<double>(i / 255.0);
      mesh->StdColor().push_back(res_color);
//      tmp =  (blue - green_blue) * i / (pos_max) + green_blue;
//      res_color.R = static_cast<unsigned char>(tmp[0]);
//      res_color.G = static_cast<unsigned char>(tmp[1]);
//      res_color.B = static_cast<unsigned char>(tmp[2]);
//      res_color.Scalar = static_cast<double>(i / 255.0);
//      mesh->StdColor().push_back(res_color);
    }
    if(mesh->ScalarType() == sqi::meshdata::kUniform)
    {
      if(!lq::equal(mesh->MinThreshold(), 0.0f))
      {
        size_t mid = (0 + mesh->StdColor().size()) >> 1;
//        double rate = mesh->MinThreshold() / mesh->MaxE(lq::kRight);
        double rate = mesh->MinThreshold() / mesh->MaxColor(lq::kRight);
        size_t num = rate * pos_max;
        if(num > pos_max)
          num = pos_max;
        for(size_t i = 0; i <= num; ++i)
        {
          mesh->StdColor()[mid + i].R = static_cast<unsigned char>(green[0]);
          mesh->StdColor()[mid + i].G = static_cast<unsigned char>(green[1]);
          mesh->StdColor()[mid + i].B = static_cast<unsigned char>(green[2]);
          mesh->StdColor()[mid - i].R = static_cast<unsigned char>(green[0]);
          mesh->StdColor()[mid - i].G = static_cast<unsigned char>(green[1]);
          mesh->StdColor()[mid - i].B = static_cast<unsigned char>(green[2]);
        }
      }
    }
    return lq::kOK;
  }

  static int InitHSLColor(OpenMeshType& mesh)
  {
    vcg::Color4d rgb, hsl;
    sqi::meshdata::Color tmp;
    
    for(size_t i = 0; i < 256; ++i)
    {
      //if(i == 127 || i == 128)
      if(i > 124 && i <= 129)
      {
        tmp.B = 255, tmp.G = 255, tmp.R = 255;
        mesh->StdColor().push_back(tmp);
        continue;
      }
      hsl[0] = i / 360.0;
      hsl[1] = 1.0;
      hsl[2] = 0.6;
      rgb = vcg::ColorSpace<double>::HSLtoRGB(hsl);
      tmp.R = static_cast<unsigned char>(rgb[0] * 255.0);
      tmp.G = static_cast<unsigned char>(rgb[1] * 255.0);
      tmp.B = static_cast<unsigned char>(rgb[2] * 255.0);
      mesh->StdColor().push_back(tmp);
    }
    
  }
  
  static int InitClassifyColor(OpenMeshType& mesh)
  {
    sqi::meshdata::Color res_color;
    int i;
    mesh->StdColor().clear();
    for(i = 1; i <= 11; ++i)
    {
      res_color.R = static_cast<unsigned char>(classify_color[i - 1][0]);
      res_color.G = static_cast<unsigned char>(classify_color[i - 1][1]);
      res_color.B = static_cast<unsigned char>(classify_color[i - 1][2]);
      res_color.Scalar = static_cast<double>(i / 11);
      mesh->StdColor().push_back(res_color);
    }
    return lq::kOK;
  }

  //apply Linear interpolation method (y - y0) / (x - x0) = (y1 - y0) / (x1 - x0)
  //For a x value we can get a y value

  static void ColorInterpolation(double &value , vcg::Color4b& ret ,
                                 double minf = 0.0f, double maxf = 1.0f)
  {
    vcg::Color4d tmp;
    vcg::Color4d red(255, 0, 0, 0);
    vcg::Color4d white(255, 255, 255, 0);
    vcg::Color4d blue(0, 0, 255,0);
    double mid = (minf + maxf) / 2;
    if(lq::is_less_equal(value, mid))
    {
      tmp = (white * (value - minf) + blue * (mid - value)) / (mid - minf);
    }
    else
    {
      tmp = (red * (value - mid) + white * (maxf - value)) / (maxf - mid);
    }
    ret[0] = static_cast<unsigned char>(tmp[0]);
    ret[1] = static_cast<unsigned char>(tmp[1]);
    ret[2] = static_cast<unsigned char>(tmp[2]);
  }
  
  static void UpdateColor(OpenMeshType &m,
                          double minf, double maxf)
  {
    io::ConsoleMessage<std::string>::MessageText("Begin Update color...", 
                                                 io::kInfoColor);
    for(sqi::meshdata::Mesh::FaceIterator fi = m->M(1).face.begin();
        fi != m->M(1).face.end(); ++fi)
    {
      
//      if(lq::is_less((*fi).Q(), 0.0))
//      {
//        (*fi).C() = vcg::Color4b::Red;
//      }
//      else
//      {
//        (*fi).C() = vcg::Color4b::Blue;
//      }
      (*fi).C().SetColorRamp(maxf, minf, (*fi).Q());
      //ColorInterpolation((*fi).Q(), (*fi).C(), minf, maxf);
    }
    std::cout << "[#info]Finish Update color." << std::endl;
  }
  static void UpdateHSLColor(OpenMeshType& m)
  {
    double max_error = m->MaxE();
    sqi::meshdata::Mesh::PerFaceAttributeHandle<double> value_convex_list = 
        vcg::tri::Allocator<sqi::meshdata::Mesh>::GetPerFaceAttribute<double>(m->M(lq::kRight),std::string("ValueConvex"));
    sqi::meshdata::Mesh::PerMeshAttributeHandle<double> max_convex = 
        vcg::tri::Allocator<sqi::meshdata::Mesh>::GetPerMeshAttribute<double>(m->M(lq::kRight),std::string("MaxValueConvex"));
    sqi::meshdata::Mesh::PerMeshAttributeHandle<double> min_convex = 
        vcg::tri::Allocator<sqi::meshdata::Mesh>::GetPerMeshAttribute<double>(m->M(lq::kRight),std::string("MinValueConvex"));
    for(meshdata::FaceIterator fi = m->FBeginIter(lq::kRight);
        fi != m->FEndIter(lq::kRight); ++fi)
    {
      vcg::Color4d rgb, hsl;
      double k = 256.0 / (360.0 * (max_convex() - min_convex()));
      hsl[0] = k * value_convex_list[*fi] - k * min_convex();
      hsl[1] = 1.0;
      hsl[2] = 1.0 - 0.5 * ((*fi).Q() / max_error);
      rgb = vcg::ColorSpace<double>::HSLtoRGB(hsl);
      (*fi).C()[0] = static_cast<unsigned char>(rgb[0] * 255.0);
      (*fi).C()[1] = static_cast<unsigned char>(rgb[1] * 255.0);
      (*fi).C()[2] = static_cast<unsigned char>(rgb[2] * 255.0);
    }
  }

  static void UpdateColor(OpenMeshType &m)
  {
    io::ConsoleMessage<std::string>::MessageText("Begin Update color...", 
                                                 io::kInfoColor);
    io::ConsoleMessage<std::string>::MessageText("Min triangle error is :", 
                                                 io::kInfoColor, m->MinE());
    io::ConsoleMessage<std::string>::MessageText("Max triangle error is :",
                                                 io::kInfoColor, m->MaxE());
    double max_error, min_error;
    if(m->ScalarType() == meshdata::kIndependent)
    {
      max_error = m->MaxE();
      min_error = m->MinE();
    }
    else if(m->ScalarType() == meshdata::kUniform)
    {
      max_error = m->MaxColor();
      min_error = m->MinColor();
    }
    double times = 255 / (max_error - min_error);
    for(meshdata::FaceIterator fi = m->FBeginIter(lq::kRight);
        fi != m->FEndIter(lq::kRight); ++fi)
    {
      size_t idx;
      if(lq::is_less_equal((*fi).Q(), min_error))
      {
        idx = 0;
      }
      else if(lq::is_larger_equal((*fi).Q(), max_error))
      {
        idx = m->ColorNum() - 1;
      }
      else 
      {
        idx = static_cast<size_t>(times * ((*fi).Q() - min_error));
        assert(idx < m->ColorNum());
      }
      (*fi).C()[0] = m->StdColor()[idx].R;
      (*fi).C()[1] = m->StdColor()[idx].G;
      (*fi).C()[2] = m->StdColor()[idx].B;
    }
  }

  static void UpdateClassifyColor(OpenMeshType& m, lq::WindowFlag win = lq::kLeft)
  {
    io::ConsoleMessage<std::string>::MessageText("Begin Update Classify color...", 
                                                 io::kInfoColor);
    std::ofstream fout("vertex-color.txt");
    for(meshdata::VertexIterator vi = m->VBeginIter(win);
        vi != m->VEndIter(win); ++vi)
    {
      fout << vi->Q()  << std::endl;
      (*vi).C()[0] = m->StdColor()[static_cast<int>(vi->Q() - 1)].R;
      (*vi).C()[1] = m->StdColor()[static_cast<int>(vi->Q() - 1)].G;
      (*vi).C()[2] = m->StdColor()[static_cast<int>(vi->Q() - 1)].B;
    }
    fout.close();
  }

  static void UpdateRainbowColor(OpenMeshType& m, lq::WindowFlag win = lq::kRight)
  {
    double M;
    if(lq::is_larger(fabs(m->MaxE(win)), fabs(m->MinE(win))))
      M = fabs(m->MaxE(win));
    else
      M = fabs(m->MinE(win));
    double pos_max = m->MaxColor(win);
    double minus_max = m->MinColor(win);
    double zero_threshold = m->MinThreshold();
    if(m->ScalarType() == meshdata::kIndependent)
    {
      pos_max = M;
      minus_max = 0 - M;
      zero_threshold = 0.0f;
      cout << "independent " << endl;
    }
    std::cout << "color max " << pos_max << " " << " color min : " << minus_max << std::endl;
    std::cout << "min threshold : " << m->MinThreshold() << std::endl;
    vcg::Color4d red(255.0, 0, 0, 0);
    vcg::Color4d green(0.0, 255.0, 0.0, 0.0);
    vcg::Color4d green_blue(128.0, 255.0, 255.0, 0.0);
    vcg::Color4d yellow(255.0, 255.0, 0.0, 0.0);
    vcg::Color4d blue(0.0, 0.0, 255.0, 0.0);
    vcg::Color4d tmp;
    for(sqi::meshdata::Mesh::FaceIterator fi = m->M(win).face.begin();
        fi != m->M(win).face.end(); ++fi)
    {
      if(/*lq::is_zero((*fi).Q(), sqi::meshdata::EPS_E6) || */
         lq::is_less_equal(fabs((*fi).Q()), zero_threshold))
      {
        if(lq::is_zero(zero_threshold))
          tmp = green_blue;
        else 
          tmp = green;
      }
      else if(lq::is_less_equal((*fi).Q(), minus_max))
        tmp = blue;
      else if(lq::is_larger_equal((*fi).Q(), pos_max))
        tmp = red;
      else if(lq::is_less((*fi).Q(), sqi::meshdata::EPS_E6))
        tmp =  (blue - green_blue) * ((*fi).Q() / minus_max) + green_blue;
      else
        tmp =  (red - yellow) * ((*fi).Q() / (pos_max)) + yellow;
      (*fi).C()[0] = static_cast<unsigned char>(tmp[0]);
      (*fi).C()[1] = static_cast<unsigned char>(tmp[1]);
      (*fi).C()[2] = static_cast<unsigned char>(tmp[2]);
    }
  }

private:
  static double classify_color[11][3];
};
template<typename T>
double ColorOperate<T>::classify_color[11][3] = 
{{245, 245, 220},
{255, 0, 0},
{0, 0, 255},
{255, 255, 0},
{0, 139, 139},
{255, 228, 181},
{0, 100, 0},
{238, 130, 238},
{255, 165, 0},
{255, 20, 147},
{79, 79, 79}};

}
}

#endif // COLOROPERATE_H
