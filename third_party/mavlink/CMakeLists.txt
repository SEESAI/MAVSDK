cmake_minimum_required(VERSION 3.13)

project(external-mavlink)
include(ExternalProject)

list(APPEND CMAKE_ARGS
    "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}"
    "-DCMAKE_TOOLCHAIN_FILE:PATH=${CMAKE_TOOLCHAIN_FILE}"
    "-DMAVLINK_DIALECT=${MAVLINK_DIALECT}"
    )

set(MAVLINK_GIT_HASH 7693cbecce7a935c5c96a0e05a699a8ffed9d625)

if(IOS)
    ExternalProject_add(
        mavlink
        GIT_REPOSITORY https://github.com/SEESAI/mavlink
        GIT_TAG ${MAVLINK_GIT_HASH}
        PREFIX mavlink
        PATCH_COMMAND git checkout .
        COMMAND git apply ${PROJECT_SOURCE_DIR}/mavlink.patch
        COMMAND git apply ${PROJECT_SOURCE_DIR}/mavlink_ios.patch
        CMAKE_ARGS "${CMAKE_ARGS}"
    )
else()
    ExternalProject_add(
        mavlink
        GIT_REPOSITORY https://github.com/SEESAI/mavlink
        GIT_TAG ${MAVLINK_GIT_HASH}
        PREFIX mavlink
        PATCH_COMMAND git checkout . && git apply ${PROJECT_SOURCE_DIR}/mavlink.patch
        CMAKE_ARGS "${CMAKE_ARGS}"
    )
endif()
