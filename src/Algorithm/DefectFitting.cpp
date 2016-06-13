#include <DefectFitting.h>
#include <CurvatureAlg.h>

namespace sqi{
namespace alg{
using namespace std;

DefectFitting::DefectFitting()
{
  
}

DefectFitting::~DefectFitting()
{
  
}

void DefectFitting::CalSingleProperty(meshdata::MeshDataPtr &mesh_data, 
                                      lq::InspectFlag flag, lq::WindowFlag win)
{
    if(!mesh_data->IsHasConvex(win)())
        CurvatureAlg<sqi::meshdata::Mesh>::MeanAndGaussian(mesh_data->M(win));
    CurvatureAlg<meshdata::Mesh>::PrincipalCurvatureAndDirection(mesh_data->M(win));
    if(!vcg::tri::HasFFAdjacency(mesh_data->M(win)))
        vcg::tri::UpdateTopology<sqi::meshdata::Mesh>::FaceFace(mesh_data->M(win));
    meshdata::Mesh::FaceIterator fi;
    std::vector<bool> visited(mesh_data->FNum(win), false);
    for(fi = mesh_data->FBeginIter(win); fi != mesh_data->FEndIter(win); ++fi)
    {
        //SearchCapArea(fi, mesh_data, win, visited);
        if(IsPrincipalZero(&(*fi)) 
                || lq::is_larger(FacePrincipalDiff(&(*fi)), 0.0015))
        {
            continue;
        }
        else 
            fi->Q() = 1.0f;
    }
    
}

void DefectFitting::SearchCapArea(meshdata::Mesh::FaceIterator fi,
                                  meshdata::MeshDataPtr &mesh_data, 
                                  lq::WindowFlag win, std::vector<bool> &visited)
{
    double threshold = 0.001f;
    int idx = mesh_data->GetIndexFace(&(*fi), win);
    if(IsPrincipalZero(&(*fi)) 
            || lq::is_larger(FacePrincipalDiff(&(*fi)), threshold))
    {
        return;
    }
    queue<meshdata::Mesh::FacePointer> q;
    q.push(&(*fi));
    while(!q.empty())
    {
        meshdata::Mesh::FacePointer fptr = q.front();
        fptr->Q() = 1.0f;
        q.pop();
        for(int i = 0; i < 3; ++i)
        {
            meshdata::Mesh::FacePointer adj_ptr = fptr->FFp(i);
            idx = mesh_data->GetIndexFace(adj_ptr, win);
            if(IsPrincipalZero(adj_ptr) 
                    || lq::is_larger(FacePrincipalDiff(adj_ptr), threshold))
            {
                continue;
            }
            else 
            {
                visited[idx] = true;
                q.push(adj_ptr);
            }
        }
    }
}

double DefectFitting::FacePrincipalDiff(meshdata::Mesh::FacePointer fp)
{
    double k1 = 0.0f, k2 = 0.0f;
    for(int i = 0; i < 3; ++i)
    {
        k1 += fp->cV(i)->K1();
        k2 += fp->cV(i)->K2();
    }
    k1 /= 3.0f;
    k2 /= 3.0f;
    return (k1 - k2);
}

bool DefectFitting::IsPrincipalZero(meshdata::Mesh::FacePointer fp)
{
    double k1 = 0.0f, k2 = 0.0f;
    for(int i = 0; i < 3; ++i)
    {
        k1 += fp->cV(i)->K1();
        k2 += fp->cV(i)->K2();
    }
    k1 /= 3.0f;
    k2 /= 3.0f;
    //std::cout << "k1 : " <<  k1 << "   k2 : " << k2 << std::endl;
    if(lq::is_zero(k1, 1e-6) && lq::is_zero(k2, 1e-6))
        return true;
    else 
        return false;
}

double DefectFitting::LenEdgeOfFace(meshdata::Mesh::FacePointer fp, 
                                    int edge_index)
{
  
}

}
}
