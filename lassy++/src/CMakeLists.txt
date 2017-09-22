SET(LASSY_INCLUDES
	"../include/lashell.h"
)

SET(LASSY_SRCS 
	lashell.cxx
) 

ADD_LIBRARY(lassy++ ${LASSY_SRCS} ${LASSY_INCLUDES})
target_link_libraries(lassy++ ${ITK_LIBRARIES} ${VTK_LIBRARIES})

