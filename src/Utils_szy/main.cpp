#include <iostream>
#include <string>

#include <vcg/complex/complex.h>
#include <vcg/complex/algorithms/create/platonic.h>
#include <vcg/complex/algorithms/update/curvature.h>
#include <vcg/complex/algorithms/update/normal.h>
#include <vcg/complex/algorithms/update/quality.h>
#include <wrap/io_trimesh/export_obj.h>
#include <wrap/io_trimesh/import_obj.h>
//#include <wrap/io_trimesh/vtk.h>
#include <vector>
#include <utility>

using namespace std;

//#define TEST_MERTE

const double ACCURACY = 1e-10;
class MeshEdge;
class MeshFace;
class MeshVertex;
struct MeshUsedTypes : public vcg::UsedTypes<	vcg::Use<MeshVertex>::AsVertexType,
    vcg::Use<MeshEdge>::AsEdgeType,
    vcg::Use<MeshFace>::AsFaceType>{};

class MeshVertex : public vcg::Vertex<MeshUsedTypes, vcg::vertex::Coord3f, 
    vcg::vertex::Normal3f, vcg::vertex::Curvaturef, vcg::vertex::Qualityd,
    vcg::vertex::CurvatureDirf, vcg::vertex::VFAdj, vcg::vertex::Color4b,
    vcg::vertex::VEAdj, vcg::vertex::BitFlags>{};

class MeshFace : public vcg::Face< MeshUsedTypes, 
    vcg::face::FFAdj, vcg::face::VFAdj, vcg::face::FEAdj,
    vcg::face::Qualityd, vcg::face::Color4b, vcg::face::Normal3f, 
    vcg::face::VertexRef, vcg::face::BitFlags> {};

class MeshEdge : public vcg::Edge<MeshUsedTypes, vcg::edge::VertexRef, 
    vcg::edge::EFAdj, vcg::edge::EEAdj, vcg::edge::EVAdj, vcg::edge::VEAdj,
    vcg::edge::BitFlags>{};

class Mesh : public vcg::tri::TriMesh< std::vector<MeshVertex>, 
    std::vector<MeshFace>, std::vector<MeshEdge>  > {};

typedef Mesh::VertexIterator                     VertexIterator;
typedef Mesh::VertexType                         VertexType;
typedef Mesh::VertexPointer                      VertexPointer;
typedef Mesh::ConstVertexPointer                 ConstVertexPointer;
typedef Mesh::ConstVertexIterator                ConstVertexIterator;
typedef Mesh::VertContainer                      VertContainer;

typedef Mesh::FaceIterator                       FaceIterator;
typedef Mesh::FaceType                           FaceType;
typedef Mesh::FacePointer                        FacePointer;
typedef Mesh::ConstFacePointer                   ConstFacePointer; 
typedef Mesh::ConstFaceIterator                  ConstFaceIterator;
typedef Mesh::FaceContainer                      FaceContainer;

typedef Mesh::EdgeIterator                       EdgeIterator;
typedef Mesh::EdgeType                           EdgeType;
typedef Mesh::EdgePointer                        EdgePointer;
typedef Mesh::ConstEdgeIterator                  ConstEdgeIterator;

typedef Mesh::CoordType                          CoordType;

template <typename OS, typename FLOAT, typename INT>
int tri2vtk(OS &os,
            const FLOAT *node, size_t node_num,
            const INT *tri, size_t tri_num) {
  assert(os);
  os << "# vtk DataFile Version 2.0\nTRI\nASCII\n\nDATASET UNSTRUCTURED_GRID\n";

  os<< "POINTS " << node_num << " float\n";
  for(size_t i = 0; i < node_num; ++i)
    os << node[i*3+0] << " " << node[i*3+1] << " " << node[i*3+2] << "\n";
  os << "CELLS " << tri_num << " " << tri_num*4 << "\n";
  for(size_t i = 0; i < tri_num; ++i)
    os << 3 << "  " << tri[i*3+0] << " " << tri[i*3+1] << " " << tri[i*3+2] << "\n";
  os << "CELL_TYPES " << tri_num << "\n";
  for(size_t i = 0; i < tri_num; ++i)
    os << 5 << "\n";
  return 0;
}

template <typename OS, typename Iterator, typename INT>
void vtk_data(OS &os, Iterator first, INT size, char *value_name,
              char *table_name = "my_table")
{
  os << "SCALARS " << value_name << " float\nLOOKUP_TABLE " << table_name << "\n";
  for(size_t i = 0; i < size; ++i, ++first)
    os << *first << "\n";
}

template <typename OS, typename Iterator, typename INT>
void cell_data(OS &os, Iterator first, INT size, char *value_name, 
               char *table_name = "my_table")
{
  os << "CELL_DATA " << size << "\n";
  vtk_data(os, first, size, value_name, table_name);
}

