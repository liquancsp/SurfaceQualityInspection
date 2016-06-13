#ifndef CALLINEINTERSECTWITHMESH_H
#define CALLINEINTERSECTWITHMESH_H

#include <vtkOBBTree.h>
#include <vtkSmartPointer.h>
#include <MeshData.h>
#include <vtkModifiedBSPTree.h>

namespace sqi {
namespace alg {

class CalLineIntersectWithMesh{

public :
    CalLineIntersectWithMesh();
    ~CalLineIntersectWithMesh();  
    
    bool CalMinIntersect(vtkSmartPointer<vtkOBBTree>& tree,
                         double p0[3], double p1[3], 
                         double center[3], double& res);
    bool CalMinIntersect(vtkSmartPointer<vtkModifiedBSPTree>& tree, 
                         double p0[3], double p1[3], double center[3], double &res);
    
    bool CalMinIntersect(vtkSmartPointer<vtkOBBTree>& tree,
                         float p0x, float p0y, float p0z,
                         float p1x, float p1y, float p1z,
                         float cx, float cy, float cz, double& res);
    
    bool CalMinIntersect(vtkSmartPointer<vtkModifiedBSPTree>& tree,
                         float p0x, float p0y, float p0z,
                         float p1x, float p1y, float p1z,
                         float cx, float cy, float cz, double& res);
private : 
    
  
};
}
}
#endif // CALLINEINTERSECTWITHMESH_H
