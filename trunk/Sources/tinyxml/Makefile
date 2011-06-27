#############################################################################
include $(BUILD_SYSTEM)/build-prepare.mk


#****************************************************************************
#
# Makefile for TinyXml test.
# Lee Thomason
# www.grinninglizard.com
#
# This is a GNU make (gmake) makefile
#****************************************************************************

# DEBUG can be set to YES to include debugging info, or NO otherwise
DEBUG          := NO

# PROFILE can be set to YES to include profiling info, or NO otherwise
PROFILE        := NO

# TINYXML_USE_STL can be used to turn on STL support. NO, then STL
# will not be used. YES will include the STL files.
TINYXML_USE_STL := YES

#****************************************************************************

DEBUG_CFLAGS     := -Wall -Wno-format -g -DDEBUG
RELEASE_CFLAGS   := -Wall -Wno-unknown-pragmas -Wno-format -O3

DEBUG_CXXFLAGS   := ${DEBUG_CFLAGS} 
RELEASE_CXXFLAGS := ${RELEASE_CFLAGS}

DEBUG_LDFLAGS    := -g
RELEASE_LDFLAGS  :=

ifeq (YES, ${DEBUG})
   TIXML_CFLAGS       := ${DEBUG_CFLAGS}
   TIXML_CXXFLAGS     := ${DEBUG_CXXFLAGS}
   TIXML_LDFLAGS      := ${DEBUG_LDFLAGS}
else
   TIXML_CFLAGS       := ${RELEASE_CFLAGS}
   TIXML_CXXFLAGS     := ${RELEASE_CXXFLAGS}
   TIXML_LDFLAGS      := ${RELEASE_LDFLAGS}
endif

ifeq (YES, ${PROFILE})
   TIXML_CFLAGS   := ${TIXML_CFLAGS} -pg -O3
   TIXML_CXXFLAGS := ${TIXML_CXXFLAGS} -pg -O3
   TIXML_LDFLAGS  := ${TIXML_LDFLAGS} -pg
endif

#****************************************************************************
# Preprocessor directives
#****************************************************************************

ifeq (YES, ${TINYXML_USE_STL})
  DEFS := -DTIXML_USE_STL
else
  DEFS :=
endif

#****************************************************************************
# Build the libtinyxml
#****************************************************************************

include $(CLEAR_VARS)

LOCAL_CFLAGS   := ${TIXML_CFLAGS}   ${DEFS}
LOCAL_CXXFLAGS := ${TIXML_CXXFLAGS} ${DEFS}
LOCAL_PATH      := $(shell pwd)
LOCAL_MODULE    := tinyxml
LOCAL_SRC_FILES := tinystr.cpp  tinyxml.cpp  tinyxmlerror.cpp  tinyxmlparser.cpp
LOCAL_HEADER_FILES := tinystr.h tinyxml.h

include $(BUILD_SETUP_MODULE)
include $(BUILD_SHARED_LIBRARY)

#############################################################################
include $(BUILD_SYSTEM)/build-finish.mk
