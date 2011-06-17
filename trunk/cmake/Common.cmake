#
# all common macro function and variable live here
#

## DirectFB 
SET(DS_DFB_INCLUDE_DIR ${DS_DFB_PATH}/include/directfb ${DS_DFB_PATH}/include/directfb-internal)
SET(DS_DFB_LIBRARY_DIR ${DS_DFB_PATH}/lib)
SET(DS_DFB_LIBS direct fusion directfb pthread z)

## Freetype
SET(DS_FREETYPE_INCLUDE_DIR	${DS_FREETYPE_PATH}/include/freetype2)
SET(DS_FREETYPE_LIBRARY_DIR ${DS_FREETYPE_PATH}/lib)
SET(DS_FREETYPE_LIBS freetype)

## SDL
SET(DS_SDL_INCLUDE_DIR ${DS_SOURCE_DIR}/3rdParties/SDL/include)
SET(DS_SDL_LIBS SDL)

## SDL_gfx
SET(DS_SDLGFX_INCLUDE_DIR ${DS_SOURCE_DIR}/3rdParties/SDL_gfx/include)
SET(DS_SDLGFX_LIBS SDL_gfx)

## libpng
SET(DS_PNG_INCLUDE_DIR ${DS_SOURCE_DIR}/3rdParties/libpng)
SET(DS_PNG_LIBS png)

## jpeg
SET(DS_JPEG_INCLUDE_DIR ${DS_SOURCE_DIR}/3rdParties/jpeg)
SET(DS_JPEG_LIBS jpeg)

## gif
SET(DS_GIF_INCLUDE_DIR ${DS_SOURCE_DIR}/3rdParties/giflib)
SET(DS_GIF_LIBS gif)

## zlib
SET(DS_ZLIB_INCLUDE_DIR ${DS_SOURCE_DIR}/3rdParties/zlib)
SET(DS_ZLIB_LIBS z)

## sigc++
#SET(DS_SIGC++_INCLUDE_DIR ${DS_SOURCE_DIR}/3rdParties/libsigc++)
#SET(DS_SIGC++_LIBS sigc++)

## sqlite
SET(DS_SQLITE_INCLUDE_DIR ${DS_SOURCE_DIR}/3rdParties/sqlite)
SET(DS_SQLITE_LIBS sqlite3)

## libcurl
#SET(DS_CURL_INCLUDE_DIR ${DS_SOURCE_DIR}/3rdParties/curl/include)
#SET(DS_CURL_LIBS curl)

## expat
SET(DS_EXPAT_INCLUDE_DIR ${DS_SOURCE_DIR}/3rdParties/expat/lib)
SET(DS_EXPAT_LIBS expat)

## libxml2
SET(DS_LIBXML2_INCLUDE_DIR ${DS_SOURCE_DIR}/3rdParties/libxml2/include)
SET(DS_LIBXML2_LIBS xml2)

## libxslt
SET(DS_LIBXSLT_INCLUDE_DIR ${DS_SOURCE_DIR}/3rdParties/libxslt)
SET(DS_LIBXSLT_LIBS xslt)

## icu
SET(DS_ICU_INCLUDE_DIR ${DS_SOURCE_DIR}/3rdParties/icu4c/i18n ${DS_SOURCE_DIR}/3rdParties/icu4c/common)
SET(DS_ICU_LIBS icudata icuuc icui18n)

## fontconfig
SET(DS_FONTCONFIG_INCLUDE_DIR ${DS_SOURCE_DIR}/3rdParties/fontconfig)
SET(DS_FONTCONFIG_LIBS fontconfig)

## pixman
SET(DS_PIXMAN_INCLUDE_DIR ${DS_SOURCE_DIR}/3rdParties/pixman/pixman)
SET(DS_PIXMAN_LIBS	pixman-1)

## cairo
SET(DS_CAIRO_INCLUDE_DIR ${DS_SOURCE_DIR}/3rdParties/cairo/src)
SET(DS_CAIRO_LIBS cairo)

## cppunit
SET(DS_CPPUNIT_INCLUDE_DIR ${DS_SOURCE_DIR}/3rdParties/cppunit/include)
SET(DS_CPPUNIT_LIBS cppunit)

## disko
SET(DS_DISKO_INCLUDE_DIR ${DS_SOURCE_DIR}/3rdParties/disko/inc)
SET(DS_DISKO_LIBS disko)

## openssl
SET(DS_OPENSSL_INCLUDE_DIR ${DS_SOURCE_DIR}/3rdParties/openssl/include)
SET(DS_OPENSSL_LIBS crypto ssl)

## LiTE
SET(DS_LITE_INCLUDE_DIR ${DS_SOURCE_DIR}/3rdParties/LiTE)
SET(DS_LITE_LIBS LiTE)

## iconv
SET(DS_ICONV_INCLUDE_DIR ${DS_SOURCE_DIR}/3rdParties/libiconv/include)
SET(DS_ICONV_LIBS iconv)

## wxDFB
SET(DS_WXDFB_INCLUDE_DIR ${DS_SOURCE_DIR}/3rdParties/wxDFB/lib/wx/include/dfbuniv-ansi-release-2.8 ${DS_SOURCE_DIR}/3rdParties/wxDFB/include)
SET(DS_WXDFB_LIBS wx_dfbuniv)

