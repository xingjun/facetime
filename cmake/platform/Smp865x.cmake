######################################################################################
##
## Setup Platform variables for SMP865X/DCCHD
##

MACRO(DS_USE_TOOLCHAIN _toolchain_path _toolchain_prefix)
    # this one is important
    SET(CMAKE_SYSTEM_NAME Linux)
    #this one not so much
    SET(CMAKE_SYSTEM_VERSION 1)

    SET(CMAKE_SYSTEM_PROCESSOR MIPS)

    SET(CMAKE_CROSSCOMPILING TRUE)

    SET(VA_TOOLCHAIN_PATH ${_toolchain_path})

    # specify the cross compiler
    SET(CMAKE_C_COMPILER   ${_toolchain_path}/bin/${_toolchain_prefix}gcc)
    SET(CMAKE_CXX_COMPILER ${_toolchain_path}/bin/${_toolchain_prefix}g++)
    #SET(CMAKE_ASM-ATT_COMPILER ${VA_TOOLCHAIN_PATH}/bin/mips2_fp_be-gcc)

    # where is the target environment 
    #SET(CMAKE_FIND_ROOT_PATH  ${VA_TOOLCHAIN_PATH}/target)

    # search for programs in the build host directories
    #SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    # for libraries and headers in the target directories
    #SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    #SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
ENDMACRO(DS_USE_TOOLCHAIN)

