#include <iostream>
#include <string>
#include <TrigonometryDisturb.h>
#include <SphereCapDisturb.h>
#include <ConvertFlatToCylinder.h>
#include <ConvertFlatToSphere.h>
#include <Algorithms/MeanCurvatureHistogram.h>

#include <Facade/MenuOperator.h>

#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkAxis.h>
#include <vtkTimerLog.h>
#include <vtkMath.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkIdList.h>
#include <vtkKdTreePointLocator.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkChartXY.h>
#include <vtkPlot.h>
#include <vtkTable.h>
#include <vtkFloatArray.h>
#include <vtkContextView.h>
#include <vtkContextScene.h>
#include <vtkSphereSource.h>

#include <vector>
#include <time.h>

using namespace sqi::utils;
using namespace sqi::alg;
using namespace sqi::meshdata;
using namespace std;

void Parser(int argc, char** argv)
{
  sqi::meshdata::MeshDataPtr mesh_data(new sqi::meshdata::MeshData<sqi::meshdata::Mesh>);
  vtkSmartPointer<lq::vtk_model_data> data_set = 
      vtkSmartPointer<lq::vtk_model_data>::New();
  std::auto_ptr<sqi::common::MenuOperator> opt(new sqi::common::MenuOperator(mesh_data,
                                                                             data_set));
  string op_type(argv[1]);
  string file_name(argv[2]);
  opt->LoadModelFile(file_name, lq::kLeft);
  opt->SetRenderData(lq::kLeft);
  if(argc == 4)
  {
    double radius = atof(argv[3]);
    //    opt->LoadModelFile(file_name, lq::kLeft);
    string out_file = file_name;
    int flag = out_file.find_last_of(".");
    if(strcmp(op_type.c_str(), "-cylinder") == 0)
    {
      
      std::tr1::shared_ptr<ConvertFlatToCylinder> cylinder(new ConvertFlatToCylinder());
      out_file.insert(flag, "-cylinder");
      cout << "[#Info]Out file is : " << out_file << endl;
      vcg::Point3f center;
      cylinder->Update(mesh_data, out_file, center, radius);
    }
    else if(strcmp(op_type.c_str(), "-sphere") == 0)
    {
      std::tr1::shared_ptr<ConvertFlatToSphere> sphere(new ConvertFlatToSphere());
      out_file.insert(flag, "-spheresurface");
      cout << "[#Info]Out file is : " << out_file << endl;
      vcg::Point3f center(0, 0, 0);
      sphere->Update(mesh_data, out_file, center, radius);
    }
  }
  else if(argc == 6)
  {
    if(strcmp(op_type.c_str(), "-sphere") == 0) // ./utils -sphere file_name idx radius dist
    {
      int idx = atoi(argv[3]);
      double radius = atof(argv[4]), dist = atof(argv[5]);
      std::tr1::shared_ptr<SphereCapDisturb> sphere_disturber(new SphereCapDisturb());
      //      opt->LoadModelFile(file_name, lq::kLeft);
      sqi::meshdata::VertexIterator vi = mesh_data->VBeginIter(lq::kLeft) + idx;
      
      //      vcg::Point3f sphere_center(vi->cP()[0], vi->cP()[1], vi->cP()[2] - dist);
      vcg::Point3f sphere_center = vi->cP() - vi->N() * dist;
      cout << "[#Info]Sphere center is : (" << sphere_center[0] << "," << sphere_center[1] <<
              "," << sphere_center[2] << ")" << endl;
      string out_file = file_name;
      int flag = out_file.find_last_of(".");
      out_file.insert(flag, "-sphere");
      cout << "[#Info]Out file is : " << out_file << endl;
      sphere_disturber->Update(mesh_data, out_file, sphere_center, radius);
    }
    //    ./utils -histogram file_name cur_type ratio idx
    else if(strcmp(op_type.c_str(), "-histogram") == 0)
    {
      int cur_type = atoi(argv[3]);
      cout << "[#Info] cur_type : " << cur_type << endl;
      double ratio = atof(argv[4]);
      mesh_data->RadiusRatio() = ratio;
      int idx = atoi(argv[5]);
      std::tr1::shared_ptr<MeanCurvatureHistogram> histogram_alg(new MeanCurvatureHistogram());
      vector<double> histogram;
      histogram_alg->Descriptor(mesh_data, data_set, lq::kLeft, idx, histogram, cur_type);
      cout << "[#Info]Vertex index is : " << idx << "  Histogram dim is : " << histogram.size() << endl;
      int num_dim = histogram.size();
      std::vector<std::pair<double, double> > results;
      double delta = 2.0f / static_cast<double>(num_dim);
      double begin = -1.0f;
      int cnt = 1;
      while(cnt <= num_dim)
      {
        double key = begin + cnt * delta;
        double t = histogram[cnt - 1];
        std::pair<double, double> result(key, t);
        results.push_back(result);
        ++cnt;
      }
      // Create a table with some points in it
      vtkSmartPointer<vtkTable> table =
          vtkSmartPointer<vtkTable>::New();
      
      vtkSmartPointer<vtkFloatArray> maxPointsPerRegion =
          vtkSmartPointer<vtkFloatArray>::New();
      maxPointsPerRegion->SetName("Curvature Range");
      table->AddColumn(maxPointsPerRegion);
      
      vtkSmartPointer<vtkFloatArray> runtime =
          vtkSmartPointer<vtkFloatArray>::New();
      runtime->SetName("Frequency");
      table->AddColumn(runtime);
      
      // Fill in the table with some example values
      size_t numPoints = results.size();
      table->SetNumberOfRows(numPoints);
      cout << "numPoints " << numPoints << endl;
      for(size_t i = 0; i < numPoints; ++i)
      {
        table->SetValue(i, 0, results[i].first);
        table->SetValue(i, 1, results[i].second);
      }
      // Set up the view
      vtkSmartPointer<vtkContextView> view =
          vtkSmartPointer<vtkContextView>::New();
      view->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
      
      // Add multiple line plots, setting the colors etc
      vtkSmartPointer<vtkChartXY> chart =
          vtkSmartPointer<vtkChartXY>::New();
      view->GetScene()->AddItem(chart);
      vtkPlot *line = chart->AddPlot(vtkChart::STACKED);
#if VTK_MAJOR_VERSION <= 5
      line->SetInput(table, 0, 1);
#else
      line->SetInputData(table, 0, 1);
#endif
      line->SetColor(0, 255, 0, 255);
      line->SetWidth(3.0);
      line->GetXAxis()->SetTitle("Curvature Range");
      line->GetYAxis()->SetTitle("Frequency");
      
      view->GetRenderWindow()->SetMultiSamples(0);
      
      // Start interactor
      view->GetInteractor()->Initialize();
      view->GetInteractor()->Start();
      return EXIT_SUCCESS;
    }
  }
  else if(argc == 7)
  {
    if(strcmp(op_type.c_str(), "-sine") == 0) //./utils -sine file_name radius center_idx amplitude phase
    {
      double radius = atof(argv[3]);
      int center_idx = atoi(argv[4]);
      double amplitude = atof(argv[5]), phase = atof(argv[6]);
      std::tr1::shared_ptr<SineFunctionDisturb> sine_disturber(new SineFunctionDisturb());
      
      //      opt->LoadModelFile(file_name, lq::kLeft);
      //      opt->SetRenderData(lq::kLeft);
      sine_disturber->GenerateDisturbList(mesh_data, data_set, center_idx, radius);
      string out_file = file_name;
      int flag = out_file.find_last_of(".");
      out_file.insert(flag, "-sine");
      cout << "[#Info]Out file is : " << out_file << endl;
      sine_disturber->Update(mesh_data, out_file, amplitude, phase);
    }
  }
}


int main(int argc, char** argv)
{
  Parser(argc, argv);
  return 0;
}