## skia
SET(DS_CORECG_INCLUDE_DIR ${DS_SOURCE_DIR}/3rdParties/skia/include/corecg)
SET(DS_CORECG_LIBS corecg)
SET(DS_SGL_INCLUDE_DIR ${DS_SOURCE_DIR}/3rdParties/skia/include/graphics)
SET(DS_SGL_LIBS sgl)

## flex
#FIND_PROGRAM(DS_FLEX_EXECUTABLE flex)
#IF(NOT DS_FLEX_EXECUTABLE)
#	MESSAGE(FATAL_ERROR "flex not found - aborting")
#ENDIF(NOT DS_FLEX_EXECUTABLE)
#
#MACRO(DS_ADD_FLEX_FILES _sources)
#    FOREACH (_current_FILE ${ARGN})
#      GET_FILENAME_COMPONENT(_in ${_current_FILE} ABSOLUTE)
#      GET_FILENAME_COMPONENT(_basename ${_current_FILE} NAME_WE)
#
#      SET(_out ${CMAKE_CURRENT_BINARY_DIR}/flex_${_basename}.cpp)
#
#      ADD_CUSTOM_COMMAND(
#         OUTPUT ${_out}
#         COMMAND ${DS_FLEX_EXECUTABLE}
#         ARGS
#         -o${_out}
#         ${_in}
#         DEPENDS ${_in}
#      )
#
#      SET(${_sources} ${${_sources}} ${_out} )
#   ENDFOREACH (_current_FILE)
#ENDMACRO(DS_ADD_FLEX_FILES)

## bison
#FIND_PROGRAM(DS_BISON_EXECUTABLE bison)
#IF (NOT DS_BISON_EXECUTABLE)
#	MESSAGE(FATAL_ERROR "bison not found - aborting")
#ENDIF (NOT DS_BISON_EXECUTABLE)
#
#MACRO(DS_ADD_BISON_FILES _sources)
#    FOREACH (_current_FILE ${ARGN})
#      GET_FILENAME_COMPONENT(_in ${_current_FILE} ABSOLUTE)
#      GET_FILENAME_COMPONENT(_basename ${_current_FILE} NAME_WE)
#
#      SET(_out ${CMAKE_CURRENT_BINARY_DIR}/bison_${_basename}.cpp)
#
#      ADD_CUSTOM_COMMAND(
#         OUTPUT ${_out}
#         COMMAND ${DS_BISON_EXECUTABLE}
#         ARGS
#         -o${_out}
#         ${_in}
#         DEPENDS ${_in}
#      )
#
#      SET(${_sources} ${${_sources}} ${_out} )
#   ENDFOREACH (_current_FILE)
#ENDMACRO(DS_ADD_BISON_FILES)

# gperf
#FIND_PROGRAM(DS_GPERF_EXECUTABLE gperf)
#IF(NOT DS_GPERF_EXECUTABLE)
#    MESSAGE(FATAL_ERROR " gperf not found -- aborting")
#ENDIF(NOT DS_GPERF_EXECUTABLE)

# perl
#FIND_PROGRAM(DS_PERL_EXECUTABLE perl)
#IF(NOT DS_PERL_EXECUTABLE)
#    MESSAGE(FATAL_ERROR " perl not found -- aborting")
#ENDIF(NOT DS_PERL_EXECUTABLE)
#
#MACRO(DS_ADD_LIBRARY _target _sources)
#    IF(DS_ENABLE_STATIC)
#        ADD_LIBRARY(${_target} ${_sources})
#    ELSE(DS_ENABLE_STATIC)
#        ADD_LIBRARY(${_target} SHARED ${_sources})
#    ENDIF(DS_ENABLE_STATIC)
#ENDMACRO(DS_ADD_LIBRARY)

## tinyxml
SET(DS_TINYXML_INCLUDE_DIR ${DS_SOURCE_DIR}/Sources/tinyxml)
SET(DS_TINYXML_LIBS tinyxml)

## bluez
SET(DS_BLUEZ_INCLUDE_DIR ${DS_SOURCE_DIR}/3rdParties/bluez)
SET(DS_BLUEZ_LIBS bluez)

SET(DS_INSTALL_RPATH "")
MACRO(DS_USE)
	FOREACH(_current ${ARGN})
		IF (NOT DS_${_current}_INCLUDE_DIR)
			MESSAGE(FATAL_ERROR " library ${_current} is not found")
		ENDIF (NOT DS_${_current}_INCLUDE_DIR)

		INCLUDE_DIRECTORIES (${DS_${_current}_INCLUDE_DIR})
		LINK_DIRECTORIES (${DS_${_current}_LIBRARY_DIR})
		LINK_LIBRARIES (${DS_${_current}_LIBS})
		#LINK_LIBRARIES (${DS_${_current}_LDFLAGS} ${DS_${_current}_LIBS})
        SET(DS_INSTALL_RPATH ${DS_INSTALL_RPATH};${DS_${_current}_INSTALL_RPATH})
        #SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${DS_${_current}_LDFLAGS}")
	ENDFOREACH(_current)
ENDMACRO(DS_USE)


