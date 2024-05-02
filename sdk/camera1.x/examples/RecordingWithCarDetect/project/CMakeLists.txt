################################################################################################################
#                                                                                                              #
# Copyright      2024 MiTAC International Corp.                                                                #
#                                                                                                              #
#--------------------------------------------------------------------------------------------------------------#
# PROJECT     :  Camera SDK Example Support                                                                    #
# BINARY NAME :  RecordingWithCarDetect                                                                        #
# FILE NAME   :  CMakeLists.txt                                                                                #
# CREATED BY  :  Forest.Yeh <forest.yeh@mic.com.tw>                                                            #
# CREATED DATE:  03/05/24 (MM/DD/YY)                                                                           #
################################################################################################################

cmake_minimum_required(VERSION 3.4.1)

project(RecordingWithCarDetect)

set(3RDPARTY_ROOT ../../../../../3rdParty)
set(BSP_ROOT ../../../../../bsp)
set(SDK_ROOT ../../..)

set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_LINKER aarch64-linux-gnu-gcc)

include_directories(${SDK_ROOT}/include/)
include_directories(${3RDPARTY_ROOT}/ffmpeg/include/)
include_directories(${BSP_ROOT}/include/)
include_directories(${BSP_ROOT}/include/snpe/)

set(GLIB_LIB ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/glib-2.0/lib/libglib-2.0.so.0.5800.0)
set(GMODULE ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/glib-2.0/lib/libgmodule-2.0.so.0.5800.0)
set(GOBJECT ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/glib-2.0/lib/libgobject-2.0.so.0.5800.0)
set(PCRE_LIB ${CMAKE_CURRENT_BINARY_DIR}/${BSP_ROOT}/lib/libpcre.so.1)
set(FFI_LIB ${CMAKE_CURRENT_BINARY_DIR}/${BSP_ROOT}/lib/libffi.so.6)
set(GST_LIB ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/gstreamer-1.0/lib/libgstreamer-1.0.so.0.1603.0)
set(SDK_LIB ${CMAKE_CURRENT_BINARY_DIR}/${SDK_ROOT}/project/libMitacCameraSdk.so)
set(FFMPEG_AVCODEC ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/ffmpeg/lib/libavcodec.so.58)
set(FFMPEG_AVFORMAT ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/ffmpeg/lib/libavformat.so.58)
set(FFMPEG_AVUTIL ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/ffmpeg/lib/libavutil.so.56)
set(FFMPEG_SWRESAMPLE ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/ffmpeg/lib/libswresample.so.3)
set(FFMPEG_SWSCALE ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/ffmpeg/lib/libswscale.so.5)
set(SNPE_LIB ${CMAKE_CURRENT_BINARY_DIR}/${BSP_ROOT}/lib/snpe/libSNPE.so)
set(FASTCV_DSP ${CMAKE_CURRENT_BINARY_DIR}/${BSP_ROOT}/lib/libfastcvdsp_stub.so)
set(FASTCV_OPT ${CMAKE_CURRENT_BINARY_DIR}/${BSP_ROOT}/lib/libfastcvopt.so)

add_executable(RecordingWithCarDetect ../RecordingWithCarDetect.cpp ../FfmpegMuxer.cpp ../SNPELib.cpp ../FastCVHelper.cpp)

target_link_libraries(RecordingWithCarDetect ${GLIB_LIB} ${GMODULE} ${GOBJECT} ${PCRE_LIB} ${FFI_LIB} ${GST_LIB} ${SDK_LIB} ${FFMPEG_AVCODEC} ${FFMPEG_AVFORMAT} ${FFMPEG_AVUTIL} ${FFMPEG_SWRESAMPLE} ${FFMPEG_SWSCALE} ${SNPE_LIB} ${FASTCV_DSP}
${FASTCV_OPT} stdc++ -pthread)
