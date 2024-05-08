################################################################################################################
#                                                                                                              #
# Copyright      2024 MiTAC International Corp.                                                                #
#                                                                                                              #
#--------------------------------------------------------------------------------------------------------------#
# PROJECT     :  SmartCable Example                                                                            #
# BINARY NAME :  SmartCableReader                                                                              #
# FILE NAME   :  CMakeLists.txt                                                                                #
# CREATED BY  :  Jihung Li <jihung.li@mic.com.tw>                                                              #
# CREATED DATE:  04/26/24 (MM/DD/YY)                                                                           #
################################################################################################################

cmake_minimum_required(VERSION 3.4.1)

project(SmartCable)

set(3RDPARTY_ROOT ../../../3rdParty)
set(BASP_ROOT ../../../bsp)

set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_LINKER aarch64-linux-gnu-gcc)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../)
include_directories(${3RDPARTY_ROOT}/curl/include/)
include_directories(${3RDPARTY_ROOT}/openssl/include/)

set(GLIBC_LIB ${CMAKE_CURRENT_BINARY_DIR}/${BASP_ROOT}/lib/libc.so.6)
set(SSL_LIB ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/openssl/lib/libssl.a)
set(CRYPTO_LIB ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/openssl/lib/libcrypto.a)
set(CURL_LIB ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/curl/lib/libcurl.a)

add_executable(SmartCableReader ../SmartCableReader.cpp ../SerialPort.cpp ../DataPacket.cpp)

target_link_libraries(SmartCableReader ${CURL_LIB} ${GLIBC_LIB} ${SSL_LIB} stdc++ -lpthread -lm -Wl,--no-as-needed -ldl ${CRYPTO_LIB})
