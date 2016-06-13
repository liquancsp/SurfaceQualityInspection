#ifndef VTKLEASTSQUARE_H
#define VTKLEASTSQUARE_H
#include <vtkMath.h>
#include <vcg/space/deprecated_point3.h>
#include <vcg/math/matrix44.h>

namespace sqi {
namespace alg{

template<typename MeshType, typename MatrixType, typename VecType = vcg::Point3f>
class VtkLeastSquare{
public:
  
  VtkLeastSquare(){}
  ~VtkLeastSquare(){}
  typedef typename MeshType::FaceType FaceType;
  typedef typename MeshType::FacePointer FacePointer;
  typedef typename MeshType::FaceIterator FaceIterator;
  typedef typename MeshType::VertexIterator VertexIterator;
  typedef typename MeshType::VertContainer VertContainer;
  typedef typename MeshType::VertexType VertexType;
  typedef typename MeshType::VertexPointer VertexPointer;
  
  void ExecuteSVD(MatrixType& mat)
  {
    float A[3][3];
    for(size_t i = 0; i < 3; ++i)
      for(size_t j = 0; j < 3; ++j)
        A[i][j] = mat[i][j];
    vtkMath::SingularValueDecomposition3x3(A, U_, W, VT_);
  }
  
  
  void GlobalLeastSquare(MeshType &source, MeshType &target, MatrixType &rotate_mat)
  {
    vcg::Point3f centroid1(0.0, 0.0, 0.0);
    vcg::Point3f centroid2(0.0, 0.0, 0.0);
    for(VertexIterator vi1 = source.vert.begin(), vi2 = target.vert.begin(); 
        vi1 != source.vert.end(), vi2 != target.vert.end(); ++vi1, ++vi2)
    {
      centroid1 += vi1->cP();
      centroid2 += vi2->cP();
    }
    centroid1 /= static_cast<float>(source.vert.size());
    centroid2 /= static_cast<float>(target.vert.size());
    for(size_t i = 0; i < 3; ++i)
      std::cout << "centroid1 : " << centroid1[0] << " " << centroid1[1] << " "
                << centroid1[2] << std::endl;
    MatrixType H;
    //It must be initialized
    H.SetZero();
    for(VertexIterator vi1 = source.vert.begin(), vi2 = target.vert.begin(); 
        vi1 != source.vert.end(), vi2 != target.vert.end(); ++vi1, ++vi2)
    {
      MatrixType tmp;
      tmp.ExternalProduct(vi1->cP() - centroid1, vi2->cP() - centroid2);
      H += tmp;
    }
    //H.Transpose();
    ExecuteSVD(H);
    GetRotateMatrix(rotate_mat);
    translate_ = centroid2 - rotate_mat * centroid1;
//    for(size_t i = 0; i < 3; ++i)
//      std::cout << translate_[i] << "  ";
//    std::cout << std::endl;
  }
  
  void LocalLeastSquare(MeshType& source,  MeshType& target,
                        std::vector<int>& neigh_list)
  {
    MatrixType rotate_mat;
    vcg::Point3f centroid1(0.0, 0.0, 0.0);
    vcg::Point3f centroid2(0.0, 0.0, 0.0);
    VertexIterator vi1, vi2;
    for(size_t i = 0; i < neigh_list.size(); ++i)
    {
      vi1 = source.vert.begin() + neigh_list[i];
      vi2 = target.vert.begin() + neigh_list[i];
      centroid1 += vi1->cP();
      centroid2 += vi2->cP();
    }
    centroid1 /= static_cast<float>(neigh_list.size());
    centroid2 /= static_cast<float>(neigh_list.size());
    MatrixType H;
    H.SetZero();
    for(size_t i = 0; i < neigh_list.size(); ++i)
    {
      vi1 = source.vert.begin() + neigh_list[i];
      vi2 = target.vert.begin() + neigh_list[i];
      MatrixType tmp;
      tmp.ExternalProduct(vi1->cP() - centroid1, vi2->cP() - centroid2);
      H += tmp;
    }
    ExecuteSVD(H);
    GetRotateMatrix(rotate_mat);
    translate_ = centroid2 - rotate_mat * centroid1;
    mat_.SetIdentity();
    for(size_t i = 0; i < 3; ++i)
      for(size_t j = 0; j < 3; ++j)
        mat_[i][j] = rotate_mat[i][j];
    mat_.SetTranslate(translate_);
//    for(size_t i = 0; i < 4; ++i)
//    {
//      for(size_t j = 0; j < 4; ++j)
//      {
//        cout << mat_[i][j] << "  ";
//      }
//      cout << endl;
//    }
  }

  void GetRotateMatrix(MatrixType& mat)
  {
    MatrixType u_mat, v_mat;
    ToMatrix(U_, u_mat);
    ToMatrix(VT_, v_mat);
    mat = u_mat * v_mat;
    mat.Transpose();
//    std::cout << "rotate matrix : " << std::endl;
//    PrintMatrix(mat);
//    std::cout << "vtk U matrix : " << std::endl;
//    PrintMatrix(U_);
//    std::cout << "vtk V matrix :" << std::endl;
//    PrintMatrix(VT_);
  }
  
  static void PrintMatrix(const float mat[3][3])
  {
    for(size_t i = 0; i < 3; ++i)
    {
      for(size_t j = 0; j < 3; ++j)
        std::cout << mat[i][j] << " ";
      std::cout << std::endl;
    }
  }
  
  static void PrintMatrix(MatrixType mat) 
  {
    for(size_t i = 0; i < 3; ++i)
    {
      for(size_t j = 0; j < 3; ++j)
        std::cout << mat[i][j] << " ";
      std::cout << std::endl;
    }
  }
  
  static void ToMatrix(const float array[3][3], MatrixType& mat)
  {
    for(size_t i = 0 ; i < 3; ++i)
      for(size_t j = 0; j < 3; ++j)
        mat[i][j] = array[i][j];
  }
  
  const VecType& GetTranslate() const 
  {
    return translate_;
  }
  
  const vcg::Matrix44f& GetTransferMatrix()
  {
    return mat_;
  }
  
private:
  float U_[3][3];
  float VT_[3][3];
  float W[3];
  vcg::Matrix44f mat_;
  VecType translate_;
};

}

}

#endif // VTKLEASTSQUARE_H
