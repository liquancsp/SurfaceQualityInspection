cmake_minimum_required(VERSION 2.8)

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
#SET(ViewUI MainPanel.ui ModelView.ui)
FILE(GLOB ViewUI *.ui)
#SET(ViewHeaders  main_panel.h model_win.h configure.h)
FILE(GLOB ViewHeaders *.h)

#FILE(GLOB resource image_resource.qrc)
QT4_WRAP_UI(UISrcs ${ViewUI})
QT4_WRAP_CPP(MOCSrcs ${ViewHeaders})
link_directories(${PROJECT_SOURCE_DIR}/lib)
#QT4_ADD_RESOURCES(QT_RESOURCE ${resource})
ADD_LIBRARY(ui_menu_resource ${ViewSrcs} ${UISrcs} ${MOCSrcs})
#ADD_LIBRARY(ui_menu_resource SHARED ${ViewSrcs} ${UISrcs} ${MOCSrcs})
#if(MINGW)
target_link_libraries(ui_menu_resource
   #QVTK
   QtCore
   QtGui
)
#else()
#target_link_libraries(ui_menu_resource
#   #QVTK
#   ${QT_LIBRARY_DIR}/QtCore4.lib
#   ${QT_LIBRARY_DIR}/QtGui4.lib
#)
#endif()
