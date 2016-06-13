#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

#include <Facade/MenuOperator.h>
#include <Algorithms/CalHeightMap.h>
#include <Algorithms/ZernikeCoefficient.h>
#include <Algorithms/CalHeightMapZBuffer.h>
#include <Algorithms/ZernikerPropagation.h>
#include <Algorithms/Propagation.h>
#include <vtk_data/vtk_model_data.h>
#include <MeshData.h>

using namespace std;
using namespace sqi;
void print_file(const string& file_name)
{
  std::cout << file_name << endl;
}

#define KDTREE_SPLIT
void Console(int argc, char* argv[])
{
  sqi::meshdata::MeshDataPtr mesh_data(new sqi::meshdata::MeshData<sqi::meshdata::Mesh>);
  vtkSmartPointer<lq::vtk_model_data> data_set = 
      vtkSmartPointer<lq::vtk_model_data>::New();
  std::auto_ptr<sqi::common::MenuOperator> opt(new sqi::common::MenuOperator(mesh_data,
                                                                             data_set));
  if(argc == 3)
  {
    string file_name(argv[2]);
    string opt_type(argv[1]);
   
   
    opt->LoadModelFile(file_name, lq::kLeft);
    opt->SetRenderData(lq::kLeft);
    std::auto_ptr<sqi::alg::CalHeightMap> height_map(new sqi::alg::CalHeightMap());
    std::auto_ptr<sqi::alg::CalHeightMapZBuffer> z_buffer(new sqi::alg::CalHeightMapZBuffer());
    std::auto_ptr<sqi::alg::ZernikeCoefficient> zerniker_coeff(new sqi::alg::ZernikeCoefficient());
    double bounds[6];
    data_set->PD(lq::kLeft)->GetBounds(bounds);
    vector<vector<double> > height_map_vec;
    vector<vector<double> > zerniker_coeff_vec;
    clock_t start, end;
    start = clock();
    height_map->GetTimer()->StartTimer();
    if(strcmp(opt_type.c_str(), "-hmapkd") == 0)
    {
      data_set->BuildKdTree(lq::kLeft);
      height_map_vec.clear();
      string kdtree_out("kdtree_out_split.txt");
      height_map->SetDataSet(data_set->PD(lq::kLeft));
#ifdef KDTREE_SPLIT
      height_map->CreateKDTree(data_set->PD(lq::kLeft));
      height_map->CalMeshHeightMapKDTreeSplit(mesh_data, kdtree_out, data_set->KdTree(), 
                                              bounds, lq::kLeft, height_map_vec);
#else
      height_map->CalMeshHeightMapKDTree(mesh_data, kdtree_out, data_set->KdTree(), 
                                         bounds, lq::kLeft, height_map_vec);
#endif
    }
    else if(strcmp(opt_type.c_str(), "-hmapbsp") == 0)
    {
      data_set->BuildBSPTree(lq::kLeft);
      height_map_vec.clear();
      string bsp_tree_out("bsp_tree_out.txt");
      height_map->CreateModifyTree(data_set->PD(lq::kLeft));
      height_map->CalMeshHeightMapBSPTree(mesh_data, bsp_tree_out, data_set->BSPTree(), 
                                          bounds, lq::kLeft, height_map_vec);
    }
    else if(strcmp(opt_type.c_str(), "-hmapobb") == 0)
    {
      data_set->BuildOBBTree(lq::kLeft);
      height_map_vec.clear();
      string obb_tree_out("obb_tree_out.txt");
      height_map->CalMeshHeightMapOBBTree(mesh_data, obb_tree_out, data_set->OBBTree(), 
                                          bounds, lq::kLeft, height_map_vec);
    }
    else if(strcmp(opt_type.c_str(), "-hmapzb") == 0) //z-buffer
    {
      height_map_vec.clear();
      string zbuffer_out("z_buffer_out.txt");
      z_buffer->HeightMapZBuffer(mesh_data, zbuffer_out, data_set->PD(lq::kLeft),
                                 bounds, lq::kLeft, height_map_vec);
    }
    
    //      zerniker_coeff->CalZernikeCoeff(height_map_vec, bounds, zerniker_coeff_vec);
    zerniker_coeff->CalZernikeCoeffWithGaussianWeight(mesh_data->VBeginIter(lq::kLeft),
                                                      data_set->KdTree(), height_map_vec,
                                                      bounds, static_cast<double>(height_map->GetRadius()),
                                                      zerniker_coeff_vec);
    cout << "[#Debug]Height map radius : " << height_map->GetRadius() << endl;
    double tmp_dist = (mesh_data->M(lq::kLeft).vert[43].P() - mesh_data->M(lq::kLeft).vert[2656].P()).Norm();
    cout << "tmp_dist : " << tmp_dist << endl;
    std::auto_ptr<sqi::alg::Propagation> propagate(new sqi::alg::Propagation());
    propagate->PropagateLocalExemplar(mesh_data, zerniker_coeff_vec, data_set->KdTree(),
                                      lq::kLeft, 0.1, 0.8, 0.003, 7);
    end = clock();
    height_map->GetTimer()->StopTimer();
    cout << "[#Info]Vtk timer total cost time is : " << height_map->GetTimer()->GetElapsedTime() << endl;
    cout << "[#Info]Total cost time is : " << difftime(end, start) / CLOCKS_PER_SEC << endl;
  }
  else if(argc == 5)
  {
    string opt_type(argv[1]);
    if(strcmp(opt_type.c_str(), "-test") == 0)
    {
      string in_file(argv[3]), out_file(argv[4]);
      opt->LoadModelFile(in_file, lq::kLeft);
      opt->SetRenderData(lq::kLeft);
      sqi::alg::ZernikerPropagation::SetDataSet(data_set);
      std::auto_ptr<sqi::alg::ZernikerPropagation> zerniker_propatate(new sqi::alg::ZernikerPropagation());
      mesh_data->RadiusRatio() = 1.0f;
      mesh_data->HeightRadiusMulti() = 1.0f;
//      zerniker_propatate->CreateTestData(mesh_data, out_file, lq::kLeft, idx_list);
      zerniker_propatate->CreateTestData(mesh_data, out_file, lq::kLeft);
    }
    else if(strcmp(opt_type.c_str(), "-dist") == 0) // ./console -dist input_file idx1 idx2
    {
      string in_file(argv[2]), idx1_str(argv[3]), idx2_str(argv[4]);
      opt->LoadModelFile(in_file, lq::kLeft);
      opt->SetRenderData(lq::kLeft);
      double bound[6];
      data_set->PD(lq::kLeft)->GetBounds(bound);
      vcg::Point3f left_bottom(bound[0], bound[2], bound[4]);
      vcg::Point3f right_top(bound[1], bound[3], bound[5]);
      double diagonal_len = vcg::Distance(left_bottom, right_top);
      double rect_edge = (2.5 * diagonal_len) / 100.0f;
      double radius = rect_edge / 2.0f;
      std::cout << "[#Info]Search radius is : " << radius << endl;
      int idx1 = atoi(idx1_str.c_str()), idx2 = atoi(idx2_str.c_str());
      sqi::meshdata::VertexIterator vi1, vi2;
      vi1 = mesh_data->VBeginIter(lq::kLeft) + idx1;
      vi2 = mesh_data->VBeginIter(lq::kLeft) + idx2;
      double dist_square = vcg::Distance<float>(vi1->P(), vi2->P());
      cout << "[#Info]Distance between " << idx1 << " and " << idx2 << " is : " << dist_square << endl;
    }
  }
  else if(argc == 6)// ./console -train type in_file idx_file out_file
  {
    string in_file(argv[3]), idx_file(argv[4]), out_file(argv[5]);
    string opt_type(argv[1]), type(argv[2]);
    opt->LoadModelFile(in_file, lq::kLeft);
    opt->SetRenderData(lq::kLeft);
    sqi::alg::ZernikerPropagation::SetDataSet(data_set);
    std::auto_ptr<sqi::alg::ZernikerPropagation> zerniker_propatate(new sqi::alg::ZernikerPropagation());
    if(strcmp(opt_type.c_str(), "-train") == 0)
    {
      vector<int> idx_list;
      ifstream fin(idx_file.c_str());
      string line;
      while(getline(fin, line) != NULL)
      {
        idx_list.push_back(atoi(line.c_str()));
      }
      fin.close();
//      zerniker_propatate->CreateTrainningData(mesh_data, out_file, lq::kLeft, 
//                                              static_cast<sqi::io::ClassifyType>(atoi(type.c_str())),
//                                              idx_list);
      fin.open(in_file.c_str());
      vector<string> str_list;
      while(!fin.eof())
      {
        getline(fin, line);
        str_list.push_back(line);
      }
      fin.close();
      cout << "[#Info]Idx_list size : " << idx_list.size() << endl;
      ofstream fout(out_file.c_str());
      for(int i = 0; i < idx_list.size(); ++i)
      {
        string tmp;
        int flag = str_list[idx_list[i]].find(" ");
        tmp = str_list[idx_list[i]].substr(flag + 1);
        tmp = type + " " + tmp;
        fout << tmp << endl;
      }
      fout.close();
    }
  }
  return;
}
int main(int argc, char* argv[])
{
    Console(argc, argv);
}
