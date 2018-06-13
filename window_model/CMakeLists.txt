cmake_minimum_required(VERSION 3.0)
project(CAR_EYE)
FIND_PACKAGE(OpenCV REQUIRED)
#add GTK+2
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})
#add gtkmm
pkg_check_modules(GTKMM gtkmm-3.0)
link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 11)
set(window mainWindow.cpp)
add_library(window SHARED ${window})
target_link_libraries(window ${GTKMM_LIBRARIES} )
target_link_libraries(window serialModel ${OpenCV_LIBS} ${GTK3_LIBRARIES} log Thread config )
