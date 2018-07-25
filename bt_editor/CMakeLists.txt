cmake_minimum_required(VERSION 3.2)

project(behavior_tree_editor)
add_definitions(-std=c++11  -pthread)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")



#if(APPLE AND EXISTS /usr/local/opt/qt5)
#	# Homebrew installs Qt5 (up to at least 5.9.1) in
#	# /usr/local/qt5, ensure it can be found by CMake since
#	# it is not in the default /usr/local prefix.
#	list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt5")
#endif()
#SET(Qt5_DIR C:/Program Files (x86)/robotology/Qt-5.6.1_v14_x86)


find_package(Qt5 COMPONENTS Core Widgets Gui OpenGL Xml)


#########################################################
# FIND Lua
#########################################################
find_package(Lua REQUIRED)
IF(Lua_FOUND)
ELSE(Lua_FOUND)
message(WARNING "Lua not found!")
ENDIF(Lua_FOUND)



INCLUDE_DIRECTORIES(${VREP_API_DIR})

find_package( Threads )


set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_definitions(${Qt5Widgets_DEFINITIONS} -DNODE_EDITOR_STATIC -std=c++11)
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")


INCLUDE_DIRECTORIES(${catkin_INCLUDE_DIRS} ./qt_nodeditor/include )
INCLUDE_DIRECTORIES(${BT_CORE_INCLUDE_DIR})
INCLUDE_DIRECTORIES(./bt_editor/)




file(GLOB_RECURSE LIB_CPPS  ./qt_nodeditor/src/*.cpp )
file(GLOB_RECURSE APP_CPPS  ./bt_editor/*.cpp )

qt5_add_resources(RESOURCES ./qt_nodeditor/resources/resources.qrc)


set(Qt5Core_DIR "C:/Qt/5.9.1/mingw53_32/lib/cmake/Qt5Core") ##TODO FIX THIS


# Tell CMake to create the helloworld executable
add_library(qt_node_editor STATIC ${LIB_CPPS} ${RESOURCES})

target_link_libraries(qt_node_editor
                      Qt5::Core  Qt5::Widgets
                      Qt5::Gui   Qt5::OpenGL
                      Qt5::Xml  )

QT5_WRAP_UI(FORMS_HEADERS
  ./bt_editor/mainwindow.ui
  )


add_executable(youbot_behavior_tree_editor ${APP_CPPS} ${FORMS_HEADERS} )
target_link_libraries(youbot_behavior_tree_editor LINK_PUBLIC COREBTLIBRARY qt_node_editor ${CMAKE_THREAD_LIBS_INIT})

