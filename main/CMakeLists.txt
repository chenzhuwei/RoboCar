project(CAR_EYE)
set(CAR_EYE main.cpp)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

cmake_minimum_required(VERSION 3.0)
FIND_PACKAGE(OpenCV REQUIRED)
add_executable(CAR_EYE main.cpp)
link_directories(./ekho/lib64 ${PROJECT_BINARY_DIR}/libs ${PROJECT_BINARY_DIR}/lib /usr.local/libs /usr/lib)
target_link_libraries(
        CAR_EYE
        ${OpenCV_LIBS}
        Thread
        pthread
        #vorbis
        #vorbisenc
        config
        window
        ${GTK3_LIBRARIES}
        log
        jsoncpp_lib_static
)