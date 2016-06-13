#ifndef LEASTSQUARES_H
#define LEASTSQUARES_H
#include <eigenlib/Eigen/SVD>
#include <eigenlib/Eigen/src/Core/Matrix.h>
#include <vcg/space/deprecated_point3.h>

#include <vector>

namespace sqi{
namespace alg{

template<typename MeshType, typename MatrixType, typename VecType = vcg::Point3f>
class LeastSquares{
public:
  
  LeastSquares(){}
  ~LeastSquares()
  {}

  typedef typename MeshType::FaceType FaceType;
  typedef typename MeshType::FacePointer FacePointer;
  typedef typename MeshType::FaceIterator FaceIterator;
  typedef typename MeshType::VertexIterator VertexIterator;
  typedef typename MeshType::VertContainer VertContainer;
  typedef typename MeshType::VertexType VertexType;
  typedef typename MeshType::VertexPointer VertexPointer;

  void ExecuteSVD(MatrixType& mat)
  {
    Eigen::Matrix3f eigen_mat;
    mat.ToEigenMatrix(eigen_mat);
//    Eigen::JacobiSVD<Eigen::Matrix3f> svd(eigen_mat, Eigen::ComputeThinU | 
//                                          Eigen::ComputeThinV);
    Eigen::JacobiSVD<Eigen::Matrix3f> svd(eigen_mat, Eigen::ComputeFullU | 
                                          Eigen::ComputeFullV);
    
    std::cout << "singular value : " << svd.singularValues() << std::endl;
    
    matrix_u_.FromEigenMatrix(svd.matrixU());
    matrix_v_.FromEigenMatrix(svd.matrixV());
    std::cout << "U matrix : " << std::endl;
    PrintMatrix(matrix_u_);
    std::cout << "V matrix : " << std::endl;
    PrintMatrix(matrix_v_);
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
    std::cout << "rotate matrix :" << std::endl;
    PrintMatrix(rotate_mat);
    translate_ = centroid2 - rotate_mat * centroid1;
    for(size_t i = 0; i < 3; ++i)
      std::cout << translate_[i] << "  ";
    std::cout << std::endl;
  }
  
  void GetRotateMatrix(MatrixType& mat)
  {
    mat = matrix_u_ * matrix_v_;
    mat.Transpose();
  }

  const VecType& GetTranslate() const 
  {
    return translate_;
  }
  
  
protected :
  void PrintMatrix(MatrixType mat)
  {
    for(size_t i = 0; i < 3; ++i)
    {
      for(size_t j = 0; j < 3; ++j)
        std::cout << mat[i][j] << " ";
      std::cout << std::endl;
    }
  }

  MatrixType matrix_u_;
  MatrixType matrix_v_;
  
  VecType translate_;
};

}
}

#endif // LEASTSQUARES_H
