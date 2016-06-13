#include <ZernikerClassify.h>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
using namespace std;

namespace sqi{
namespace alg{

ZernikerClassify::ZernikerClassify()
{
}

ZernikerClassify::~ZernikerClassify()
{
  
}

void ZernikerClassify::CalSingleProperty(meshdata::MeshDataPtr &mesh_data, 
                                         lq::InspectFlag flag, lq::WindowFlag win)
{
  cout << "[#Info-Alg]Load predict file!" << endl;
  ifstream fin(mesh_data->PredictFile().c_str());
  string line;
  meshdata::VertexIterator vi = mesh_data->VBeginIter(win);
  while(!fin.eof())
  {
    getline(fin, line);
    vi->Q() = static_cast<double>(atoi(line.c_str()));
    ++vi;
  }
}

}
}
