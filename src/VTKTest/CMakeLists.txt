cmake_minimum_required(VERSION 2.8)
 
PROJECT(test_vtk)
 
find_package(VTK REQUIRED)
find_package(PCL REQUIRED)
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

include(${VTK_USE_FILE})
 
add_executable(test_vtk MACOSX_BUNDLE test_vtk)
 
if(VTK_LIBRARIES)
  target_link_libraries(test_vtk ${VTK_LIBRARIES} ${PCL_LIBRARIES})
else()
  target_link_libraries(test_vtk vtkHybrid vtkWidgets ${PCL_LIBRARIES})
endif()