template<typename ItemPtr, typename ItemContainer>
inline int GetIndex(const ItemPtr ptr, const ItemContainer& container)
{
  return ptr - &*(container.begin());
}

inline bool IsZero(const double &x, const double eps = ACCURACY)
{
  return (fabs(x) < eps);
}

template<typename VertexType, typename ColorType>
void ConvertMeshData(Mesh& mesh, vector<VertexType>& vertex_coord, vector<int>& face_idx,
                     vector<ColorType>& color_idx)
{
  vertex_coord.clear();
  for(VertexIterator vi = mesh.vert.begin(); vi != mesh.vert.end(); ++vi)
  {
    for(int i = 0; i < 3; ++i)
      vertex_coord.push_back(vi->P()[i]);
  }
  face_idx.clear();
  for(FaceIterator fi = mesh.face.begin(); fi != mesh.face.end(); ++fi)
  {
    for(int i = 0; i < 3; ++i)
    {
      int idx = GetIndex<ConstVertexPointer, VertContainer>((*fi).cV(i), mesh.vert);
      face_idx.push_back(idx);
    }
    color_idx.push_back(fi->Q());    
  }
 
}

void SetColorType(Mesh& mesh, int color_type)
{
  vcg::tri::UpdateQuality<Mesh>::FaceConstant(mesh, static_cast<float>(color_type));
}

//split string 
std::vector<float> Split(std::string str,std::string pattern)
{
    std::string::size_type pos;
    std::vector<float> result;
    str += pattern;
    int size = str.size();
    for(int i = 0; i < size; ++i)
    {
        pos = str.find(pattern, i);
        if(pos < size)
        {
          if((pos - i) > 0)
          {
            std::string s = str.substr(i, pos - i);
            result.push_back(atof(s.c_str()));
            i = pos + pattern.size() - 1;
          }  
        }
    }
    return result;
}

void SetMaxtrix4x4(const vector<float>& mat_value, vcg::Matrix44f& mat)
{
  assert(mat_value.size() == 16);
  mat.SetZero();
  for(int k = 0; k < mat_value.size(); ++k)
  {
    int i = k / 4;
    int j = k % 4;
    mat.ElementAt(i, j) = mat_value[k];
  }
}

void ExportVTK(Mesh& merge, const string& out_file)
{
  ofstream fout(out_file.c_str());
  vector<float> vertex_coord;
  vector<int> face_idx;
  vector<float> color_idx;
  ConvertMeshData<float, float>(merge, vertex_coord, face_idx, color_idx);
  tri2vtk<ofstream, float, int>(fout, &vertex_coord[0], merge.vert.size(),
      &face_idx[0], merge.face.size());
  cell_data<ofstream, vector<float>::iterator, int>(fout, color_idx.begin(), 
                                                    color_idx.size(), "color");
  fout.close();
}

