#
# 
#

#ADD_DEFINITIONS(-DTIXML_USE_STL)

LIST(APPEND	libtinyxml_SRC
  tinystr.cpp  tinyxml.cpp  tinyxmlerror.cpp  tinyxmlparser.cpp
  )

LIST(APPEND libtinyxml_HEADERS
  tinystr.h tinyxml.h
  )

INCLUDE_DIRECTORIES(.)
ADD_LIBRARY(tinyxml ${DS_LIBRARY_TYPE} ${libtinyxml_SRC})

