cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS -fpermissive)
find_package(PCL REQUIRED)
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})
find_package(OpenMP)
if(OPENMP_FOUND)
message("algorithm openmp")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})
FIND_PACKAGE(VTK)
INCLUDE(${VTK_USE_FILE})
link_directories(${VTK_LIBRARY_DIRS})
link_directories(${QT_LIBRARY_DIR})
SET (QT_USE_QTMAIN TRUE)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} )
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/IO)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/VTK)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/QT)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/Common)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/MeshData)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/3rd)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/Algorithm)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/Color)


SET(main_srcs console.cpp)
#SET(CMAKE_BUILD_TYPE = Debug)
ADD_EXECUTABLE(console ${main_srcs})
#if(MINGW)
TARGET_LINK_LIBRARIES(console
  SqiMeshData
  vtk_model_data
  SqiCommonFacade
  VtkAlgs
  SqiAlg
  )
#else()
#TARGET_LINK_LIBRARIES(SQI
#  ui_menu_action
#  ui_widget_action
#  QtXml4
#  )
#endif()
