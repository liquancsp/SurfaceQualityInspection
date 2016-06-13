#include <TrainAndPredict.h>
#include <iostream>
#include <Python.h>
using namespace std;
namespace sqi{
namespace alg{

TrainAndPredict::TrainAndPredict() : BaseAlgorithm()
{
  
}

TrainAndPredict::~TrainAndPredict()
{
  
}

void TrainAndPredict::CalSingleProperty(meshdata::MeshDataPtr &mesh_data, 
                                        lq::InspectFlag flag, 
                                        lq::WindowFlag win)
{
  cout << "[#Info]Train and predict algorithm ! " << endl;
  Py_Initialize();
  if(!Py_IsInitialized())
  {
    cerr << "[#Error]Fail to initialize!" << endl;
    return;
  }
  PyRun_SimpleString("import sys");
  int flag_slide = mesh_data->PythonFilePath().find_last_of('/');
  int flag_dot = mesh_data->PythonFilePath().find_last_of('.');
  string python_file_name = mesh_data->PythonFilePath().substr(flag_slide + 1, 
                                                               flag_dot - flag_slide - 1);
  string python_file_path = mesh_data->PythonFilePath().substr(0, flag_slide + 1);
  cout << "[#Info]Python file name is : " << python_file_name <<
          "  python file path : " << python_file_path << endl;
  
  string path = "sys.path.append('" + python_file_path + "')";
  cout << "[#Info]Path : " << path << endl;
  PyRun_SimpleString(path.c_str());
  PyObject *pName, *pModule, *pDict, *pFunc, *py_args;
  pName = PyString_FromString(python_file_name.c_str());
  pModule = PyImport_Import(pName);
  if (!pModule)
  {
    cerr << "[#Error]Can not find " << path << endl;
    return;
  }
  pDict = PyModule_GetDict(pModule);
  if (!pDict)
  {
    cerr << "[#Error]Python module load fail!" << endl;
    return;
  }
  pFunc = PyDict_GetItemString(pDict, "Process");
  if (!pFunc || !PyCallable_Check(pFunc))
  {
    printf("can't find function [Process]");
    return;
  }
  //create one parameter for function 
  py_args = PyTuple_New(1);
  //  PyObject* Py_BuildValue(char *format, ...)
  //  convert a c++ object to a python object. 
  //  When you want to pass arguments from c++ to
  //  python. You can use Py_BuildValue
  //  s : string,
  //  i : int，
  //  f : float,
  //  O : a python object
  PyTuple_SetItem(py_args, 0, Py_BuildValue("s", python_file_path.c_str()));
  PyObject_CallObject(pFunc, py_args);
  Py_DECREF(pName);
  Py_DECREF(pModule);
  // Close Python
  Py_Finalize();
  return;
}

}
}
