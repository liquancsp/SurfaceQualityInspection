#include <menu_operate/GlobalLSNormalComponent.h>
#include <menu_operate/VtkLeastSquare.h>

#include <ctime>

namespace sqi {
namespace alg{

using namespace meshdata;
using namespace lq;
using namespace std;

GlobalLSNormalComponent::GlobalLSNormalComponent():BaseAlgorithm()
{}

GlobalLSNormalComponent::~GlobalLSNormalComponent()
{
  
}

void GlobalLSNormalComponent::cal_tri_error(meshdata::MeshDataPtr &mesh_data, lq::InspectFlag flag)
{
  clock_t start, end;
  start = clock();  
  VtkLeastSquare<Mesh, vcg::Matrix33f> svd;
  vcg::Matrix33f rotate_mat;
  svd.GlobalLeastSquare(mesh_data->M(kRight), mesh_data->M(kLeft), rotate_mat);
#pragma omp parallel for num_threads(thread_num(mesh_data->FNum(0), MIN_ITERATOR_NUM))
  for(int i = 0; i < mesh_data->FNum(kRight); ++i)
  {
    FaceIterator fi1 = mesh_data->FBeginIter(kLeft) + i;
    FaceIterator fi2 = mesh_data->FBeginIter(kRight) + i;
    vcg::Point3f bary_center1 = vcg::Barycenter(*fi1);
    vcg::Point3f bary_center2 = vcg::Barycenter(*fi2);
    bary_center2 = rotate_mat * (bary_center2 + svd.GetTranslate());
    bary_center2 -= bary_center1;
    fi2->Q() = bary_center2 * fi1->cN() / fi1->cN().Norm();
  }
  BaseAlgorithm::GenerateFinalScalarField(mesh_data, flag);
  end = clock();
  io::ConsoleMessage<string>::SendMessage("", io::kEndAlg, lq::kNull, mesh_data->MinE(),
                                          difftime(end, start)/CLOCKS_PER_SEC);
}

}

}
