#include <SurfaceDescriptor.h>
#include <MeshData.h>
#include <CurvatureAlg.h>

#include <vtkIdList.h>

#include <iostream>
#include <fstream>
namespace sqi {
namespace alg {
using namespace std;
SurfaceDescriptor::SurfaceDescriptor() : timer_(vtkSmartPointer<vtkTimerLog>::New())
{
  
}

SurfaceDescriptor::~SurfaceDescriptor()
{
  
}

void SurfaceDescriptor::WriteFile(const std::vector<std::vector<double> > &height_map, 
                                  const std::string &file_name) const
{
  cout << "[#Info]Begin to export file." << endl;
  ofstream fout(file_name.c_str());
  for(int i = 0; i < height_map.size(); ++i)
  {
    fout << height_map[i].size();
    for(int j = 0; j < height_map[i].size(); ++j)
    {
      fout << " " << height_map[i][j];
    }
    fout << endl;
  }
  fout.close();
  cout << "[#Info]Export successful." << endl;
}

void SurfaceDescriptor::CalGridFlag(std::vector<bool> &flag, double e, double radius)
{
  vcg::Point3f center(0, 0, 0);
  vcg::Point3f y_dirt(0, 1, 0);
  vcg::Point3f x_dirt(1, 0, 0);
  flag.resize(256);
  for(int i = 0; i < flag.size(); ++i)
  {
    int k = i / 16 - 7;
    int j = i % 16 - 7;
    vcg::Point3f p(0, 0, 0);
    double y_dist = (static_cast<double>(k) - 0.5) * e;
    double x_dist = (static_cast<double>(j) - 0.5) * e;
    p += y_dirt * y_dist;
    p += x_dirt * x_dist;
    float dist = vcg::Distance<float>(center, p);
    if(lq::is_larger(dist, radius))
      flag[i] = false;
    else 
      flag[i] = true;
  }
}

void SurfaceDescriptor::InitializeParameters(meshdata::MeshDataPtr &mesh_data, 
                                             const double bound[], double &radius)
{
  vcg::Point3f left_bottom(bound[0], bound[2], bound[4]);
  vcg::Point3f right_top(bound[1], bound[3], bound[5]);
  double diagonal_len = vcg::Distance(left_bottom, right_top);
  double rect_edge = (2.5 * diagonal_len) / 100.0f;
  cout << "[#Info]Rect edge : " << rect_edge << endl;
  if(!lq::is_zero(mesh_data->RadiusRatio(), 1e-6))
    rect_edge /= mesh_data->RadiusRatio();
  radius = rect_edge / 2.0f;
}

void SurfaceDescriptor::InitializeParameters(meshdata::MeshDataPtr &mesh_data, 
                                             std::vector<func_ptr> &radial_poly, 
                                             const double bound[], double &radius)
{
//  vcg::Point3f left_bottom(bound[0], bound[2], bound[4]);
//  vcg::Point3f right_top(bound[1], bound[3], bound[5]);
//  double diagonal_len = vcg::Distance(left_bottom, right_top);
//  double rect_edge = (2.5 * diagonal_len) / 100.0f;
//  if(!lq::is_zero(mesh_data->RadiusRatio(), 1e-6))
//    rect_edge /= mesh_data->RadiusRatio();
//  radius = rect_edge / 2.0f;
  InitializeParameters(mesh_data, bound, radius);
  SurfaceDescriptor::PushRadialPolyFunc(radial_poly);
}

void SurfaceDescriptor::InitializeParameters(meshdata::MeshDataPtr &mesh_data, 
                                             vtkSmartPointer<lq::vtk_model_data> &data_set, 
                                             lq::WindowFlag win, double bound[])
{
  sqi::meshdata::MeshHandleB is_have_convex = 
      vcg::tri::Allocator<sqi::meshdata::Mesh>::GetPerMeshAttribute<bool>(mesh_data->M(win), 
                                                                          std::string("IsHaveConvex"));
  if(!is_have_convex())
  {
    CurvatureAlg<sqi::meshdata::Mesh>::MeanAndGaussian(mesh_data->M(win));
    is_have_convex() = true;
  }
  if(!mesh_data->IsHasPrinCur())
  {
    mesh_data->IsHasPrinCur() = true;
    CurvatureAlg<meshdata::Mesh>::PrincipalCurvatureAndDirection(mesh_data->M(win));
    CurvatureAlg<meshdata::Mesh>::PrincipalCurWithDirection(mesh_data->M(win));
  }
  data_set->BuildKdTree(win);
  data_set->PD(win)->GetBounds(bound);
}

int SurfaceDescriptor::MapCurvatureToBin(double cur, int div)
{
  assert(div != 0);
  double val = 0.0f;
  if(!lq::is_zero(cur, sqi::meshdata::EPS_E8))
    val = (cur / fabs(cur)) * (1 - 1 / (1 + fabs(cur)));
  
  double delta = 2.0f / static_cast<double>(div);
  for(int i = 1; i <= div; ++i)
  {
    double left, right;
    left = (i == 1) ? -1.0f : (-1.0f + delta * (i - 1));
    right = -1.0f + i * delta;
    if(val >= left && val <= right)
      return i - 1;
  }
  return div - 1;
}

void SurfaceDescriptor::CalZernikerCoeff(meshdata::MeshDataPtr &mesh_data, 
                                         const std::vector<double> &surface_feature, 
                                         const vtkSmartPointer<vtkIdList>& idx_list,
                                         std::vector<func_ptr>& radial_poly,
                                         int i, lq::WindowFlag win,
                                         std::vector<vector<double> > &zerniker_coeff,
                                         bool is_diff)
{
  //calculate the zerniker coefficient on i'th vertex
  int cnt = 0;
  sqi::meshdata::VertexIterator vi = mesh_data->VBeginIter(win) + i;
  for(int p = 0; p <= 8; ++p)
    for(int q = 0; q <= p; ++q)
    {
      double real = 0.0f;
      double imaginary = 0.0f;
      if(((p - q) & 1) == 0)
      {
        for(int j = 0; j < idx_list->GetNumberOfIds(); ++j)
        {
          float rho = 0.0f;
          float theta = 0.0f;
          sqi::meshdata::VertexIterator tmp_vi = mesh_data->VBeginIter(win) +
              idx_list->GetId(j);
          vcg::Point3f p = tmp_vi->cP() - vi->cP();
          double dist = p.Norm();
          p.Normalize();
          double dot_alpha = p.dot(vi->PD1()), dot_beta = p.dot(vi->PD2()),
              dot_gama = p.dot(vi->N());
          if(lq::is_larger_equal(fabs(dot_gama), 1.0f))
            continue;
          if(dot_alpha > 1.0f)
            dot_alpha = 1.0f;
          else if(dot_alpha < -1.0f)
            dot_alpha = -1.0f;
          double gama = 0.0f;
          double eps = 1e-16;
          if(dot_gama < 0)
            gama = lq::PI - acos(dot_gama);
          else 
            gama = acos(dot_gama);
          double alpha = 0.0f;
          if(dot_alpha < 0.0f)
            alpha = lq::PI - acos(dot_alpha);
          else 
            alpha = acos(dot_alpha);
          if(alpha > lq::PI / 2.0f)
            alpha = lq::PI / 2.0f;
          rho = dist * sin(gama);
          assert(sin(gama) != 0.0f);
          double ratio = cos(alpha) / sin(gama);
          if(ratio > 1.0f)
            ratio = 1.0f;
//          if(!(ratio >= 0 && ratio <= 1.0 + 1e-5))
//          {
//            cout << "i : " << i <<  " dot_alpha : " << dot_alpha <<
//                    " dot_gama : " << dot_gama <<  " ratio : " << std::setprecision(10) << ratio << endl;
//            cout << "gama : " << std::setprecision(10) << (180.0f * gama) / lq::PI
//                 << " alpha : " << (180.0f * alpha) / lq::PI << endl;
//            cout << "cos(alpha) : " << cos(alpha) << " sin(gama) : " << sin(gama) << endl;
//            cout << "alpha : " << std::setprecision(10) << alpha << " PI/2 : " << lq::PI / 2.0f << endl;
//          }
          assert(ratio >= 0 && ratio <= 1.0 + 1e-5);
          double convert_ratio = acos(ratio);
          if(lq::is_larger(dot_alpha, eps))
            theta = convert_ratio;
          else if(lq::is_less(dot_alpha, eps))
          {
            if(lq::is_larger_equal(dot_beta, eps))
              theta = lq::PI / 2.0f + convert_ratio;
            else 
              theta = convert_ratio - lq::PI;
          }
          else if(lq::is_zero(dot_alpha, eps))
          {
            if(lq::is_larger(dot_beta, eps))
              theta = lq::PI / 2.0f;
            else 
              theta = -lq::PI / 2.0f;
          }
          double poly = radial_poly[cnt](rho);
          real += poly * cos(static_cast<double>(q) * theta) * surface_feature[j];
          imaginary += poly * sin(static_cast<double>(q) * theta) * surface_feature[j];
        }
        double coeff = (static_cast<double>(p) + 1.0f) / lq::PI;
        real *= coeff;
        imaginary *= coeff;
        double z = sqrt(real * real + imaginary * imaginary);
        if(is_diff)
          zerniker_coeff[i][cnt] -= z;
        else 
          zerniker_coeff[i].push_back(z);
        ++cnt;
      }
    }
}

void SurfaceDescriptor::CalZernikerCoeff(meshdata::Mesh &mesh, 
                                         const std::vector<double> &surface_feature, 
                                         const vtkSmartPointer<vtkIdList> &idx_list, 
                                         std::vector<func_ptr> &radial_poly, int i, 
                                         std::vector<std::vector<double> > &zerniker_coeff,
                                         bool is_diff)
{
  //calculate the zerniker coefficient on i'th vertex
  int cnt = 0;
  sqi::meshdata::VertexIterator vi = mesh.vert.begin() + i;
  for(int p = 0; p <= 8; ++p)
    for(int q = 0; q <= p; ++q)
    {
      double real = 0.0f;
      double imaginary = 0.0f;
      if(((p - q) & 1) == 0)
      {
        for(int j = 0; j < idx_list->GetNumberOfIds(); ++j)
        {
          float rho = 0.0f;
          float theta = 0.0f;
          sqi::meshdata::VertexIterator tmp_vi = mesh.vert.begin() +
              idx_list->GetId(j);
          vcg::Point3f p = tmp_vi->cP() - vi->cP();
          double dist = p.Norm();
          p.Normalize();
          double dot_alpha = p.dot(vi->PD1()), dot_beta = p.dot(vi->PD2()),
              dot_gama = p.dot(vi->N());
          if(lq::is_larger_equal(fabs(dot_gama), 1.0f))
            continue;
          if(dot_alpha > 1.0f)
            dot_alpha = 1.0f;
          else if(dot_alpha < -1.0f)
            dot_alpha = -1.0f;
          double gama = 0.0f;
          double eps = 1e-16;
          if(dot_gama < 0)
            gama = lq::PI - acos(dot_gama);
          else 
            gama = acos(dot_gama);
          double alpha = 0.0f;
          if(dot_alpha < 0.0f)
            alpha = lq::PI - acos(dot_alpha);
          else 
            alpha = acos(dot_alpha);
          if(alpha > lq::PI / 2.0f)
            alpha = lq::PI / 2.0f;
          rho = dist * sin(gama);
          assert(sin(gama) != 0.0f);
          double ratio = cos(alpha) / sin(gama);
          if(ratio > 1.0f)
            ratio = 1.0f;
//          if(!(ratio >= 0 && ratio <= 1.0 + 1e-5))
//          {
//            cout << "i : " << i <<  " dot_alpha : " << dot_alpha <<
//                    " dot_gama : " << dot_gama <<  " ratio : " << std::setprecision(10) << ratio << endl;
//            cout << "gama : " << std::setprecision(10) << (180.0f * gama) / lq::PI
//                 << " alpha : " << (180.0f * alpha) / lq::PI << endl;
//            cout << "cos(alpha) : " << cos(alpha) << " sin(gama) : " << sin(gama) << endl;
//            cout << "alpha : " << std::setprecision(10) << alpha << " PI/2 : " << lq::PI / 2.0f << endl;
//          }
          assert(ratio >= 0 && ratio <= 1.0 + 1e-5);
          double convert_ratio = acos(ratio);
          if(lq::is_larger(dot_alpha, eps))
            theta = convert_ratio;
          else if(lq::is_less(dot_alpha, eps))
          {
            if(lq::is_larger_equal(dot_beta, eps))
              theta = lq::PI / 2.0f + convert_ratio;
            else 
              theta = convert_ratio - lq::PI;
          }
          else if(lq::is_zero(dot_alpha, eps))
          {
            if(lq::is_larger(dot_beta, eps))
              theta = lq::PI / 2.0f;
            else 
              theta = -lq::PI / 2.0f;
          }
          double poly = radial_poly[cnt](rho);
          real += poly * cos(static_cast<double>(q) * theta) * surface_feature[j];
          imaginary += poly * sin(static_cast<double>(q) * theta) * surface_feature[j];
        }
        double coeff = (static_cast<double>(p) + 1.0f) / lq::PI;
        real *= coeff;
        imaginary *= coeff;
        double z = sqrt(real * real + imaginary * imaginary);
        if(is_diff)
          zerniker_coeff[i][cnt] -= z;
        else 
          zerniker_coeff[i].push_back(z);
        ++cnt;
      }
    }
}

void SurfaceDescriptor::CalGaussianWeightZernikeCoeff(meshdata::MeshDataPtr &mesh_data, 
                                                      vtkSmartPointer<vtkKdTreePointLocator> &kd_tree,
                                                      const std::vector<std::vector<double> > &zerniker_coeff, 
                                                      std::vector<std::vector<double> > &zer_coeff_gauss, 
                                                      double radius, lq::WindowFlag win)
{
  zer_coeff_gauss.resize(mesh_data->VNum(win));
  for(int i = 0; i < mesh_data->VNum(win); ++i)
  {
    vtkSmartPointer<vtkIdList> idx_list = 
        vtkSmartPointer<vtkIdList>::New();
    meshdata::VertexIterator vi = mesh_data->VBeginIter(win) + i;
    double ref_point[3] = {vi->cP()[0], vi->cP()[1], vi->cP()[2]};
    kd_tree->FindPointsWithinRadius(radius, ref_point, idx_list);
    double denominator = 0.0f;
    double numerator_coeff = 0.0f;
    meshdata::VertexIterator vi2;
    if(idx_list->GetNumberOfIds() == 1)
    {
      zer_coeff_gauss[i] = zerniker_coeff[i];
      continue;
    }
    zer_coeff_gauss[i].resize(zerniker_coeff[i].size(), 0.0f);
    for(size_t j = 0; j < idx_list->GetNumberOfIds(); ++j)
    {
      int v2_idx = idx_list->GetId(j);
      vi2 = mesh_data->VBeginIter(win) + v2_idx;
      double dist_square = vcg::SquaredDistance<float>(vi->P(), vi2->P());
      numerator_coeff = -1.0f * (dist_square / (2.0f * radius * radius));
      numerator_coeff = exp(numerator_coeff);
      
      for(int k = 0; k < zerniker_coeff[v2_idx].size(); ++k)
        zer_coeff_gauss[i][k] += zerniker_coeff[v2_idx][k] * numerator_coeff;
      denominator += numerator_coeff;
    }
    DivVector<double>(zer_coeff_gauss[i], denominator);
  }
}

void SurfaceDescriptor::CalGaussianWeightZernikeCoeff(meshdata::Mesh &mesh, 
                                                      vtkSmartPointer<vtkKdTreePointLocator> &kd_tree, 
                                                      const std::vector<std::vector<double> > &zerniker_coeff, 
                                                      std::vector<std::vector<double> > &zer_coeff_gauss, 
                                                      double radius, bool is_diff)
{
  int n = mesh.vert.size();
  if(!is_diff)
    zer_coeff_gauss.resize(mesh.vert.size());
  for(int i = 0; i < n; ++i)
  {
    vtkSmartPointer<vtkIdList> idx_list = 
        vtkSmartPointer<vtkIdList>::New();
    meshdata::VertexIterator vi = mesh.vert.begin() + i;
    double ref_point[3] = {vi->cP()[0], vi->cP()[1], vi->cP()[2]};
    kd_tree->FindPointsWithinRadius(radius, ref_point, idx_list);
    double denominator = 0.0f;
    double numerator_coeff = 0.0f;
    meshdata::VertexIterator vi2;
    std::vector<double> tmp_zer(zerniker_coeff[i].size(), 0.0f);
    zer_coeff_gauss[i].resize(zerniker_coeff[i].size(), 0.0f);
    if(idx_list->GetNumberOfIds() == 1)
      tmp_zer = zerniker_coeff[i];
    else
    {
      for(size_t j = 0; j < idx_list->GetNumberOfIds(); ++j)
      {
        int v2_idx = idx_list->GetId(j);
        vi2 = mesh.vert.begin() + v2_idx;
        double dist_square = vcg::SquaredDistance<float>(vi->P(), vi2->P());
        numerator_coeff = -1.0f * (dist_square / (2.0f * radius * radius));
        numerator_coeff = exp(numerator_coeff);
        for(int k = 0; k < zerniker_coeff[v2_idx].size(); ++k)
          tmp_zer[k] += zerniker_coeff[v2_idx][k] * numerator_coeff;
        denominator += numerator_coeff;
      }
      DivVector<double>(tmp_zer, denominator);
    }
    if(is_diff)
    {
      for(int j = 0; j < tmp_zer.size(); ++j)
      {
        zer_coeff_gauss[i][j] -= tmp_zer[j];
      }
    }
    else 
      zer_coeff_gauss[i] = tmp_zer;
  }
}

void SurfaceDescriptor::GenerateExportFileName(const string &model_file, 
                                               const string &flag_str,
                                               string &export_file)
{
  int flag = model_file.find_last_of(".");
  export_file = model_file.substr(0, flag);
  export_file += flag_str;
  cout << "[#Info]Test file path is : " << export_file << endl; 
}

//calculate zernike radial polynomial
double  CalRadialPolyP0Q0(double rho)
{
  return 1;
}

double  CalRadialPolyP1Q1(double rho)
{
  return rho;
}

double  CalRadialPolyP2Q0(double rho)
{
  return (2.0f * rho * rho - 1.0f);
}

double  CalRadialPolyP2Q2(double rho)
{
  return rho * rho;
}
double  CalRadialPolyP3Q1(double rho)
{
  double rho_square = rho * rho;
  return (rho * (3.0 * rho_square - 1.0));
}
double  CalRadialPolyP3Q3(double rho)
{
  return rho * rho * rho;
}

double  CalRadialPolyP4Q0(double rho)
{
  double rho_square = rho * rho;
  return (6.0 * rho_square * (rho_square - 1.0));
}
double  CalRadialPolyP4Q2(double rho)
{
  double rho_square = rho * rho;
  return rho_square * (4.0 * rho_square - 3.0f);
}
double  CalRadialPolyP4Q4(double rho)
{
  return std::pow(rho, 4.0f);
}
double  CalRadialPolyP5Q1(double rho)
{
  double rho_cube = std::pow(rho, 3);
  return (2.0 * rho_cube * (5.0 * rho * rho - 6.8) + 3.0 * rho);
}
double  CalRadialPolyP5Q3(double rho)
{
  double rho_square = rho * rho;
  return (rho_square * rho * (5.0f * rho_square - 4.0f));
}
double  CalRadialPolyP5Q5(double rho)
{
  return std::pow(rho, 5.0f);
}
double  CalRadialPolyP6Q0(double rho)
{
  double rho_square = rho * rho;
  return 10.0 * rho_square * rho_square * (2.0f * rho_square - 3.0f) + 
      12.0f * rho_square - 1.0f;
}

double  CalRadialPolyP6Q2(double rho)

{
  double rho_square = rho * rho;
  return (5.0f * rho_square * rho_square * ( 3.0f * rho_square) + 6 * rho_square);
}
double  CalRadialPolyP6Q4(double rho)
{
  double rho_square = rho * rho;
  return (rho_square * rho_square * (6 * rho_square - 5.0f));
}
double  CalRadialPolyP6Q6(double rho)
{
  return std::pow(rho, 6.0f);
}
double  CalRadialPolyP7Q1(double rho)
{
  double rho_square = rho * rho;
  return (5.0f * rho_square * rho * (7.0f * rho_square * rho_square - 
                                      12.0f * rho_square + 6.0f) - 4.0f * rho);
}
double  CalRadialPolyP7Q3(double rho)
{
  double rho_square = rho * rho;
  return (rho_square * rho * (10 - 30 * rho_square + 21 * rho_square * rho_square));
}
double  CalRadialPolyP7Q5(double rho)
{
  double rho_five = std::pow(rho, 5);
  return rho_five * (7 * rho * rho - 6);
}

double  CalRadialPolyP7Q7(double rho)
{
  return std::pow(rho, 7);
}

double  CalRadialPolyP8Q0(double rho)
{
  double rho_square = rho * rho;
  double rho_four = rho_square * rho_square;
  return (1 - 20 * rho_square + 10 * rho_four * (9 - 14 * rho_square + 7 * rho_four));
}

double  CalRadialPolyP8Q2(double rho)
{
  double rho_square = rho * rho;
  double rho_four = rho_square * rho_square;
  return (rho_four * (60 - 105 * rho_square + 56 * rho_four) - 10 * rho_square);
}

double  CalRadialPolyP8Q4(double rho)
{
  double rho_square = rho * rho;
  double rho_four = rho_square * rho_square;
  return (rho_four * (15 - 42 * rho_square + 28 * rho_four));
}

double  CalRadialPolyP8Q6(double rho)
{
  double rho_six = pow(rho, 6);
  return (rho_six * (8 * rho * rho - 7));
}

double  CalRadialPolyP8Q8(double rho)
{
  return std::pow(rho, 8);
}

void SurfaceDescriptor::PushRadialPolyFunc(std::vector<func_ptr> &radial_poly)
{
  radial_poly.push_back(CalRadialPolyP0Q0);
  radial_poly.push_back(CalRadialPolyP1Q1);
  radial_poly.push_back(CalRadialPolyP2Q0);
  radial_poly.push_back(CalRadialPolyP2Q2);
  radial_poly.push_back(CalRadialPolyP3Q1);
  radial_poly.push_back(CalRadialPolyP3Q3);
  radial_poly.push_back(CalRadialPolyP4Q0);
  radial_poly.push_back(CalRadialPolyP4Q2);
  radial_poly.push_back(CalRadialPolyP4Q4);
  radial_poly.push_back(CalRadialPolyP5Q1);
  radial_poly.push_back(CalRadialPolyP5Q3);
  radial_poly.push_back(CalRadialPolyP5Q5);
  radial_poly.push_back(CalRadialPolyP6Q0);
  radial_poly.push_back(CalRadialPolyP6Q2);
  radial_poly.push_back(CalRadialPolyP6Q4);
  radial_poly.push_back(CalRadialPolyP6Q6);
  radial_poly.push_back(CalRadialPolyP7Q1);
  radial_poly.push_back(CalRadialPolyP7Q3);
  radial_poly.push_back(CalRadialPolyP7Q5);
  radial_poly.push_back(CalRadialPolyP7Q7);
  radial_poly.push_back(CalRadialPolyP8Q0);
  radial_poly.push_back(CalRadialPolyP8Q2);
  radial_poly.push_back(CalRadialPolyP8Q4);
  radial_poly.push_back(CalRadialPolyP8Q6);
  radial_poly.push_back(CalRadialPolyP8Q8);
}

}

}
