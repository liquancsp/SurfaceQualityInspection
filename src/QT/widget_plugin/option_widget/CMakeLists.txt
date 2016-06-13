cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS -fpermissive)
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})
FIND_PACKAGE(VTK)
INCLUDE(${VTK_USE_FILE})
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} )
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/IO)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/VTK)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/QT)
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

ADD_LIBRARY(ui_option_widget  ${ViewSrcs} ${UISrcs} ${MOCSrcs})
#if(MINGW)
target_link_libraries(ui_option_widget
QtGui
QtCore
vtk_model_data
)
#else()
#target_link_libraries(ui_option_widget
#   ${QT_LIBRARY_DIR}/QtCore4.lib
#   ${QT_LIBRARY_DIR}/QtGui4.lib
#   vtk_model_data
#)
#endif()
