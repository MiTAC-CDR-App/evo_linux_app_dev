################################################################################################################
#                                                                                                              #
# Copyright      2024 MiTAC International Corp.                                                                #
#                                                                                                              #
#--------------------------------------------------------------------------------------------------------------#
# PROJECT     :  EVO Linxu Example Support                                                                     #
# BINARY NAME :  MqttTest                                                                                      #
# FILE NAME   :  CMakeLists.txt                                                                                #
# CREATED BY  :  Brian Wu <tzung.home@mic.com.tw>                                                              #
# CREATED DATE:  02/21/24 (MM/DD/YY)                                                                           #
################################################################################################################

cmake_minimum_required(VERSION 3.4.1)

project(MqttTest)

set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_LINKER aarch64-linux-gnu-gcc)

include_directories(
    ../../../3rdParty/awsIotSdkCppV2/include)
link_directories(
    ../../../3rdParty/awsIotSdkCppV2/lib
    ../../../3rdParty/openssl/lib)

add_executable(MqttTest ../MqttTest.cpp)

set(MQTT_LIBS aws-c-mqtt aws-c-http aws-c-cal aws-c-compression aws-c-auth aws-c-io aws-c-common s2n)

target_link_libraries(MqttTest stdc++ -pthread ${MQTT_LIBS} m ssl crypto dl)
