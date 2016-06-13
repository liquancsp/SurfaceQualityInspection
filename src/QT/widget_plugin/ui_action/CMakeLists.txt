cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS -fpermissive)
FIND_PACKAGE(VTK)
INCLUDE(${VTK_USE_FILE})
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} )
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/IO)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/VTK)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/QT)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/Common)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/3rd)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/Algorithm)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/MeshData)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/Color)
#SET(ViewSrcs main_panel.cpp model_win.cpp)
FILE(GLOB ViewSrcs *.cpp)
#SET(ViewHeaders  main_panel.h model_win.h configure.h)
FILE(GLOB ViewHeaders *.h)

QT4_WRAP_UI(UISrcs ${ViewUI})
QT4_WRAP_CPP(MOCSrcs ${ViewHeaders})

#ADD_LIBRARY(ui_widget_action  ${ViewSrcs} ${UISrcs} ${MOCSrcs})
ADD_LIBRARY(ui_widget_action  ${ViewSrcs} ${UISrcs} ${MOCSrcs})
target_link_libraries(ui_widget_action
  ui_menu_action
  ui_widget_resource
  vtk_operate
  widget_operate
  # diff
  #vtkWidgets 
  #vtkHybrid
)