//./ExportOBJ color_type sphere_obj cylinder_obj matrix4x4_file.
void CreateMeshByApplyMatrix(int argc, char** argv)
{
  assert(argc == 5);
  Mesh obj, merge, joint;
  int color_type = atoi(argv[1]);
  string obj_file(argv[2]);
  string joint_file(argv[3]);
#ifndef TEST_MERTE
  ifstream matrix_fin(argv[4]);
  if(matrix_fin.fail())
  {
    cerr << "[#Error]Fail to load matrix file." << endl;
    return ;
  }
  vector<vector<float> > matrix_list;
  string input;
  while(getline(matrix_fin, input))
    matrix_list.push_back(Split(input, " "));
  matrix_fin.close();
#endif
  int mask = 0;
  vcg::tri::io::ImporterOBJ<Mesh>::Open(obj, obj_file.c_str(), mask);
  vcg::tri::io::ImporterOBJ<Mesh>::Open(joint, joint_file.c_str(), mask);
  SetColorType(obj, color_type);
  SetColorType(joint, 1);
  vcg::tri::Append<Mesh, Mesh>::MeshCopy(merge, obj);
  
  vcg::Matrix44f mat;
  mat.SetIdentity();
#ifdef TEST_MERTE
  int n = 20;
  vcg::Point3f tmp_axes(0, 0, 1);
  mat.SetRotateDeg(45, tmp_axes);
  mat.ElementAt(0, 3) = 2;
  mat.ElementAt(1, 3) = 2;
  mat.ElementAt(2, 3) = 2;
  for(int i = 0; i < n; ++i)
  {
#else
  for(int i = 0; i < matrix_list.size(); ++i)
  {
    SetMaxtrix4x4(matrix_list[i], mat);
#endif
    vcg::tri::UpdateBounding<Mesh>::Box(obj);
    vcg::Point3f pre_vertex = obj.bbox.Center();
    vcg::tri::UpdatePosition<Mesh>::Matrix(obj, mat);
    vcg::tri::Append<Mesh, Mesh>::Mesh(merge, obj);//append new position into final mesh
    vcg::tri::UpdateBounding<Mesh>::Box(obj);
    vcg::Point3f pos_vertex = obj.bbox.Center();
    vcg::Point3f dir = pos_vertex - pre_vertex;
    Mesh tmp;
    vcg::tri::Append<Mesh, Mesh>::MeshCopy(tmp, joint);
    float scale = dir.Norm();
    for(VertexIterator vi = tmp.vert.begin(); vi != tmp.vert.end(); ++vi)
    {
      if(!IsZero(vi->P()[2], 1e-6))
        vi->P()[2] = vi->P()[2] * scale;
    }
    dir.Normalize();
    vcg::Point3f ref_dir(0, 0, 1);
    vcg::Point3f axes = ref_dir ^ dir;
    axes.Normalize();
    float angle = acos(dir.dot(ref_dir));
    vcg::Matrix44f transform_mat;
    transform_mat.SetIdentity();
    transform_mat = transform_mat.SetRotateRad(angle, axes);
    transform_mat.ElementAt(0, 3) = pre_vertex[0];
    transform_mat.ElementAt(1, 3) = pre_vertex[1];
    transform_mat.ElementAt(2, 3) = pre_vertex[2];
    vcg::tri::UpdatePosition<Mesh>::Matrix(tmp, transform_mat);
    vcg::tri::Append<Mesh, Mesh>::Mesh(merge, tmp);
  }
  ofstream fout("merge.vtk");
  vector<float> vertex_coord;
  vector<int> face_idx;
  vector<int> color_idx;
  ConvertMeshData<float, int>(merge, vertex_coord, face_idx, color_idx);
  tri2vtk<ofstream, float, int>(fout, &vertex_coord[0], merge.vert.size(),
      &face_idx[0], merge.face.size());
  cell_data<ofstream, vector<int>::iterator, int>(fout, color_idx.begin(), 
                                                  color_idx.size(), "color");
  fout.close();
}

void LoadCSVFile(const string& file_path, vector<vcg::Point4f>& vertex_list)
{
  ifstream fin(file_path.c_str());
  if(fin.fail())
  {
    cerr << "[#Error]Fail to find " << file_path << endl;
    exit(1);
  }
  string input;
  while(getline(fin, input))
  {
    vector<float> data = Split(input, ",");
    assert(data.size() == 4);
    vertex_list.push_back(vcg::Point4f(data[1], data[2], data[3], data[0]));
  }
  cout << "[#Info]Vertex number is : " << vertex_list.size() << endl;
  fin.close();
}

void CreateSphere(const vector<vcg::Point4f>& vertex_list, Mesh& sphere,
                  Mesh& merge, float radius)
{
  Mesh init_sphere;
  vcg::tri::Append<Mesh, Mesh>::MeshCopy(init_sphere, sphere);
  vcg::Matrix44f mat;
  mat.SetIdentity();
  mat.SetScale(radius, radius, radius);
  vcg::tri::UpdatePosition<Mesh>::Matrix(init_sphere, mat);
  for(int i = 0; i < vertex_list.size(); ++i)
  {
    Mesh tmp_sphere;
    vcg::tri::Append<Mesh, Mesh>::MeshCopy(tmp_sphere, init_sphere);
    mat.SetIdentity();
    mat.SetTranslate(vertex_list[i][0], vertex_list[i][1], vertex_list[i][2]);
    vcg::tri::UpdatePosition<Mesh>::Matrix(tmp_sphere, mat);
    vcg::tri::UpdateQuality<Mesh>::FaceConstant(tmp_sphere, vertex_list[i][3]);
    //cout << "color : " << vertex_list[i][3] << endl;
    vcg::tri::Append<Mesh, Mesh>::Mesh(merge, tmp_sphere);
  }
}

void MergeMesh(int argc, char** argv)
{
  assert(argc == 5);
  Mesh sphere_obj, merge;
  string csv_file(argv[1]), out_file(argv[4]);
  pair<float, vector<vcg::Point4f> > csv_data1;
  csv_data1.first = atof(argv[3]);
  string sphere_file(argv[2]);
  int mask = 0;
  vcg::tri::io::ImporterOBJ<Mesh>::Open(sphere_obj, sphere_file.c_str(), mask);
  LoadCSVFile(csv_file, csv_data1.second);
  CreateSphere(csv_data1.second, sphere_obj, merge, csv_data1.first);
  ExportVTK(merge, out_file);
}

void Parser(int argc, char** argv)
{
  /*if(argc == 5)
    CreateMeshByApplyMatrix(argc, argv);
  else*/ if(argc == 5)
    MergeMesh(argc, argv);
}

int main(int argc, char** argv)
{
  Parser(argc, argv);
  return 0;
}