MACRO(DS_SET_PLATFORM_SMP865X)

    SET(DS_PLATFORM_SMP865X TRUE)
    SET(CMAKE_SYSTEM_PROCESSOR MIPS)

    SET(BUILD_SHARED_LIBS TRUE)
    SET(DS_TARGET_ARCH  "mips")
    SET(DS_TARGET_NAME  "mips-linux-gnu")
    SET(DS_SDK_SMP865X_C_FLAGS "-EL -Wa,-mips32r2 -march=24kf -mtune=24kf -mhard-float -mglibc -mocteon-useun -falign-arrays -mfix-r4000 -fPIC")
    SET(DS_SDK_SMP865X_C_FLAGS "-rdynamic -ldl ${DS_SDK_SMP865X_C_FLAGS}")
    ADD_DEFINITIONS(-DLITTLE_ENDIAN=1 -DBYTE_ORDER=LITTLE_ENDIAN -DCROSS_COMPILING=1 -D__mips__)

    SET(DS_SDK_PLATFORM_SMP865X_BASE "/opt/smp865x_sdk" CACHE STRING "set to your smp865x sdk base folder, which contains mrua, dcchd, flashlite, cs_rootfs, smp86xx_kernel_source")
    SET(DS_SDK_SMP865X_VERSION "3.8.0" CACHE STRING "version of the sdk, options: 3.8.0, 3.9.0, 3.9.2")
    IF(DS_SDK_SMP865X_VERSION STREQUAL "3.8.0")
        SET(DS_SDK_TOOLCHAIN_PATH "${DS_SDK_PLATFORM_SMP865X_BASE}/mips-4.3")
        SET(DS_SDK_TARGET_KERNEL_PATH "${DS_SDK_PLATFORM_SMP865X_BASE}/smp86xx_kernel_source_R2.6.22-27/linux-2.6.22.19")
        SET(DS_SDK_MRUA_DIR "${DS_SDK_PLATFORM_SMP865X_BASE}/mrua_SMP8654F_3_8_0_dev.mips/MRUA_src")
        SET(DS_SDK_DCCHD_DIR "${DS_SDK_PLATFORM_SMP865X_BASE}/dcchd_SMP8652_3_8_0_red.mips/dcchd")
        SET(DS_SDK_DFB_DIR "${DS_SDK_PLATFORM_SMP865X_BASE}/dcchd_SMP8652_3_8_0_red.mips/directfb")
        SET(DS_SDK_FLASHLITE_DIR "${DS_SDK_PLATFORM_SMP865X_BASE}/flashlite3_SMP8654_3_8_0-1.mips")
        SET(DS_SDK_ROOTFS_DIR "${DS_SDK_PLATFORM_SMP865X_BASE}/cs_rootfs_1.2.6")
    ENDIF(DS_SDK_SMP865X_VERSION STREQUAL "3.8.0")

    IF(DS_SDK_SMP865X_VERSION STREQUAL "3.9.0")
        SET(DS_SDK_TOOLCHAIN_PATH "${DS_SDK_PLATFORM_SMP865X_BASE}/mips-4.3")
        SET(DS_SDK_TARGET_KERNEL_PATH "${DS_SDK_PLATFORM_SMP865X_BASE}/smp86xx_kernel_source_R2.6.22-27/linux-2.6.22.19")
        SET(DS_SDK_MRUA_DIR "${DS_SDK_PLATFORM_SMP865X_BASE}/mrua_SMP8654F_3_9_0_dev.mips/MRUA_src")
        SET(DS_SDK_DCCHD_DIR "${DS_SDK_PLATFORM_SMP865X_BASE}/dcchd_SMP865x_3_9_0_black.mips/dcchd")
        SET(DS_SDK_DFB_DIR "${DS_SDK_PLATFORM_SMP865X_BASE}/dcchd_SMP865x_3_9_0_black.mips/directfb")
        SET(DS_SDK_FLASHLITE_DIR "${DS_SDK_PLATFORM_SMP865X_BASE}/flashlite3_SMP8654_3_8_0-1.mips")
        SET(DS_SDK_ROOTFS_DIR "${DS_SDK_PLATFORM_SMP865X_BASE}/cs_rootfs_1.2.10")
    ENDIF(DS_SDK_SMP865X_VERSION STREQUAL "3.9.0")

    IF(DS_SDK_SMP865X_VERSION STREQUAL "3.9.2")
        SET(DS_SDK_TOOLCHAIN_PATH "${DS_SDK_PLATFORM_SMP865X_BASE}/mips-4.4")
        SET(DS_SDK_TARGET_KERNEL_PATH "${DS_SDK_PLATFORM_SMP865X_BASE}/smp86xx_kernel_source_R2.6.22-35/linux-2.6.22.19")
        SET(DS_SDK_MRUA_DIR "${DS_SDK_PLATFORM_SMP865X_BASE}/mrua_SMP8654F_3_9_2_dev.mips/MRUA_src")
        SET(DS_SDK_DCCHD_DIR "${DS_SDK_PLATFORM_SMP865X_BASE}/dcchd_SMP865x_3_9_2_black.mips/dcchd")
        SET(DS_SDK_DFB_DIR "${DS_SDK_PLATFORM_SMP865X_BASE}/dcchd_SMP865x_3_9_2_black.mips/directfb")
        SET(DS_SDK_FLASHLITE_DIR "${DS_SDK_PLATFORM_SMP865X_BASE}/flashlite3_SMP8654_3_8_0-1.mips")
        SET(DS_SDK_ROOTFS_DIR "${DS_SDK_PLATFORM_SMP865X_BASE}/cs_rootfs_1.2.12")
    ENDIF(DS_SDK_SMP865X_VERSION STREQUAL "3.9.2")

	## set toolchain
    DS_USE_TOOLCHAIN(${DS_SDK_TOOLCHAIN_PATH} mips-linux-gnu-)

    ## rootfs
    SET(DS_SDK_ROOTFS_INCLUDE_DIR ${DS_SDK_ROOTFS_DIR}/cross_rootfs/include)
    SET(DS_SDK_ROOTFS_LIBRARY_DIR ${DS_SDK_ROOTFS_DIR}/cross_rootfs/lib)
    SET(DS_SDK_ROOTFS_LIBS 
	    png
	    jpeg
	    ungif
        )
    ## libcurl
    SET(DS_CURL_INCLUDE_DIR ${DS_SDK_ROOTFS_DIR}/cross_rootfs/include)
    SET(DS_CURL_LIBRARY_DIR ${DS_SDK_ROOTFS_DIR}/cross_rootfs/lib)
    SET(DS_CURL_LIBS curl rt)
    ## sigc++
    SET(DS_SIGC++_INCLUDE_DIR ${DS_SDK_ROOTFS_DIR}/cross_rootfs/include/sigc++-2.0 ${DS_SDK_ROOTFS_DIR}/cross_rootfs/include)
    SET(DS_SIGC++_LIBRARY_DIR ${DS_SDK_ROOTFS_DIR}/cross_rootfs/lib)
    SET(DS_SIGC++_LIBS sigc-2.0)
    ## libxml2
    SET(DS_LIBXML2_INCLUDE_DIR ${DS_SDK_ROOTFS_DIR}/cross_rootfs/include/sigc++-2.0 ${DS_SDK_ROOTFS_DIR}/cross_rootfs/include/libxml2)
    SET(DS_LIBXML2_LIBRARY_DIR ${DS_SDK_ROOTFS_DIR}/cross_rootfs/lib)
    SET(DS_LIBXML2_LIBS xml2)

    ## directfb
    SET(DS_SDK_DFB_INCLUDE_DIR
        ${DS_SDK_DFB_DIR}/include/directfb
        ${DS_SDK_DFB_DIR}/include/directfb-internal
        )
    SET(DS_SDK_DFB_LIBRARY_DIR ${DS_SDK_DFB_DIR}/lib)
    SET(DS_SDK_DFB_LIBS 
	    fusion
	    direct
	    directfb
        )
    ADD_DEFINITIONS(-D_REENTRANT)

    ## mrua
    SET(DS_SDK_MRUA_INCLUDE_DIR ${DS_SDK_MRUA_DIR})
    SET(DS_SDK_MRUA_LIBRARY_DIR ${DS_SDK_MRUA_DIR}/lib)
    SET(DS_SDK_MRUA_LIBS 
        rmcw 
        llad 
        rua 
        dcc 
        rmcpputils 
        rmcore 
        rmvdemux 
        rmmpegsystemindex 
        rmmpegsystemindexcreator 
        rmwmaprodecodercore 
        rmwmaprodecoder 
        rmrtk86 
        rmscc 
        rmdrm 
        rmwmdrmndstub 
        rmwmdrmstub 
        rmhttp 
        rmavicore 
        rmriff 
        rmzlib 
        rmmpeg4framework 
        rmmp4core 
        rmdescriptordecoder 
        rmmp4 
        rmmp4api 
        rmasfdemuxcore 
        rmasfdemux 
        rmdetector3 
        rmdetector3api 
        rmsubidx 
        rmtextsubs 
        rmdvbsubs 
        rmfontrender 
        rmsofttxt 
        rmstreamingprotocols 
        rmmkvdemux 
        rmdtcpapi 
        rmlibplay 
        rmmm
        rmfp 
        rmpthreadw
        rmdetector
        rmdetectorapi
        ndsp
        rmcdfs_t
        rmdisplay
#        rmoutput
#        ruaoutput
#        ruaoutputdebugger
        rmvideoout
        audiooutports
        displayoutports
        rmi2c
        rmedid
        ruai2c
        rmcec
        rmsha1
        rmhdmi
        ruahdmi
        rmhsi
        ruahsi
        #tangogfx
        samples_t
        )

    IF(DS_SDK_SMP865X_VERSION STREQUAL "3.9.0" OR DS_SDK_SMP865X_VERSION STREQUAL "3.9.2")
    SET(DS_SDK_MRUA_LIBS ${DS_SDK_MRUA_LIBS}
        rmoutput
        ruaoutput
        ruaoutputdebugger
		)
    ENDIF(DS_SDK_SMP865X_VERSION STREQUAL "3.9.0" OR DS_SDK_SMP865X_VERSION STREQUAL "3.9.2")

    SET(DS_SDK_MRUA_CFLAGS
        -O2  
        -DEM86XX_CHIP=EM86XX_CHIPID_TANGO3 
        -DEM86XX_REVISION=3 
        -DXBOOT2_SMP865X=1 
        -DEM86XX_MODE=EM86XX_MODEID_STANDALONE 
        -DWITH_XLOADED_UCODE=1 
        -DNO_DTSXLL_DECODE 
        -DDTSHD_CAPABLE_IF_ANY 
        -DDECODER_DOWNMIX 
        -DNO_REENCODE 
        -DNO_DTSHD_8CH_ABOVE=48000 
        -DNO_TrueHD_192K_ABOVE=6 
        -DNO_TrueHD_96K_ABOVE=8 
        -DNO_MC_DOWNSAMPLE_ABOVE=48000 
        -DNO_MIX_ABOVE=48000 
        -DNO_BM_ABOVE=48000 
        -DDMIX_IF_NO_BM 
        -DSTC_OFFSET_MS=200 
        -DPRIORITY_BOOST=-5 
        -DALLOC_AND_RELEASE_IN_PLAY 
        -DAUDIO_RECCCS_HACK 
        -DPCMX_HACK_FILL_INCOMPLETE_CHUNKS 
        -DCRT_SCALER=DispCRTMultiScaler 
        -DGFX_SCALER=DispGFXMultiScaler 
        -DHDMI_CHECK_PRIORITY_BOOST=-15 
        -DALLOW_NON_JFIF 
        -DHACK_NO_DISPLAY_STOP_CONFIRMATION 
        -DHACK_ENABLE_STEREO_SURROUND_SUPPORT 
        -DHACK_NO_CSS_IF_CPM_IS_ZERO 
        -DNO_BLUE_SUPPORT 
        -DNO_AACSONLINE 
        -DUSE_LIBSAMPLES_V2 
        -DWITH_RMHDMI=1 
        -DNO_CRT_SCALER 
        -DMMAP_DRAM1_SIZE=0 
        -DMMAP_DRAM0_SIZE=98 
        -DMMAP_CPS_SIZE=0 
        -fPIC 
        -D_FILE_OFFSET_BITS=64 
        -DALLOW_OS_CODE 
        -D_REENTRANT 
        -DWITH_THREADS=1 
        -DRMPLATFORM=RMPLATFORMID_TANGO3 
        )
    ADD_DEFINITIONS(${DS_SDK_MRUA_CFLAGS})

    ## dcchd
    SET(DS_SDK_DCCHD_INCLUDE_DIR
        ${DS_SDK_DCCHD_DIR}/dcchd
        ${DS_SDK_DCCHD_DIR}/core 
        ${DS_SDK_DCCHD_DIR}/brd 
        ${DS_SDK_DCCHD_DIR}/mono 
        ${DS_SDK_DCCHD_DIR}/dvdvr 
        ${DS_SDK_DCCHD_DIR}/curacao 
        ${DS_SDK_DCCHD_DIR}/dtv
        ${DS_SDK_DCCHD_DIR}/config/single_bank
        ${DS_SDK_DCCHD_DIR}/config/default
        )
    # SET(DS_SDK_DCCHD_LIBRARY_DIR
    #     ${DS_SDK_DCCHD_DIR}/dcchd
    #     ${DS_SDK_DCCHD_DIR}/core 
    #     ${DS_SDK_DCCHD_DIR}/brd 
    #     ${DS_SDK_DCCHD_DIR}/mono 
    #     ${DS_SDK_DCCHD_DIR}/dvdvr 
    #     ${DS_SDK_DCCHD_DIR}/curacao 
    #     ${DS_SDK_DCCHD_DIR}/curacao/lib 
    #     ${DS_SDK_DCCHD_DIR}/dtv
    #     ${DS_SDK_DCCHD_DIR}/dtv/network
    #     ${DS_SDK_DCCHD_DIR}/dtv/capture
    #     ${DS_SDK_DCCHD_DIR}/config/single_bank
    #     ${DS_SDK_DCCHD_DIR}/config/default
    # )
    #SET(DS_SDK_DCCHD_LIBS 
    #	dcchd
    #	dcchd_core
    #	dcchd_dvd
    #	dcchd_mcast
    #	dcchd_cap
    #	dcchd_dtv
    #	dcchd_lpb
    #	dcchd_dvdvr
    #	rmdvdvideoapi
    #	rmdvdvideonav
    #	rmcddanavapi
    #	rmcdda
    #	rmdisc
    #	rmpacketcommand
    #	rmdvdfsapi
    #	rmdvdfs
    #)

    #LIST(APPEND DS_SDK_DCCHD_LDFLAGS
    #  -Wl,-rpath-link -Wl,${DS_SDK_DCCHD_DIR}/core
    #  -Wl,-rpath-link -Wl,${DS_SDK_DCCHD_DIR}/mono
    #  -Wl,-rpath-link -Wl,${DS_SDK_DCCHD_DIR}/dvdvr
    #  -Wl,-rpath-link -Wl,${DS_SDK_DCCHD_DIR}/brd
    #  -Wl,-rpath-link -Wl,${DS_SDK_DCCHD_DIR}/curacao
    #  -Wl,-rpath-link -Wl,${DS_SDK_DCCHD_DIR}/curacao/lib
    #  -Wl,-rpath-link -Wl,${DS_SDK_DCCHD_DIR}/dtv
    #  -Wl,-rpath-link -Wl,${DS_SDK_DCCHD_DIR}/dtv/acap
    #  -Wl,-rpath-link -Wl,${DS_SDK_DCCHD_DIR}/dtv/tuner
    #  -Wl,-rpath-link -Wl,${DS_SDK_DCCHD_DIR}/dtv/capture
    #  -Wl,-rpath-link -Wl,${DS_SDK_DCCHD_DIR}/dtv/network
    #  -Wl,-rpath-link -Wl,${DS_SDK_DCCHD_DIR}/dcchd
    #)

    # replace DS_DCCHD_LDFLAGS ("-Wl,-rpath-link -Wl,$DIR"), use with SET_TARGET_PROPERTIES ()
    SET(DS_SDK_DCCHD_INSTALL_RPATH
        ${DS_SDK_DCCHD_DIR}/core
        ${DS_SDK_DCCHD_DIR}/mono
        ${DS_SDK_DCCHD_DIR}/dvdvr
        ${DS_SDK_DCCHD_DIR}/brd
        ${DS_SDK_DCCHD_DIR}/curacao
        ${DS_SDK_DCCHD_DIR}/curacao/lib
        ${DS_SDK_DCCHD_DIR}/dtv
        ${DS_SDK_DCCHD_DIR}/dtv/acap
        ${DS_SDK_DCCHD_DIR}/dtv/tuner
        ${DS_SDK_DCCHD_DIR}/dtv/capture
        ${DS_SDK_DCCHD_DIR}/dtv/network
        ${DS_SDK_DCCHD_DIR}/dcchd
        )

    ## flashlite
    SET(DS_SDK_FLASHLITE_INCLUDE_DIR
        ${DS_SDK_FLASHLITE_DIR}/include
        )
    SET(DS_SDK_FLASHLITE_LIBS
        m z
        ${DS_SDK_FLASHLITE_DIR}/lib/libAELoader.a 
        ${DS_SDK_FLASHLITE_DIR}/lib/libAEModule.a
        )

    ## compile flags.
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${DS_SDK_SMP865X_C_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${DS_SDK_SMP865X_C_FLAGS}")

ENDMACRO(DS_SET_PLATFORM_SMP865X)
