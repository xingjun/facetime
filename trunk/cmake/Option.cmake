#
# VAOption.cmake all global options live here
#

SET(DS_TARGET "X86" CACHE STRING "Choose the target board, options are: SMP863X SMP865X X86")

SET(DS_PROJECT "HongFei" CACHE STRING "Choose the project, options are: HongFei")

OPTION(DS_ENABLE_DEBUG "Enable debug support")

OPTION(DS_ENABLE_CCACHE "Enable ccache to speed up compile" ON)

OPTION(DS_ENABLE_GPROF "Enable gprof support")

IF(DS_ENABLE_GPROF)
    OPTION(DS_ENABLE_STATIC "Build static library" ON)
ELSE(DS_ENABLE_GPROF)
    OPTION(DS_ENABLE_STATIC "Build static library" OFF)
ENDIF(DS_ENABLE_GPROF)

IF(DS_ENABLE_STATIC)
    SET(DS_LIBRARY_TYPE STATIC)
ELSE(DS_ENABLE_STATIC)
    SET(DS_LIBRARY_TYPE SHARED)
ENDIF(DS_ENABLE_STATIC)


