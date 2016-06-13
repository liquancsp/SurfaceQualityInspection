#ifndef REGISTATTRIBUTE_H
#define REGISTATTRIBUTE_H

#include<vcg/complex/complex.h>

namespace sqi {

namespace meshdata {

template<class MeshType>
class RegistAttribute {
  
public:
  
  typedef typename MeshType::VertexType VertexType;
  
//  static void RegistMeanCurDir(MeshType& m)
//  {
//    vcg::tri::Allocator<MeshType>::GetPerVertexAttribute<VertexType>(m, std::string("MeanCurDir"));
//  }
  
//  static void RegistMaxError(MeshType& m)
//  {
//    vcg::tri::Allocator<MeshType>::GetPerMeshAttribute<double>(m, std::string("MaxError"));
//  }
  
};

}

}

#endif // REGISTATTRIBUTE_H
