#include <Algorithms/ZernikeCoefficient.h>
#include <vtkMath.h>
#include <vtkIdList.h>

#include <MeshData.h>
#include <data_type.h>

#include <vector>

namespace sqi {
namespace alg {
using namespace std;
using namespace lq;
ZernikeCoefficient::ZernikeCoefficient()
{
  timer_ = vtkSmartPointer<vtkTimerLog>::New();
}


ZernikeCoefficient::~ZernikeCoefficient()
{
  
}

void ZernikeCoefficient::CalZernikeCoeff(const std::vector<std::vector<double> > &height_map, 
                                         const double bound[], 
                                         std::vector<std::vector<double> > &zernike_coeff)
{
  double left_bottom[3] = {bound[0], bound[2], bound[4]};
  double right_top[3] = {bound[1], bound[3], bound[5]};
  float diagonal_len = sqrt(vtkMath::Distance2BetweenPoints(left_bottom, right_top));
  float rect_edge = (2.5 * diagonal_len) / 100.0f;
  if(!lq::is_zero(mesh_data_->RadiusRatio(), 1e-6))
    rect_edge /= mesh_data_->RadiusRatio();
  float radius = rect_edge / 2.0f;
  float e = rect_edge / 16;
  vector<PolarCoord> grid_polar;//Obtain polar coordinate of 16 x 16 grid
  for(int i = 0; i < 256; ++i)
  {
    int k = i / 16 - 7;
    int j = i % 16 - 7;
    vcg::Point3d center(0.0f, 0.0f, 0.0f);
    vcg::Point3d p(0.0f, 0.0f, 0.0f);
    vcg::Point3d dir1(0.0, 1.0f, 0.0f);
    vcg::Point3d dir2(1.0f, 0.0f, 0.0f);
    double y_dist = (static_cast<double>(k) - 0.5) * e;
    double x_dist = (static_cast<double>(j) - 0.5) * e;
    p += dir1 * y_dist;
    p += dir2 * x_dist;
    
    double rho = vcg::Distance<double>(center, p);
    double theta = 0.0f;
    if(lq::is_larger(rho, radius))
    {
      grid_polar.push_back(PolarCoord());
      continue;
    }
    if(k > 0)
      theta = atan(y_dist / x_dist);
    else if(j <= 0)
      theta = atan(y_dist / x_dist) - PI;
    else if(j > 0)
      theta = atan(y_dist / x_dist) + PI;
    grid_polar.push_back(PolarCoord(rho, theta));
  }
  
  vector<func_ptr> radial_poly;
  zernike_coeff.resize(height_map.size());
  PushRadialPolyFunc(radial_poly);
  for(int i = 0; i < height_map.size(); ++i)
  {
    int cnt = 0;
    //25 Zerniker polynomials 
    for(int p = 0; p <= 8; ++p)
      for(int q = 0; q <= p; ++q)
      {
        double real = 0.0f;
        double imaginary = 0.0f;
        if(((p - q) & 1) == 0)
        {
          for(int j = 0; j < height_map[i].size(); ++j)
          {
            if(lq::equal(height_map[i][j], -1.0f))
              continue;
            double rho = grid_polar[j].rho;
            double theta = grid_polar[j].theta;
            double poly = radial_poly[cnt](rho);
            real += poly * cos(static_cast<double>(q) * theta) * height_map[i][j];
            imaginary += poly * sin(static_cast<double>(q) * theta) * height_map[i][j];
          }
          double coeff = (static_cast<double>(p) + 1.0f) / PI;
          real *= coeff;
          imaginary *= coeff;
          double z = sqrt(real * real + imaginary * imaginary);
          zernike_coeff[i].push_back(z);
          ++cnt;
        }
      }
  }
  SurfaceDescriptor::WriteFile(zernike_coeff, "ZernikeCoeff.txt");
}

void ZernikeCoefficient::CalZernikeCoeff(const std::vector<std::vector<double> > &height_map, 
                                         const double rect_edge, 
                                         std::vector<std::vector<double> > &zernike_coeff)
{
  float e = rect_edge / 16;
  float radius = rect_edge / 2.0f;
  vector<PolarCoord> grid_polar;//Obtain polar coordinate of 16 x 16 grid
  for(int i = 0; i < 256; ++i)
  {
    int k = i / 16 - 7;
    int j = i % 16 - 7;
    vcg::Point3d center(0.0f, 0.0f, 0.0f);
    vcg::Point3d p(0.0f, 0.0f, 0.0f);
    vcg::Point3d dir1(0.0, 1.0f, 0.0f);
    vcg::Point3d dir2(1.0f, 0.0f, 0.0f);
    double y_dist = (static_cast<double>(k) - 0.5) * e;
    double x_dist = (static_cast<double>(j) - 0.5) * e;
    p += dir1 * y_dist;
    p += dir2 * x_dist;
    
    double rho = vcg::Distance<double>(center, p);
    double theta = 0.0f;
    if(lq::is_larger(rho, radius))
    {
      grid_polar.push_back(PolarCoord());
      continue;
    }
    if(k > 0)
      theta = atan(y_dist / x_dist);
    else if(j <= 0)
      theta = atan(y_dist / x_dist) - PI;
    else if(j > 0)
      theta = atan(y_dist / x_dist) + PI;
    grid_polar.push_back(PolarCoord(rho, theta));
  }
  
  vector<func_ptr> radial_poly;
  zernike_coeff.resize(height_map.size());
  PushRadialPolyFunc(radial_poly);
  for(int i = 0; i < height_map.size(); ++i)
  {
    int cnt = 0;
    //25 Zerniker polynomials 
    for(int p = 0; p <= 8; ++p)
      for(int q = 0; q <= p; ++q)
      {
        double real = 0.0f;
        double imaginary = 0.0f;
        if(((p - q) & 1) == 0)
        {
          for(int j = 0; j < height_map[i].size(); ++j)
          {
            if(lq::equal(height_map[i][j], -1.0f))
              continue;
            double rho = grid_polar[j].rho;
            double theta = grid_polar[j].theta;
            double poly = radial_poly[cnt](rho);
            real += poly * cos(static_cast<double>(q) * theta) * height_map[i][j];
            imaginary += poly * sin(static_cast<double>(q) * theta) * height_map[i][j];
          }
          double coeff = (static_cast<double>(p) + 1.0f) / PI;
          real *= coeff;
          imaginary *= coeff;
          double z = sqrt(real * real + imaginary * imaginary);
          zernike_coeff[i].push_back(z);
          ++cnt;
        }
      }
  }
  SurfaceDescriptor::WriteFile(zernike_coeff, "ZernikeCoeff.txt");
}

void ZernikeCoefficient::CalZernikeCoeffWithGaussianWeight(meshdata::VertexIterator vi_begin,
                                                           vtkSmartPointer<vtkKdTreePointLocator>& kd_tree,
                                                           const std::vector<std::vector<double> > &height_map, 
                                                           const double bound[6], double neigh_radius,
                                                           std::vector<std::vector<double> > &zer_coeff_gauss)
{
  
  if(kd_tree == NULL)
    return;
  cout << "[#Info]Search radius : " << neigh_radius << endl;
  timer_->StartTimer();
  vector<vector<double> > zernike_coeff(height_map.size());
  CalZernikeCoeff(height_map, bound, zernike_coeff);
  zer_coeff_gauss.resize(height_map.size(), vector<double>(25, 0.0f));
  for(int i = 0; i < height_map.size(); ++i)
  {
    vtkSmartPointer<vtkIdList> idx_list = 
        vtkSmartPointer<vtkIdList>::New();
    meshdata::VertexIterator vi = vi_begin + i;   
    double ref_point[3] = {vi->cP()[0], vi->cP()[1], vi->cP()[2]};
    kd_tree->FindPointsWithinRadius(neigh_radius, ref_point, idx_list);
    double denominator = 0.0f;
    double numerator_coeff = 0.0f;
    meshdata::VertexIterator vi2;
    for(size_t j = 0; j < idx_list->GetNumberOfIds(); ++j)
    {
      int v2_idx = idx_list->GetId(j);
      vi2 = vi_begin + v2_idx;
      double dist_square = vcg::SquaredDistance<float>(vi->P(), vi2->P());
      numerator_coeff = -1.0f * (dist_square / (2.0f * neigh_radius * neigh_radius));
      numerator_coeff = exp(numerator_coeff);
      for(int k = 0; k < zernike_coeff[v2_idx].size(); ++k)
        zer_coeff_gauss[i][k] += zernike_coeff[v2_idx][k] * numerator_coeff;
      denominator += numerator_coeff;
    }
    DivVector<double>(zer_coeff_gauss[i], denominator);
  }
  SurfaceDescriptor::WriteFile(zer_coeff_gauss, "ZernikeCoeffGauss.txt");
  timer_->StopTimer();
  cout << "[#Info]Zerniker coeff with gaussian tree cost vtk timer's time is : " << timer_->GetElapsedTime() << endl;
}


////calculate zernike radial polynomial
//double  CalRadialPolyP0Q0(double rho)
//{
//  return 1;
//}

//double  CalRadialPolyP1Q1(double rho)
//{
//  return rho;
//}

//double  CalRadialPolyP2Q0(double rho)
//{
//  return (2.0f * rho * rho - 1.0f);
//}

//double  CalRadialPolyP2Q2(double rho)
//{
//  return rho * rho;
//}
//double  CalRadialPolyP3Q1(double rho)
//{
//  double rho_square = rho * rho;
//  return (rho * (3.0 * rho_square - 1.0));
//}
//double  CalRadialPolyP3Q3(double rho)
//{
//  return rho * rho * rho;
//}

//double  CalRadialPolyP4Q0(double rho)
//{
//  double rho_square = rho * rho;
//  return (6.0 * rho_square * (rho_square - 1.0));
//}
//double  CalRadialPolyP4Q2(double rho)
//{
//  double rho_square = rho * rho;
//  return rho_square * (4.0 * rho_square - 3.0f);
//}
//double  CalRadialPolyP4Q4(double rho)
//{
//  return std::pow(rho, 4.0f);
//}
//double  CalRadialPolyP5Q1(double rho)
//{
//  double rho_cube = std::pow(rho, 3);
//  return (2.0 * rho_cube * (5.0 * rho * rho - 6.8) + 3.0 * rho);
//}
//double  CalRadialPolyP5Q3(double rho)
//{
//  double rho_square = rho * rho;
//  return (rho_square * rho * (5.0f * rho_square - 4.0f));
//}
//double  CalRadialPolyP5Q5(double rho)
//{
//  return std::pow(rho, 5.0f);
//}
//double  CalRadialPolyP6Q0(double rho)
//{
//  double rho_square = rho * rho;
//  return 10.0 * rho_square * rho_square * (2.0f * rho_square - 3.0f) + 
//      12.0f * rho_square - 1.0f;
//}

//double  CalRadialPolyP6Q2(double rho)

//{
//  double rho_square = rho * rho;
//  return (5.0f * rho_square * rho_square * ( 3.0f * rho_square) + 6 * rho_square);
//}
//double  CalRadialPolyP6Q4(double rho)
//{
//  double rho_square = rho * rho;
//  return (rho_square * rho_square * (6 * rho_square - 5.0f));
//}
//double  CalRadialPolyP6Q6(double rho)
//{
//  return std::pow(rho, 6.0f);
//}
//double  CalRadialPolyP7Q1(double rho)
//{
//  double rho_square = rho * rho;
//  return (5.0f * rho_square * rho * (7.0f * rho_square * rho_square - 
//                                      12.0f * rho_square + 6.0f) - 4.0f * rho);
//}
//double  CalRadialPolyP7Q3(double rho)
//{
//  double rho_square = rho * rho;
//  return (rho_square * rho * (10 - 30 * rho_square + 21 * rho_square * rho_square));
//}
//double  CalRadialPolyP7Q5(double rho)
//{
//  double rho_five = std::pow(rho, 5);
//  return rho_five * (7 * rho * rho - 6);
//}

//double  CalRadialPolyP7Q7(double rho)
//{
//  return std::pow(rho, 7);
//}

//double  CalRadialPolyP8Q0(double rho)
//{
//  double rho_square = rho * rho;
//  double rho_four = rho_square * rho_square;
//  return (1 - 20 * rho_square + 10 * rho_four * (9 - 14 * rho_square + 7 * rho_four));
//}

//double  CalRadialPolyP8Q2(double rho)
//{
//  double rho_square = rho * rho;
//  double rho_four = rho_square * rho_square;
//  return (rho_four * (60 - 105 * rho_square + 56 * rho_four) - 10 * rho_square);
//}

//double  CalRadialPolyP8Q4(double rho)
//{
//  double rho_square = rho * rho;
//  double rho_four = rho_square * rho_square;
//  return (rho_four * (15 - 42 * rho_square + 28 * rho_four));
//}

//double  CalRadialPolyP8Q6(double rho)
//{
//  double rho_six = pow(rho, 6);
//  return (rho_six * (8 * rho * rho - 7));
//}

//double  CalRadialPolyP8Q8(double rho)
//{
//  return std::pow(rho, 8);
//}

void ZernikeCoefficient::PushRadialPolyFunc(std::vector<func_ptr> &radial_poly)
{
  SurfaceDescriptor::PushRadialPolyFunc(radial_poly);
//  radial_poly.push_back(CalRadialPolyP0Q0);
//  radial_poly.push_back(CalRadialPolyP1Q1);
//  radial_poly.push_back(CalRadialPolyP2Q0);
//  radial_poly.push_back(CalRadialPolyP2Q2);
//  radial_poly.push_back(CalRadialPolyP3Q1);
//  radial_poly.push_back(CalRadialPolyP3Q3);
//  radial_poly.push_back(CalRadialPolyP4Q0);
//  radial_poly.push_back(CalRadialPolyP4Q2);
//  radial_poly.push_back(CalRadialPolyP4Q4);
//  radial_poly.push_back(CalRadialPolyP5Q1);
//  radial_poly.push_back(CalRadialPolyP5Q3);
//  radial_poly.push_back(CalRadialPolyP5Q5);
//  radial_poly.push_back(CalRadialPolyP6Q0);
//  radial_poly.push_back(CalRadialPolyP6Q2);
//  radial_poly.push_back(CalRadialPolyP6Q4);
//  radial_poly.push_back(CalRadialPolyP6Q6);
//  radial_poly.push_back(CalRadialPolyP7Q1);
//  radial_poly.push_back(CalRadialPolyP7Q3);
//  radial_poly.push_back(CalRadialPolyP7Q5);
//  radial_poly.push_back(CalRadialPolyP7Q7);
//  radial_poly.push_back(CalRadialPolyP8Q0);
//  radial_poly.push_back(CalRadialPolyP8Q2);
//  radial_poly.push_back(CalRadialPolyP8Q4);
//  radial_poly.push_back(CalRadialPolyP8Q6);
//  radial_poly.push_back(CalRadialPolyP8Q8);
}


}

}
