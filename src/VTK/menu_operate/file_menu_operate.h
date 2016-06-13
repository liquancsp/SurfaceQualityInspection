/**
 *@class FileMenuOperate
 *@brief the function of this class is to consistent with the file menu item,
 *this class is to open a obj file and initialize some datas.
 *
 *@detail
 *
 *@data 2013-2-28
 *@version 1.0
 */

#ifndef FILE_MENU_OPERATE_H
#define FILE_MENU_OPERATE_H
#if defined(_MSC_VER)
#include <memory>
#endif

#if defined(__GNUC__)
#include <tr1/memory>
#endif
#include <data_type.h>
#include <MeshData.h>
#include <vtkSmartPointer.h>
#include <vtkMath.h>
namespace lq {

class vtk_model_data;

class FileMenuOperate {

public:
    FileMenuOperate();
    
    ~FileMenuOperate();
    void SetDataSet(const vtkSmartPointer<vtk_model_data> &other_data_set);
    /* int create_render(const std::string &file_name, const int flag, */
    /*                   QVTKWidget *qvtk_widget); */
    void SetMeshData(const std::tr1::shared_ptr<sqi::meshdata::MeshData<sqi::meshdata::Mesh> >& other)
    {
        this->mesh_data_ = other;
    }
    int LoadFile(const std::string &file_name, const int flag);
    int CreateRender(sqi::meshdata::MeshData<sqi::meshdata::Mesh>& mesh,
                     lq::WindowFlag flag);
    int SetRenderData(sqi::meshdata::MeshData<sqi::meshdata::Mesh>& mesh,
                      lq::WindowFlag flag);
    void SetLightParameter(const lq::WindowFlag win);

private:

    FileMenuOperate(const FileMenuOperate &other);
    FileMenuOperate& operator = (const FileMenuOperate &other);
    void SetLightProperty(const lq::WindowFlag flag);
    
    vtkSmartPointer<vtk_model_data> data_set_;
    sqi::meshdata::MeshDataPtr mesh_data_;

    fstream fout;
    
};

}

#endif
