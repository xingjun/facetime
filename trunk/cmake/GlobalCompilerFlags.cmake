#
# VA_WEBKIT global compiler flags
#
#SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -W -Wcast-align -Wchar-subscripts -Wreturn-type -Wformat -Wformat-security -Wno-format-y2k -Wundef -Wmissing-format-attribute -Wpointer-arith -Wwrite-strings -Wno-unused-parameter -Wno-parentheses")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -W -Wcast-align -Wchar-subscripts -Wreturn-type -Wformat -Wformat-security -Wno-format-y2k -Wundef -Wmissing-format-attribute -Wpointer-arith -Wwrite-strings -Wno-unused-parameter -Wno-parentheses -fvisibility-inlines-hidden")
#SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -W -Wcast-align -Wchar-subscripts -Wreturn-type -Wformat -Wformat-security -Wno-format-y2k -Wundef -Wmissing-format-attribute -Wpointer-arith -Wwrite-strings -Wno-unused-parameter -Wno-parentheses -fPIC")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -W -Wcast-align -Wchar-subscripts -Wreturn-type -Wformat -Wformat-security -Wno-format-y2k -Wundef -Wmissing-format-attribute -Wpointer-arith -Wwrite-strings -Wno-unused-parameter -Wno-parentheses -fvisibility-inlines-hidden -fPIC")

SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")
ADD_DEFINITIONS(-D_GNU_SOURCE=1 -D_REENTRANT)

IF(DS_ENABLE_GPROF)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
ENDIF(DS_ENABLE_GPROF)    

if(DS_ENABLE_DEBUG)
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    #ADD_DEFINITIONS(-DDEBUG)
else(DS_ENABLE_DEBUG)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os")
    #ADD_DEFINITIONS(-DNDEBUG)
endif(DS_ENABLE_DEBUG)

