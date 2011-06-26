#
# Build a GNU package with autotools
#  confiugre && make 
#

SET(GNUPKG_INSTALL_PATH ${DS_BINARY_DIR}/gnupkg/install)

## CC CXX CFLAGS CXXFLAGS
SET(GNUPKG_CC ${CMAKE_C_COMPILER})
SET(GNUPKG_CXX ${CMAKE_CXX_COMPILER})
IF(DS_ENABLE_CCACHE)
    SET(GNUPKG_CC ${CMAKE_C_COMPILER_ARG1})
    SET(GNUPKG_CXX ${CMAKE_CXX_COMPILER_ARG1})
ENDIF(DS_ENABLE_CCACHE)
SET(GNUPKG_CFLAGS "${CMAKE_C_FLAGS}")
SET(GNUPKG_CXXFLAGS "${CMAKE_CXX_FLAGS}")

## init GNUPKG_CONFIG_TARGETS , GNUPKG_BUILD_TARGETS 
SET(GNUPKG_CONFIG_TARGETS "")
SET(GNUPKG_BUILD_TARGETS "")

## macro CONFIG_GNU_PACKAGE
MACRO(CONFIG_GNU_PACKAGE _pkg_name _pkg_rpath _pkg_options)
    SET(GNUPKG_SOURCE_PATH ${DS_SOURCE_DIR}/${_pkg_rpath})
    SET(GNUPKG_BINARY_PATH ${DS_BINARY_DIR}/gnupkg/${_pkg_rpath})
    SET(GNUPKG_CONFIG_TARGETS ${GNUPKG_CONFIG_TARGETS} ${_pkg_name})

    ADD_CUSTOM_TARGET(
        ${_pkg_name}
        COMMENT "config ing ${_pkg_name} in ${_pkg_rpath}"
        COMMAND mkdir -p ${GNUPKG_BINARY_PATH}
        COMMAND mkdir -p ${GNUPKG_INSTALL_PATH}
        )
    ADD_CUSTOM_COMMAND(
        TARGET ${_pkg_name}
        COMMENT "options: ${_pkg_options}"
        COMMAND CC=${GNUPKG_CC} CXX=${GNUPKG_CXX} CFLAGS="${GNUPKG_CFLAGS}" CXXFLAGS="${GNUPKG_CXXFLAGS}" ${GNUPKG_SOURCE_PATH}/configure --prefix=${GNUPKG_INSTALL_PATH} ${_pkg_options}
        WORKING_DIRECTORY ${GNUPKG_BINARY_PATH}
#	    NO_ESCAPE_QUOTE
        )
ENDMACRO(CONFIG_GNU_PACKAGE)

## macro BUILD_GNU_PACKAGE
MACRO(BUILD_GNU_PACKAGE _pkg_name _pkg_rpath)
    SET(GNUPKG_BINARY_PATH ${DS_BINARY_DIR}/gnupkg/${_pkg_rpath})
    SET(GNUPKG_BUILD_TARGETS ${GNUPKG_BUILD_TARGETS} ${_pkg_name})
    ADD_CUSTOM_TARGET(
        ${_pkg_name}
        COMMENT "building ${_pkg_name} in ${_pkg_rpath}"
        )
    ADD_CUSTOM_COMMAND(
        TARGET ${_pkg_name}
        COMMAND make
        COMMAND make install
        WORKING_DIRECTORY ${GNUPKG_BINARY_PATH}
        )
ENDMACRO(BUILD_GNU_PACKAGE)

## begin add libraries to configure.
CONFIG_GNU_PACKAGE(libffmpeg_cfg 3rdParties/ffmpeg-0.8 "")

ADD_CUSTOM_TARGET(ds_gnupkg_config
    COMMENT "config done all gnu packages!"
    )
ADD_DEPENDENCIES(ds_gnupkg_config
    ${GNUPKG_CONFIG_TARGETS}
    )

## begin add libraries to build.
BUILD_GNU_PACKAGE(libffmpeg_build 3rdParties/ffmpeg-0.8)

ADD_CUSTOM_TARGET(ds_gnupkg_build
    COMMENT "built done all gnu packages"
    )
ADD_DEPENDENCIES(ds_gnupkg_build
    ${GNUPKG_BUILD_TARGETS}
    )

## set parameters for DS_USE(xx) to use. (if you want to use system ffmpeg, you should comment bellow 3 lines)
SET(DS_FFMPEG_INCLUDE_DIR ${GNUPKG_INSTALL_PATH}/include)
SET(DS_FFMPEG_LIBRARY_DIR ${GNUPKG_INSTALL_PATH}/lib)
SET(DS_FFMPEG_LIBS swscale avformat avfilter avdevice avcodec avutil asound SDL z dl m pthread)
