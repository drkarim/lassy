SET(LASSY_INCLUDES
	"../include/LaShell.h"
	"../include/LaImage.h"
	"../include/MathBox.h"
	"../include/VizBox.h"
	"../include/ShellEntropy.h"
	"../include/LaShellAlgorithms.h"
	"../include/LaImageAlgorithms.h"
	"../include/LaShellShellIntersection.h"
	"../include/LaImageShellAlgorithms.h"
	"../include/LaImageShellAlgorithms.h"
	"../include/LaImageSurfaceNormalAnalysis.h"
	"../include/LaImageNormalInterrogator.h"
	"../include/LaShellShellArithmetic.h"
	"../include/LaShellEnclosureDistance.h"
	"../include/LaShellShellDisplacement.h"
	"../include/LaShellExtractArray.h"
	"../include/LaMaskBoolOperations.h"
	"../include/LaShellAtlas.h"
	"../include/LaShellStatistics.h"
)

SET(LASSY_SRCS 
	LaShell.cxx
	LaImage.cxx
	MathBox.cxx
	VizBox.cxx
	ShellEntropy.cxx
	LaShellShellIntersection.cxx 
	LaShellAlgorithms.cxx 
	LaImageAlgorithms.cxx 
	LaImageShellAlgorithms.cxx 
	LaShellShellArithmetic.cxx
	LaImageSurfaceNormalAnalysis.cxx 
	LaImageNormalInterrogator.cxx
	LaShellEnclosureDistance.cxx
	LaShellShellDisplacement.cxx
	LaShellExtractArray.cxx
	LaMaskBoolOperations.cxx
	LaShellAtlas.cxx
	LaShellStatistics.cxx
) 

ADD_LIBRARY(lassy++ ${LASSY_SRCS} ${LASSY_INCLUDES})
target_link_libraries(lassy++ ${ITK_LIBRARIES} ${VTK_LIBRARIES})

# ONLY FOR MACOSX, REMOVE FOR WINDOWS
if (UNIX)
	install(TARGETS lassy++ DESTINATION /usr/local/lib)
endif (UNIX)
