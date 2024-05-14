cmake_minimum_required(VERSION 3.4.1)

project(Live555Test)

set(3RDPARTY_ROOT ../../../../../../3rdParty)
set(BSP_ROOT ../../../../../../bsp)
set(SDK_ROOT ../../../..)

set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_LINKER aarch64-linux-gnu-gcc)

include_directories(${SDK_ROOT}/include/)
include_directories(${3RDPARTY_ROOT}/openssl/include)
include_directories(${3RDPARTY_ROOT}/live555/include/BasicUsageEnvironment)
include_directories(${3RDPARTY_ROOT}/live555/include/groupsock)
include_directories(${3RDPARTY_ROOT}/live555/include/liveMedia)
include_directories(${3RDPARTY_ROOT}/live555/include/UsageEnvironment)

set(GLIB_LIB ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/glib-2.0/lib/libglib-2.0.so.0.5800.0)
set(GMODULE ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/glib-2.0/lib/libgmodule-2.0.so.0.5800.0)
set(GOBJECT ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/glib-2.0/lib/libgobject-2.0.so.0.5800.0)
set(PCRE_LIB ${CMAKE_CURRENT_BINARY_DIR}/${BSP_ROOT}/lib/libpcre.so.1)
set(FFI_LIB ${CMAKE_CURRENT_BINARY_DIR}/${BSP_ROOT}/lib/libffi.so.6)
set(GST_LIB ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/gstreamer-1.0/lib/libgstreamer-1.0.so.0.1603.0)
set(SDK_LIB ${CMAKE_CURRENT_BINARY_DIR}/${SDK_ROOT}/project/libMitacCameraSdk.so)
set(SSL_LIB ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/openssl/lib/libssl.a)
set(CRYPTO_LIB ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/openssl/lib/libcrypto.a)
set(LIVE555_BASIC ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/live555/lib/libBasicUsageEnvironment.a)
set(LIVE555_GROUP ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/live555/lib/libgroupsock.a)
set(LIVE555_LIVE ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/live555/lib/libliveMedia.a)
set(LIVE555_USAGE ${CMAKE_CURRENT_BINARY_DIR}/${3RDPARTY_ROOT}/live555/lib/libUsageEnvironment.a)

add_executable(Live555Test ../Live555Test.cpp)

target_link_libraries(Live555Test ${GLIB_LIB} ${GMODULE} ${GOBJECT} ${PCRE_LIB} ${FFI_LIB} ${GST_LIB} ${SDK_LIB} ${SSL_LIB} ${CRYPTO_LIB} ${LIVE555_LIVE} ${LIVE555_GROUP} ${LIVE555_BASIC} ${LIVE555_USAGE} stdc++ -pthread)
