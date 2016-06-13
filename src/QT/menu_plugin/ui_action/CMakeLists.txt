cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS -fpermissive)
find_package(OpenMP)
if(OPENMP_FOUND)
message("algorithm openmp")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()
FIND_PACKAGE(VTK)
INCLUDE(${VTK_USE_FILE})
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} )
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/IO)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/VTK)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/QT)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/Common)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/MeshData)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/Algorithm)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/3rd)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/Color)
FILE(GLOB ViewSrcs *.cpp)
#FILE(GLOB ViewUI *.ui)
FILE(GLOB ViewHeaders *.h)

QT4_WRAP_UI(UISrcs ${ViewUI})
QT4_WRAP_CPP(MOCSrcs ${ViewHeaders})

ADD_LIBRARY(ui_menu_action  ${ViewSrcs} ${UISrcs} ${MOCSrcs})
#ADD_LIBRARY(ui_menu_action SHARED ${ViewSrcs} ${UISrcs} ${MOCSrcs})
target_link_libraries(ui_menu_action
  SqiAlg
  SqiCommonFacade
  SqiMultiThread
  vtk_operate
  widget_operate
  ui_menu_resource
  ui_widget_action
  ui_option_widget
  QtXml
  )
