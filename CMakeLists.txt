cmake_minimum_required(VERSION 2.8)
 
project(lassy)
 
find_package(ITK REQUIRED)
find_package(VTK REQUIRED)

include(${ITK_USE_FILE})
include(${VTK_USE_FILE})

SET(LASSY_INCLUDES
	"./lassy++/include/"
)

add_subdirectory(lassy++/src) 
include_directories(${LASSY_INCLUDES})
add_executable(test_stub main.cxx)

target_link_libraries(test_stub lassy++ ${ITK_LIBRARIES} ${VTK_LIBRARIES})