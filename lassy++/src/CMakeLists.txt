SET(LASSY_INCLUDES
	"../include/LaShell.h"
	"../include/LaImage.h"
)

SET(LASSY_SRCS 
	LaShell.cxx
	LaImage.cxx
) 

ADD_LIBRARY(lassy++ ${LASSY_SRCS} ${LASSY_INCLUDES})
target_link_libraries(lassy++ ${ITK_LIBRARIES} ${VTK_LIBRARIES})

