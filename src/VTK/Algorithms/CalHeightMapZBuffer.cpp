#include <CalHeightMapZBuffer.h>
#include <CurvatureAlg.h>

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkCamera.h>
#include <vtkMath.h>

#include <cmath>

namespace sqi {
namespace alg {
using namespace std;

CalHeightMapZBuffer::CalHeightMapZBuffer()
{}

CalHeightMapZBuffer::~CalHeightMapZBuffer()
{
  
}

void CalHeightMapZBuffer::HeightMapZBuffer(meshdata::MeshDataPtr &mesh_data, string &out_name,
                                           vtkSmartPointer<vtkPolyData> &poly,
                                           double bound[], lq::WindowFlag win, 
                                           std::vector<std::vector<double> > &height_map)
{
  CurvatureAlg<meshdata::Mesh>::PrincipalCurWithDirection(mesh_data->M(win));
  vtkSmartPointer<vtkPolyDataMapper> mapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
  vtkSmartPointer<vtkActor> actor =
      vtkSmartPointer<vtkActor>::New();
#if VTK_MAJOR_VERSION <= 5
    mapper->SetInputConnection(poly->GetProducerPort());
#else
    mapper->SetInputData(poly);
#endif
    actor->SetMapper(mapper);
    
  double left_bottom[3] = {bound[0], bound[2], bound[4]};
  double right_top[3] = {bound[1], bound[3], bound[5]};
  double diagonal_len = sqrt(vtkMath::Distance2BetweenPoints(left_bottom, right_top));
  double rect_edge = (2.5 * diagonal_len) / 100.0f;
  double radius = rect_edge / 2.0f;
  double e = rect_edge / 16;
  vtkSmartPointer<vtkCamera> camera;
  height_map.resize(mesh_data->VNum(win));
  vector<bool> flag(256, false);
  SurfaceDescriptor::CalGridFlag(flag, e, radius);
  ofstream fout("camera_info.txt");
  for(int i = 0; i < 2; ++i)
  {
    vtkSmartPointer<vtkRenderWindow> renWin =
        vtkSmartPointer<vtkRenderWindow>::New();
    vtkSmartPointer<vtkRenderer> renderer =
        vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);
    meshdata::VertexIterator vi = mesh_data->VBeginIter(win) + i;
    vcg::Point3f camera_position = vi->N() * diagonal_len + vi->P();
    camera = renderer->GetActiveCamera();
    camera->SetPosition(camera_position[0], camera_position[1], camera_position[2]);
    camera->ParallelProjectionOn();
//    camera->SetFocalPoint(0, 0, 0);
    camera->SetFocalPoint(vi->P()[0], vi->P()[1], vi->P()[2]);
    camera->SetViewUp(vi->PD1()[0], vi->PD1()[1], vi->PD1()[2]);
//    camera->SetViewUp(1, 1, 0);
    camera->SetParallelScale(rect_edge);
    double near = diagonal_len, far = 2 * diagonal_len;
    camera->SetClippingRange(near, far);
    renWin->AddRenderer(renderer);
    renWin->SetSize(16, 16);
    renWin->Render();
    fout << "camera : " << camera_position[0] << " " << camera_position[1] << " "
            << camera_position[2] << endl;
    fout << "up : " << vi->PD1()[0] << " " << vi->PD1()[1] << " " << vi->PD1()[2] << endl;
    fout << "focal : " << vi->P()[0] << " " << vi->P()[1] << " " << vi->P()[2] << endl;
    fout << "rect_edge : " << rect_edge / 2 << endl;
    fout << "near : " << near << endl << "far : " << far << endl;
    for(int j = 0; j < 16; ++j)
      for(int k = 0; k < 16; ++k)
      {
        double b = renderer->GetZ(j, k);
        if(b == 1.0 || flag[j * 16 + k] == false)
          b = -1.0f;
        height_map[i].push_back(b);
      }
  }
  fout.close();
  SurfaceDescriptor::WriteFile(height_map, out_name);
}
}

}
