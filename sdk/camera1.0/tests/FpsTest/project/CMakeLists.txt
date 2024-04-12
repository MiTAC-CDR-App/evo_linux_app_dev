################################################################################################################
#                                                                                                              #
# Copyright      2024 MiTAC International Corp.                                                                #
#                                                                                                              #
#--------------------------------------------------------------------------------------------------------------#
# PROJECT     :  Camera SDK Example Support                                                                    #
# BINARY NAME :  FpsTest                                                                                       #
# FILE NAME   :  CMakeLists.txt                                                                                #
# CREATED BY  :  Forest.Yeh <forest.yeh@mic.com.tw>                                                            #
# CREATED DATE:  03/11/24 (MM/DD/YY)                                                                           #
################################################################################################################

cmake_minimum_required(VERSION 3.4.1)

project(FpsTest)

set(3RDPARTY_ROOT ../../../../../3rdParty)
set(BSP_ROOT ../../../../../bsp)
set(SDK_ROOT ../../..)

set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_LINKER aarch64-linux-gnu-gcc)

include_directories(${SDK_ROOT}/include/)

set(GLIB_LIB ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/glib-2.0/lib/libglib-2.0.so.0.5800.0)
set(GMODULE ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/glib-2.0/lib/libgmodule-2.0.so.0.5800.0)
set(GOBJECT ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/glib-2.0/lib/libgobject-2.0.so.0.5800.0)
set(PCRE_LIB ${CMAKE_CURRENT_BINARY_DIR}/${BSP_ROOT}/lib/libpcre.so.1)
set(FFI_LIB ${CMAKE_CURRENT_BINARY_DIR}/${BSP_ROOT}/lib/libffi.so.6)
set(GST_LIB ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/gstreamer-1.0/lib/libgstreamer-1.0.so.0.1603.0)
set(SDK_LIB ${CMAKE_CURRENT_BINARY_DIR}/${SDK_ROOT}/project/libMitacCameraSdk.so)

add_executable(FpsTest ../FpsTest.cpp )

target_link_libraries(FpsTest ${GLIB_LIB} ${GMODULE} ${GOBJECT} ${PCRE_LIB} ${FFI_LIB} ${GST_LIB} ${SDK_LIB} stdc++ -pthread)
