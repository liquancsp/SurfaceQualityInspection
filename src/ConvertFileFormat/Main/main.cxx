//#include <string>
//#include <string.h>
//#include <vector>
//#include <algorithm>
//#include <iostream>
//#include <Facade/MenuOperator.h>
//#include <MeshData.h>
//#include <vtkSmartPointer.h>
//#include <vtk_data/vtk_model_data.h>
//#include <wrap/io_trimesh/export_obj.h>
#include <QMainWindow>
#include <QtGui>
using namespace std;
//using namespace sqi;


int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  QMainWindow tmp;
  tmp.show();
  return app.exec();
//  while(1)
//  {
//    cout << "Please input the file you want to convert: " << endl;

//    meshdata::MeshDataPtr mesh_data(new meshdata::MeshData<meshdata::Mesh>);
//    vtkSmartPointer<lq::vtk_model_data> data_set = 
//        vtkSmartPointer<lq::vtk_model_data>::New();
//    std::tr1::shared_ptr<sqi::common::MenuOperator> load_file(new common::MenuOperator(mesh_data,
//                                                                                       data_set));
//    string file_name;
//    cin >> file_name;
//    int flag = file_name.find_last_of(".");
//    string ext_type = file_name.substr(flag + 1);
//    if(strcmp(ext_type.c_str(), "nas") == 0)
//    {
//      load_file->LoadModelFile(file_name, lq::kLeft); 
//      file_name.erase(flag + 1);
//      file_name.insert(file_name.length(), "obj");
//      int mark = 0;
//      vcg::tri::io::ExporterOBJ<meshdata::Mesh>::Save(mesh_data->M(lq::kLeft), file_name.c_str(), mark);
//      cout << "[#Info]Convert successful" << endl;
//    }
//    else if(strcmp(ext_type.c_str(), "obj") == 0)
//    {
//      cout << "[#Info]The input file is .obj file. No need to convert." << endl;
//    }
//    else 
//    {
//      cout << "[#Error]Unknown file format." << endl;
//    }
//  }
//  return 0;
}
