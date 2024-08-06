# Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0

# snippet-start:[cpp.example_code.s3.hello_s3.cmake]
# Set the minimum required version of CMake for this project.
cmake_minimum_required(VERSION 3.13)

# Set the AWS service components used by this project.
set(SERVICE_COMPONENTS s3 kinesisvideo kinesis-video-archived-media)

# Set this project's name.
project("hello_s3")

# Set the C++ standard to use to build this target.
# At least C++ 11 is required for the AWS SDK for C++.
set(CMAKE_CXX_STANDARD 11)

# Use the MSVC variable to determine if this is a Windows build.
set(WINDOWS_BUILD ${MSVC})

if (WINDOWS_BUILD) # Set the location where CMake can find the installed libraries for the AWS SDK.
    string(REPLACE ";" "/aws-cpp-sdk-all;" SYSTEM_MODULE_PATH "${CMAKE_SYSTEM_PREFIX_PATH}/aws-cpp-sdk-all")
    list(APPEND CMAKE_PREFIX_PATH ${SYSTEM_MODULE_PATH})
endif ()


set(THIRDPARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)
message(STATUS "THIRDPARTY_DIR: ${THIRDPARTY_DIR}")

set(AWSCPPSDK_DIR ${THIRDPARTY_DIR}/aws-cpp-sdk-all)
message(STATUS "AWSCPPSDK_DIR: ${AWSCPPSDK_DIR}")

if (NOT EXISTS ${AWSCPPSDK_DIR})
    message(FATAL_ERROR "AWS C++ SDK directory not found: ${AWSCPPSDK_DIR}")
endif()


set(AWSSDK_CORE_HEADER_FILE "${AWSCPPSDK_DIR}/include/aws/core/Aws.h")
set(AWSSDK_CORE_LIB_FILE "${AWSCPPSDK_DIR}/bin/aws-cpp-sdk-core.lib")
set(AWSSDK_DIR "${AWSCPPSDK_DIR}/lib/cmake/AWSSDK")
set(aws-c-auth_DIR "${AWSCPPSDK_DIR}/lib/aws-c-auth/cmake")
set(aws-c-cal_DIR "${AWSCPPSDK_DIR}/lib/aws-c-cal/cmake")
set(aws-c-common_DIR "${AWSCPPSDK_DIR}/lib/aws-c-common/cmake")
set(aws-c-compression_DIR "${AWSCPPSDK_DIR}/lib/aws-c-compression/cmake")
set(aws-c-event-stream_DIR "${AWSCPPSDK_DIR}/lib/aws-c-event-stream/cmake")
set(aws-c-http_DIR "${AWSCPPSDK_DIR}/lib/aws-c-http/cmake")
set(aws-c-io_DIR "${AWSCPPSDK_DIR}/lib/aws-c-io/cmake")
set(aws-c-mqtt_DIR "${AWSCPPSDK_DIR}/lib/aws-c-mqtt/cmake")
set(aws-c-s3_DIR "${AWSCPPSDK_DIR}/lib/aws-c-s3/cmake")
set(aws-c-sdkutils_DIR "${AWSCPPSDK_DIR}/lib/aws-c-sdkutils/cmake")
set(aws-checksums_DIR "${AWSCPPSDK_DIR}/lib/aws-checksums/cmake")
set(aws-cpp-sdk-core_DIR "${AWSCPPSDK_DIR}/lib/cmake/aws-cpp-sdk-core")
set(aws-cpp-sdk-kinesis-video-archived-media_DIR "${AWSCPPSDK_DIR}/lib/cmake/aws-cpp-sdk-kinesis-video-archived-media")
set(aws-cpp-sdk-kinesisvideo_DIR "${AWSCPPSDK_DIR}/lib/cmake/aws-cpp-sdk-kinesisvideo")
set(aws-cpp-sdk-s3_DIR "${AWSCPPSDK_DIR}/lib/cmake/aws-cpp-sdk-s3")
set(aws-crt-cpp_DIR "${AWSCPPSDK_DIR}/lib/aws-crt-cpp/cmake")


# Find the AWS SDK for C++ package.
find_package(AWSSDK REQUIRED COMPONENTS ${SERVICE_COMPONENTS})


set(DLL_FILES
    ${AWSCPPSDK_DIR}/bin/aws-c-auth.dll
    ${AWSCPPSDK_DIR}/bin/aws-c-cal.dll
    ${AWSCPPSDK_DIR}/bin/aws-c-common.dll
    ${AWSCPPSDK_DIR}/bin/aws-c-compression.dll
    ${AWSCPPSDK_DIR}/bin/aws-c-event-stream.dll
    ${AWSCPPSDK_DIR}/bin/aws-c-http.dll
    ${AWSCPPSDK_DIR}/bin/aws-c-io.dll
    ${AWSCPPSDK_DIR}/bin/aws-c-mqtt.dll
    ${AWSCPPSDK_DIR}/bin/aws-c-s3.dll
    ${AWSCPPSDK_DIR}/bin/aws-c-sdkutils.dll
    ${AWSCPPSDK_DIR}/bin/aws-checksums.dll
    ${AWSCPPSDK_DIR}/bin/aws-cpp-sdk-core.dll
    ${AWSCPPSDK_DIR}/bin/aws-cpp-sdk-kinesis.dll
    ${AWSCPPSDK_DIR}/bin/aws-cpp-sdk-s3.dll
    ${AWSCPPSDK_DIR}/bin/aws-crt-cpp.dll
)



file(COPY ${DLL_FILES} DESTINATION ${CMAKE_BINARY_DIR}/Debug)
file(COPY ${DLL_FILES} DESTINATION ${CMAKE_BINARY_DIR}/Release)


if (WINDOWS_BUILD AND AWSSDK_INSTALL_AS_SHARED_LIBS)
    # Copy relevant AWS SDK for C++ libraries into the current binary directory for running and debugging.

    # set(BIN_SUB_DIR "/Debug") # if you are building from the command line you may need to uncomment this
    # and set the proper subdirectory to the executables' location.

    AWSSDK_CPY_DYN_LIBS(SERVICE_COMPONENTS "" ${CMAKE_CURRENT_BINARY_DIR}${BIN_SUB_DIR})
endif ()

add_executable(${PROJECT_NAME}
        hello_s3.cpp)

message(STATUS "AWSSDK_LINK_LIBRARIES: ${AWSSDK_LINK_LIBRARIES}")   
target_link_libraries(${PROJECT_NAME}
        ${AWSSDK_LINK_LIBRARIES})
# snippet-end:[cpp.example_code.s3.hello_s3.cmake]
