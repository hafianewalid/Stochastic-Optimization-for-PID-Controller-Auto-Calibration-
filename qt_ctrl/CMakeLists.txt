### CMake #################################################################
cmake_minimum_required(VERSION 2.8.0)
project(qt_ctrl)

### Catkin ################################################################

# Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

# qt_build provides the qmake-cmake glue, roscpp the comms 
find_package(catkin REQUIRED COMPONENTS qt_build roscpp nav_msgs)

## Declare ROS messages (to get odometry) - not needed, in fact
#add_message_files(DIRECTORY msg FILES Num.msg)
## Generate added messages (should not be needed)
#generate_messages(DEPENDENCIES std_msgs)

# relative paths are OK (not with link_directories)
include_directories(${catkin_INCLUDE_DIRS} include ISeeML/include)

# Use this to define what the package will export (e.g. libs, headers).
# Since the default here is to produce only a binary, we don't worry about
# exporting anything. 
catkin_package()  # may need CATKIN_DEPENDS message_runtime

### Qt Environment ########################################################

# this comes from qt_build's qt-ros.cmake which is automatically 
# included via the dependency call in package.xml
## Qt4's core and gui packages are needed
rosbuild_prepare_qt4(QtCore QtGui) 

### ISeeML Library ########################################################

## target_link_libraries needs to know were to find libCompISeeML.a
# while include_directories perfectly handles relative paths,
# link_directories REQUIRES absolute paths to work correctly
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/ISeeML/build/linux)

# I found these possible additions if link_directories does not work:
#add_library(ISeeML SHARED IMPORTED) # or STATIC instead of SHARED
#set_target_properties(ISeeML PROPERTIES
#  IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/ISeeML/build/linux/libISeeML.a"
#  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/ISeeML"
#)

### Sections ##############################################################

## define variables (QT_FORMS, QT_RESOURCES, QT_MOC) for Qt compilation
#file(GLOB QT_FORMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} files/*.ui)
file(GLOB QT_RESOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} resources.qrc)
file(GLOB_RECURSE QT_MOC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} 
  FOLLOW_SYMLINKS include/*/*.hpp)

## transmit the values of these variables to Qt
QT4_ADD_RESOURCES(QT_RESOURCES_CPP ${QT_RESOURCES})
#QT4_WRAP_UI(QT_FORMS_HPP ${QT_FORMS})
QT4_WRAP_CPP(QT_MOC_HPP ${QT_MOC})

### Sources ###############################################################

## define QT_SOURCES for Qt compilation (not transmitted?)
file(GLOB_RECURSE QT_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} 
  FOLLOW_SYMLINKS src/*/*.cpp)

### Binaries ##############################################################

## to build our programm (gui), we need to compile all the following files
add_executable(gui ${QT_SOURCES} ${QT_RESOURCES_CPP} ${QT_FORMS_HPP}
  ${QT_MOC_HPP})

## needed libraries are Qt's, Catkin's and ISeeML's
target_link_libraries(gui ${QT_LIBRARIES} ${catkin_LIBRARIES} CompISeeML)
#install(TARGETS gui RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
