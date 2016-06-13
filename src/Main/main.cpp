#include <QApplication>
#include <QFile>
#include <QtXml/QDomDocument>

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

#include <widget_plugin/ui_action/main_windows.h>
#include <Topology/UpdateRingNeighbor.h>
#include <Facade/MenuOperator.h>
#include <Topology/Geodesic.h>

using namespace std;
using namespace sqi;
/*
void print_file(const string& file_name)
{
  std::cout << file_name << endl;
}

void Console(int argc, char* argv[])
{
  if(argc == 2)
  {
    QFile xml(argv[1]);
    if(!xml.open(QFile::ReadOnly))
      return ;
    QDomDocument doc;
    vector<string> file_name;
    if(!doc.setContent(&xml))
    {
      xml.close();
      return ;
    }
    QDomNodeList node_list = doc.elementsByTagName("project");
    for(size_t i = 0; i < node_list.count(); ++i)
    {
      QDomElement element = node_list.at(i).toElement();
      QDomNode node = element.firstChild();
      while(!node.isNull())
      {
        QDomElement tmp = node.toElement();
        QString tag_name = tmp.tagName();
        if(tag_name == "model")
          file_name.push_back(qPrintable(tmp.text()));
        node = node.nextSibling();
      }
      
    }
    for_each(file_name.begin(), file_name.end(), print_file);
  }
  else if(argc == 3 || argc == 4)
  {
    string file_name(argv[2]);
    string opt_type(argv[1]);
    sqi::meshdata::MeshDataPtr mesh_data(new sqi::meshdata::MeshData<sqi::meshdata::Mesh>);
    vtkSmartPointer<lq::vtk_model_data> data_set = 
        vtkSmartPointer<lq::vtk_model_data>::New();
    std::auto_ptr<sqi::common::MenuOperator> opt(new sqi::common::MenuOperator(mesh_data,
                                                                               data_set));
    opt->LoadModelFile(file_name, lq::kLeft);
    sqi::meshdata::VertexType* vert_ptr = &mesh_data->M(lq::kLeft).vert[0];
    sqi::meshdata::FaceType* face_ptr = &mesh_data->M(lq::kLeft).face[2];
    std::set<int> neighbor_list;
    std::vector<int> neigh_list;
    if(strcmp(opt_type.c_str(), "-vv") == 0)
    {
      sqi::alg::UpdateRigngNeighbor<sqi::meshdata::Mesh>::VertexOneRingVertexNeighborPos(mesh_data->M(lq::kLeft),
                                                                                         vert_ptr, neighbor_list);
    }
    else if(strcmp(opt_type.c_str(), "-vvjp") == 0)
    {
      sqi::alg::UpdateRigngNeighbor<sqi::meshdata::Mesh>::VertexOneRingVertexNeighborJP(mesh_data->M(lq::kLeft),
                                                                                        vert_ptr, neighbor_list);
    }
    else if(strcmp(opt_type.c_str(), "-vf") == 0)
    {
      sqi::alg::UpdateRigngNeighbor<sqi::meshdata::Mesh>::VertexOneRingFaceNeighborVF(mesh_data->M(lq::kLeft),
                                                                                      vert_ptr, neighbor_list);
    }
    else if(strcmp(opt_type.c_str(), "-ff") == 0)
    {
      sqi::alg::UpdateRigngNeighbor<sqi::meshdata::Mesh>::FaceOneRingFaceNeighborFF(mesh_data->M(lq::kLeft),
                                                                                    face_ptr, neighbor_list);
    }
    else if(strcmp(opt_type.c_str(), "-rff") == 0)
    {
      if(argc == 3)
        return;
      double radius = atof(argv[3]);
      alg::Geodesic<meshdata::Mesh> dijsktra;
      dijsktra.FaceDijsktraCompute(mesh_data->M(lq::kLeft),face_ptr, radius,
                                   neigh_list);
    }
    std::cout << "neigh number : " << neigh_list.size() << endl;
    for(vector<int>::iterator iter = neigh_list.begin();
        iter != neigh_list.end(); ++iter)
      cout << *iter << endl;
    std::cout << "neighbor number : " << neighbor_list.size() << endl;
    for(set<int>::iterator iter = neighbor_list.begin();
        iter != neighbor_list.end(); ++iter)
      cout << *iter << endl;
  }
  return;
}
*/
int main(int argc, char* argv[])
{

  if(argc == 1)
  {
    QApplication app(argc, argv);
    //Create the SimpleVTK Main Window and execute it...
    //load the qss file to modify the theme of the windows
    QFile qss("./stylesheet.qss"); 
    qss.open(QFile::ReadOnly);
    app.setStyleSheet(qss.readAll());
    qss.close();
    lq::main_windows main_panel;
    main_panel.show(); 
    
    return app.exec();
  }
  /*
  else 
    Console(argc, argv);
	*/
}
