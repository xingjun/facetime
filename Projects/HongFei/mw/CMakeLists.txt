ADD_DEFINITIONS(-D__STDC_CONSTANT_MACROS=1)
DS_USE(FFMPEG)
INCLUDE_DIRECTORIES(
    /usr/include
    ./
    )

LINK_DIRECTORIES(
    /usr/lib
)

ADD_LIBRARY(windplayer ${DS_LIBRARY_TYPE} 
    packetList.cpp
    decoder.cpp
    decoder_impl.cpp
    masterclock.cpp
    output.cpp
    videoout.cpp
    audioout.cpp
    windplayer.cpp
    )
TARGET_LINK_LIBRARIES(windplayer
	utils
    pthread
    z
    )

ADD_SUBDIRECTORY(test)
ADD_SUBDIRECTORY(sdl)
