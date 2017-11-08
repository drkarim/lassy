SET(LASSY_INCLUDES
	"../include/LaShell.h"
	"../include/LaImage.h"
	"../include/MathBox.h"
	"../include/VizBox.h"
	"../include/ShellEntropy.h"
	"../include/LaShellAlgorithms.h"
	"../include/LaShellWallThickness.h"
)

SET(LASSY_SRCS 
	LaShell.cxx
	LaImage.cxx
	MathBox.cxx
	VizBox.cxx
	ShellEntropy.cxx
	LaShellWallThickness.cxx 
	LaShellAlgorithms.cxx 
) 

ADD_LIBRARY(lassy++ ${LASSY_SRCS} ${LASSY_INCLUDES})
target_link_libraries(lassy++ ${ITK_LIBRARIES} ${VTK_LIBRARIES})

# ONLY FOR MACOSX, REMOVE FOR WINDOWS
if (UNIX)
	install(TARGETS lassy++ DESTINATION /usr/local/lib)
endif (UNIX)
