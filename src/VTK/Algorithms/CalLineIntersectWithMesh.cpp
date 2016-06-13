#include <Algorithms/CalLineIntersectWithMesh.h>
#include <vtkMath.h>
#include <vtkPoints.h>

namespace sqi {
namespace alg {

CalLineIntersectWithMesh::CalLineIntersectWithMesh()
{
  
}

CalLineIntersectWithMesh::~CalLineIntersectWithMesh()
{ 
}

bool CalLineIntersectWithMesh::CalMinIntersect(vtkSmartPointer<vtkOBBTree>& tree,
                                               double p0[3], double p1[3], 
                                               double center[3], double& res)
{
  vtkSmartPointer<vtkPoints> intersect_list = 
      vtkSmartPointer<vtkPoints>::New();
  double tol = 1.e-8;
  tree->SetTolerance(tol);
  tree->IntersectWithLine(p0, p1, intersect_list, NULL);
  res = static_cast<double>(INT_MAX);
  if(intersect_list->GetNumberOfPoints() == 0)
    return false;
  double intersection[3];
  for(int i = 0; i < intersect_list->GetNumberOfPoints(); ++i)
  {
    intersect_list->GetPoint(i, intersection);
    double dist = sqrt(vtkMath::Distance2BetweenPoints(intersection, center));
    if(lq::is_larger(res, dist))
      res = dist;
  }
  return true;
}

bool CalLineIntersectWithMesh::CalMinIntersect(vtkSmartPointer<vtkModifiedBSPTree>& tree, 
                                               double p0[], double p1[], double center[],
                                               double &res)
{
  vtkSmartPointer<vtkPoints> intersect_list = 
      vtkSmartPointer<vtkPoints>::New();
  double tol = 1.e-8;
  tree->IntersectWithLine(p0, p1, tol, intersect_list, NULL);
  res = static_cast<double>(INT_MAX);
  if(intersect_list->GetNumberOfPoints() == 0)
    return false;
  double intersection[3];
  for(int i = 0; i < intersect_list->GetNumberOfPoints(); ++i)
  {
    intersect_list->GetPoint(i, intersection);
    double dist = sqrt(vtkMath::Distance2BetweenPoints(intersection, center));
    if(lq::is_larger(res, dist))
      res = dist;
  }
  return true;
}

bool CalLineIntersectWithMesh::CalMinIntersect(vtkSmartPointer<vtkOBBTree> &tree, 
                                               float p0x, float p0y, float p0z,
                                               float p1x, float p1y, float p1z,
                                               float cx, float cy, float cz, double &res)
{
  double p0[3] = {p0x, p0y, p0z};
  double p1[3] = {p1x, p1y, p1z};
  double center[3] = {cx, cy, cz};
  return CalMinIntersect(tree, p0, p1, center, res);
}

bool CalLineIntersectWithMesh::CalMinIntersect(vtkSmartPointer<vtkModifiedBSPTree>& tree, 
                                               float p0x, float p0y, float p0z, 
                                               float p1x, float p1y, float p1z, 
                                               float cx, float cy, float cz, double &res)
{
  double p0[3] = {p0x, p0y, p0z};
  double p1[3] = {p1x, p1y, p1z};
  double center[3] = {cx, cy, cz};
  return CalMinIntersect(tree, p0, p1, center, res);
}

}
}
