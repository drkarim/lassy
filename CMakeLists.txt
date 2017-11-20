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
add_executable(lassy main.cxx)
add_executable(twall applications/wallthickness.cxx)
add_executable(surf applications/surf.cxx)
add_executable(surfvtk applications/surfvtk.cxx)
add_executable(shellshell applications/shellshell.cxx)
add_executable(shellenclosethickness applications/shellenclosethickness.cxx)
add_executable(target2source applications/target2source.cxx)


target_link_libraries(lassy lassy++ ${ITK_LIBRARIES} ${VTK_LIBRARIES})
target_link_libraries(twall lassy++ ${ITK_LIBRARIES} ${VTK_LIBRARIES})
target_link_libraries(surf lassy++ ${ITK_LIBRARIES} ${VTK_LIBRARIES})
target_link_libraries(surfvtk lassy++ ${ITK_LIBRARIES} ${VTK_LIBRARIES})
target_link_libraries(shellshell lassy++ ${ITK_LIBRARIES} ${VTK_LIBRARIES})
target_link_libraries(shellenclosethickness lassy++ ${ITK_LIBRARIES} ${VTK_LIBRARIES})
target_link_libraries(target2source lassy++ ${ITK_LIBRARIES} ${VTK_LIBRARIES})

if (UNIX)
INSTALL(TARGETS lassy DESTINATION /usr/local/bin)
INSTALL(TARGETS twall DESTINATION /usr/local/bin)
INSTALL(TARGETS surf DESTINATION /usr/local/bin)
INSTALL(TARGETS surfvtk DESTINATION /usr/local/bin)
INSTALL(TARGETS shellshell DESTINATION /usr/local/bin)
INSTALL(TARGETS shellenclosethickness DESTINATION /usr/local/bin)
INSTALL(TARGETS target2source DESTINATION /usr/local/bin)
endif (UNIX)
