#include <menu_operate/GlobalLSIntegralRefl.h>
#include <IntegrationReflection.h>
#include <menu_operate/VtkLeastSquare.h>
#include <CurvatureAlg.h>

#include <ctime>
#include <iomanip>
namespace sqi{
namespace alg{
using namespace meshdata;
using namespace lq;
using namespace std;
GlobalLSIntegralRefl::GlobalLSIntegralRefl() : BaseAlgorithm(), GlobalAlign()
{}

GlobalLSIntegralRefl::~GlobalLSIntegralRefl()
{}

void GlobalLSIntegralRefl::cal_tri_error(meshdata::MeshDataPtr &mesh_data, lq::InspectFlag flag)
{
  clock_t start, end;
  start = clock();
  IntegrationReflection::GenerateIntegralReflMapList(mesh_data);
  CurvatureAlg<Mesh>::UpdateConcavAndConvex(mesh_data->M(lq::kLeft),
                                            mesh_data->M(lq::kRight));
  
  VtkLeastSquare<meshdata::Mesh, vcg::Matrix33f> svd;
  vcg::Matrix33f rotate_mat;
  svd.GlobalLeastSquare(mesh_data->M(lq::kRight), mesh_data->M(lq::kLeft), rotate_mat);
#pragma omp parallel for num_threads(thread_num(mesh_data->FNum(0), MIN_ITERATOR_NUM))  
  for(int i = 0; i < mesh_data->FNum(lq::kRight); ++i)
  {
    FaceIterator fi1 = mesh_data->FBeginIter(kLeft) + i;
    FaceIterator fi2 = mesh_data->FBeginIter(kRight) + i;
    vcg::Point3f normal(fi2->cN()[0], fi2->cN()[1], fi2->cN()[2]);
    normal = rotate_mat * normal;
    double angle = vcg::Angle<float>(fi1->cN(), normal);
    angle = (angle * 180.0f) / lq::PI;
    int diff_idx = static_cast<int>(angle * 100);
    if(diff_idx > mesh_data->IntegralReflList().size() - 1)
      diff_idx = mesh_data->IntegralReflList().size() - 1;
    fi2->Q() = mesh_data->IntegralReflList()[diff_idx];
  }
  BaseAlgorithm::MixAlgWithConvex(mesh_data, lq::kRight);
  BaseAlgorithm::GenerateFinalScalarField(mesh_data, flag);
  end = clock();
  io::ConsoleMessage<string>::SendMessage("", io::kEndAlg, lq::kNull, mesh_data->MaxE(),
                                          difftime(end, start)/CLOCKS_PER_SEC);
}

}
}
