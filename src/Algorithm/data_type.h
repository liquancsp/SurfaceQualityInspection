#ifndef DATA_TYPE_H
#define DATA_TYPE_H
#include <vector>
#include <map>
#include <set>
#include <cmath>
#include <omp.h>
#include "../3rd/zjucad/matrix/matrix.h"

namespace lq
{


#define INDEPENDENT//if not define INDEPENDENT, we will use the uniform color scalar
const double ACCURACY = 1e-10;
typedef zjucad::matrix::matrix<double> matrixd;
typedef zjucad::matrix::matrix<float> matrixf;
typedef zjucad::matrix::matrix<size_t> matrixst;

const double PI = acos(-1.0);
const size_t VIEW_POINT_CHAR_SIZE = 50;
const double RIGHT_ANGLE = PI / 2;
const int MIN_ITERATOR_NUM = 8;
const int g_ncore = omp_get_num_procs();
enum TRI_TYPE
{
  ACUTE = 0, 
  RIGHT, 
  OBTUSE
};

enum WindowFlag
{
  kLeft = 0,
  kRight,
  kNullWin
};

enum ReturnValue
{
  kOK = 0,
  kError,
  kColorInitFail,
  kColorAlreadyInit
};

enum ColorScalarType
{
  Independent = 0,
  Uniform
};

enum InspectFlag
{
  kReflectionMap = 0,
  kIntegrateReflection,
  kGlobalAlignInteRefl,
  kGlobalAlignNormalComponent,
  kLocalAlignIntegralRefl,
  kLocalAlignNormalDivision,
  kGlobalLSIntegralRefl,
  kGlobalLSNormalComponent,
  kConvexDiff,
  kMixReflConvex,
  kMixInteConvex,
  kMeanCurvature,
  kGaussianCurvature,
  kPrincipalK1,
  kPrincipalK2,
  kNormalVectorAngle,
  kDistanceDifferent,
  kMixMeanInte,
  kMixGaussInte,
  kMixK1Inte,
  kMixK2Inte,
  kDefectFitting,
  kZernikePropagation,
  kCalcuateZernikerCoefficient,
  kZernikerClassify,
  kCalMeanZernikerCoeff,
  kCalGaussZernikerCoeff,
  kCalK1ZernikerCoeff,
  kCalK2ZernikerCoeff,
  kCalCurvatureZernikerCoeff,
  kCalCurvatureDescriptor,
  kCalCurvatureHistogram,
  kCalSmoothCurvatureDiff,
  kTrainAndPredict,
  kClassify,
  kNull
};

enum AlgType
{
  kCalSingleProperty = 0,
  kCalTwoDiff
};

enum RingType
{
  Vertex = 0,
  Triangle
};

enum RepresentationType
{
  kPoints = 0,
  kWireframe,
  kSurfaces,
  kSurfaceWithEdge
};

struct vertex_normal 
{
  double nx, ny, nz;
  vertex_normal() {}
  vertex_normal(double nx0, double ny0, double nz0):
    nx(nx0), ny(ny0), nz(nz0) {}
};

struct point
{
  double x, y, z;
  point()
  {}
  point(double x0, double y0, double z0):
    x(x0), y(y0), z(z0) {}
  point(const point &p):x(p.x), y(p.y), z(p.z){}
  point(const double p[3]):x(p[0]), y(p[1]), z(p[2]){}
  point(const vertex_normal &normal):x(normal.nx), y(normal.ny), z(normal.nz){}
};

struct edge_adj_tri
{
  size_t num;
  std::vector<size_t> tri;
//      int begin;
//      int end;
  edge_adj_tri():num(0)
  {}
};

struct triangle 
{
  triangle()
  {
    
  }
  triangle(int a, int b, int c)
  {
    vertex.push_back(a);
    vertex.push_back(b);
    vertex.push_back(c);
  }
  std::vector<int> vertex;
  std::vector<int> texture;
  std::vector<int> point_normal;
  point triangle_normal;
};

struct triangle_normal {
  zjucad::matrix::matrix<double> normal;
  triangle_normal(){
  }
  triangle_normal(zjucad::matrix::matrix<double> &x):normal(x){
  }
};

struct color
{
  double scalar;
  double R;
  double G;
  double B;
};

struct vertex_adj_tri {
  std::vector<size_t> adj_tri;
};

struct vertex_info
{
  size_t index;
  double angle;
  double subtense;
};

struct triangle_info
{
  std::map<size_t, vertex_info> info;
  size_t type;
  double area;
};

struct principal_curvature_info
{
  double k1;
  double k2;
};

struct ring_neighbor
{
  std::set<size_t> adj_set;
};

typedef ring_neighbor vertex_neighbor;
typedef ring_neighbor triangle_neighbor;
typedef point vec;

/*******************************************************************************
 *@brief this fucntion is to get max thread number for parallel in openmp
 *
 *@param n number of loop iterations
 *@param min_n Single thread requires minimal iterations
 *
 *@return thread number
 ******************************************************************************/

template<typename T1, typename T2>
inline T1 thread_num(T1 n, T2 min_n)
{
  T1 max_tn = n / min_n;
  T1 tn = max_tn > g_ncore ? g_ncore : max_tn;
  if (tn < 1)
  {
    tn = 1;
  }
  return tn;
}

inline point operator+(const point &point1, const point &point2)
{
  return point(point1.x + point2.x, point1.y + point2.y,
               point1.z + point2.z);

}

inline vertex_normal operator+(const vertex_normal &vn1,
                               const vertex_normal &vn2)
{
  return vertex_normal(vn1.nx + vn2.nx, vn1.ny + vn2.ny,
                       vn1.nz + vn2.nz);
}

inline point operator-(const point &point1, const point &point2)
{
  return point(point1.x - point2.x, point1.y - point2.y,
               point1.z - point2.z);
}

inline point operator*(const point &p1, const double num)
{
  return point(p1.x * num, p1.y *num, p1.z * num);
}


inline point operator*(const double num, const point &p1)
{
  return point(p1.x * num, p1.y *num, p1.z * num);
}

inline vertex_normal operator-(const vertex_normal &vn1,
                               const vertex_normal &vn2)
{
  return vertex_normal(vn1.nx - vn2.nx, vn1.ny - vn2.ny,
                       vn1.nz - vn2.nz);
}

inline vertex_normal operator/(const vertex_normal &vn,
                               const double n)
{
  return vertex_normal(vn.nx / n, vn.ny / n, vn.nz / n);
}

inline std::ostream &operator<<(std::ostream &cout, const point &p)
{
  cout << p.x << " " << p.y << " " << p.z;
}

inline bool is_zero(const double &x, const double eps = ACCURACY)
{
  return (fabs(x) < eps);
}

inline bool equal(const double x, const double y, double eps = ACCURACY)
{
  return (fabs(x - y) < ACCURACY);
}

inline bool equal(const point &p1, const point &p2)
{
  return ((fabs(p1.x - p2.x) <ACCURACY) && (fabs(p1.y - p2.y) < ACCURACY)
          && (fabs(p1.z - p2.z) < ACCURACY));
}

inline point operator/(const point &p,
                       const double n)
{
  if(!is_zero(n))
    return point(p.x / n, p.y / n, p.z / n);
  return point(0.0f, 0.0f, 0.0f);
}

inline bool is_larger(const double &x, const double &y)
{
  return ((!equal(x, y)) && (x > y));
}

inline bool is_less(const double &x, const double &y)
{
  return ((x < y));
}

inline bool is_less_equal(const double &x, const double &y)
{
  return ((equal(x, y)) || (x < y));
}

inline bool is_larger_equal(const double &x, const double &y)
{
  return ((equal(x, y)) || (x > y));
}

inline bool equal(const vertex_normal &vn1, const vertex_normal &vn2, const double eps = ACCURACY)
{
  return ((fabs(vn1.nx - vn2.nx) < eps) &&
          (fabs(vn1.ny - vn2.ny) < eps) &&
          (fabs(vn1.nz - vn2.nz) < eps));
}


inline double dot_mult(const point &p1, const point &p2)
{
  return (p1.x * p2.x + p1.y * p2.y + p1.z * p2.z);
}

inline point cross_mult(const point &p1, const point &p2)
{
  return point((p1.y * p2.z - p1.z * p2.y),
               (p1.z * p2.x - p1.x * p2.z),
               (p1.x * p2.y - p1.y * p2.x));
}

inline vertex_normal cross_mult(const vertex_normal &p1, const vertex_normal &p2)
{
  return vertex_normal((p1.ny * p2.nz - p1.nz * p2.ny),
                       (p1.nz * p2.nx - p1.nx * p2.nz),
                       (p1.nx * p2.ny - p1.ny * p2.nx));
}

inline double vec_norm(const point &p)
{
  return sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
}

inline double vec_norm(const vertex_normal &p)
{
  return sqrt(p.nx * p.nx + p.ny * p.ny + p.nz * p.nz);
}

inline void cal_tri_center(const triangle &tri,
                           const std::vector<point> &vertex_list,
                           point &tri_center)
{
  tri_center = (vertex_list[tri.vertex[0] - 1] + vertex_list[tri.vertex[1] - 1]
                + vertex_list[tri.vertex[2] - 1]) / 3;
}

inline void get_center_point(const std::vector<point> &vertex_list,
                             const triangle &tri, point &center)
{
  size_t index1 , index2, index3;
  index1 = tri.vertex[0] - 1;
  index2 = tri.vertex[1] - 1;
  index3 = tri.vertex[2] - 1;
  center = (vertex_list[index1] + vertex_list[index2] + vertex_list[index3]) / 3;
  //std::cout << "center " << center.x << " " << center.y << " " << center.z << endl;
}

inline void get_model_name(const std::string &file_path,
                           std::string &file_name)
{
  size_t flag;
  flag = file_path.find_last_of("/\\");
  if(flag == std::string::npos && file_path.length() >= 0)
    file_name = file_path;
  else 
    file_name = file_path.substr(flag + 1);
}
inline std::ostream& operator<<(std::ostream& stream, const vertex_normal &normal)
{
  stream << "(" << normal.nx << ", " << normal.ny << ", " << normal.nz << ")";
}

inline matrixd point_to_mat(const point &p, matrixd &mat)
{
  mat = zjucad::matrix::zeros(3, 1);
  mat(0, 0) = p.x;
  mat(1, 0) = p.y;
  mat(2, 0) = p.z;
}

inline matrixd point_to_mat(const vertex_normal &p, matrixd &mat)
{
  mat = zjucad::matrix::zeros(3, 1);
  mat(0, 0) = p.nx;
  mat(1, 0) = p.ny;
  mat(2, 0) = p.nz;
}


inline matrixd mat_to_point(const matrixd &mat, vertex_normal &p)
{
  p.nx = mat(0, 0);
  p.ny = mat(1, 0);
  p.nz = mat(2, 0);
}

};

#endif
