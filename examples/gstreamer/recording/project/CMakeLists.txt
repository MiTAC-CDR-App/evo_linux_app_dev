################################################################################################################
#                                                                                                              #
# Copyright      2024 MiTAC International Corp.                                                                #
#                                                                                                              #
#--------------------------------------------------------------------------------------------------------------#
# PROJECT     :  G Streamer Example Support                                                                    #
# BINARY NAME :  GstreamerRecordingTest                                                                                 #
# FILE NAME   :  CMakeLists.txt                                                                                #
# CREATED BY  :  Forest.Yeh <forest.yeh@mic.com.tw>                                                            #
# CREATED DATE:  02/22/24 (MM/DD/YY)                                                                           #
################################################################################################################

cmake_minimum_required(VERSION 3.4.1)

project(Recording)

set(3RDPARTY_ROOT ../../../../3rdParty)
set(BSP_ROOT ../../../../bsp)

set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_LINKER aarch64-linux-gnu-gcc)

include_directories(${3RDPARTY_ROOT}/glib-2.0/include/)
include_directories(${3RDPARTY_ROOT}/gstreamer-1.0/include/)
include_directories(${3RDPARTY_ROOT}/ffmpeg/include/)

set(GLIB_LIB ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/glib-2.0/lib/libglib-2.0.so.0.5800.0)
set(GMODULE ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/glib-2.0/lib/libgmodule-2.0.so.0.5800.0)
set(GOBJECT ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/glib-2.0/lib/libgobject-2.0.so.0.5800.0)
set(PCRE_LIB ${CMAKE_CURRENT_BINARY_DIR}/${BSP_ROOT}/lib/libpcre.so.1)
set(FFI_LIB ${CMAKE_CURRENT_BINARY_DIR}/${BSP_ROOT}/lib/libffi.so.6)
set(GST_LIB ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/gstreamer-1.0/lib/libgstreamer-1.0.so.0.1603.0)
set(FFMPEG_AVCODEC ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/ffmpeg/lib/libavcodec.so.58)
set(FFMPEG_AVFORMAT ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/ffmpeg/lib/libavformat.so.58)
set(FFMPEG_AVUTIL ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/ffmpeg/lib/libavutil.so.56)
set(FFMPEG_SWRESAMPLE ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/ffmpeg/lib/libswresample.so.3)
set(FFMPEG_SWSCALE ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/ffmpeg/lib/libswscale.so.5)

add_executable(GstreamerRecordingTest ../Recording.cpp ../FfmpegMuxer.cpp)

target_link_libraries(GstreamerRecordingTest ${GLIB_LIB} ${GMODULE} ${GOBJECT} ${PCRE_LIB} ${FFI_LIB} ${GST_LIB} ${FFMPEG_AVCODEC} ${FFMPEG_AVFORMAT} ${FFMPEG_AVUTIL} ${FFMPEG_SWRESAMPLE} ${FFMPEG_SWSCALE} stdc++ -pthread)
