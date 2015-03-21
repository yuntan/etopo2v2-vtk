cmake_minimum_required(VERSION 2.8.4)
project(etopo2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(VTK COMPONENTS
  vtkCommonCore
  vtkRenderingQt
)
include(${VTK_USE_FILE})
set(SOURCE_FILES main.cpp)
add_executable(etopo2 ${SOURCE_FILES})
target_link_libraries(etopo2 ${VTK_LIBRARIES} )
