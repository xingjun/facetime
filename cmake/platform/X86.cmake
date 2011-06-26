######################################################################################
##
## Setup Platform variables for x86
##

MACRO(DS_SET_PLATFORM_X86)
	SET(DS_PLATFORM_X86 TRUE)
	SET(BUILD_SHARED_LIBS TRUE)

	SET(DS_DFB_PATH "/usr")
	SET(DS_FREETYPE_PATH ${DS_DFB_PATH})
	#SET(DS_DFB_PATH "/usr" CACHE PATH "Choose the path of DirectFB")
	#SET(DS_FREETYPE_PATH ${DS_DFB_PATH} CACHE PATH "Choose the path of FreeType2")

    ## libcurl
    SET(DS_CURL_INCLUDE_DIR /usr/include)
    SET(DS_CURL_LIBRARY_DIR /usr/lib)
    SET(DS_CURL_LIBS curl rt)
    ## sigc++
    SET(DS_SIGC++_INCLUDE_DIR /usr/include/sigc++-2.0)
    SET(DS_SIGC++_LIBRARY_DIR /usr/lib)
    SET(DS_SIGC++_LIBS sigc-2.0)
    ## libxml2
    SET(DS_LIBXML2_INCLUDE_DIR /usr/include/libxml2)
    SET(DS_LIBXML2_LIBRARY_DIR /usr/lib)
    SET(DS_LIBXML2_LIBS xml2)
    ## ffmpeg (if you do not want to use system ffmepg, you should comment bellow 3 lines)
    #SET(DS_FFMPEG_INCLUDE_DIR /usr/include)
    #SET(DS_FFMPEG_LIBRARY_DIR /usr/lib)
    #SET(DS_FFMPEG_LIBS swscale avformat avfilter avdevice avcodec avutil asound z dl m pthread)

    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNO_CPP_DEMANGLE -rdynamic -ldl")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNO_CPP_DEMANGLE -rdynamic -ldl")

ENDMACRO(DS_SET_PLATFORM_X86)

