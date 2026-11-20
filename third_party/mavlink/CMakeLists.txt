cmake_minimum_required(VERSION 3.13)

project(external-mavlink)
include(ExternalProject)

list(APPEND CMAKE_ARGS
    "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}"
    "-DCMAKE_TOOLCHAIN_FILE:PATH=${CMAKE_TOOLCHAIN_FILE}"
    "-DMAVLINK_DIALECT=${MAVLINK_DIALECT}"
    )

# SEES: SEESAI/mavlink, the same mavlink as PX4 v1.17 (upstream 33af200d + sees.xml),
# on branch vilas/fix/33af200/sees_gps_input, the same commit Firmware pins.
# Upstream's mavlink.patch applies unchanged.
# See docs/sees/firmware_v1.17.md.
set(MAVLINK_GIT_URL https://github.com/SEESAI/mavlink)
set(MAVLINK_GIT_HASH 49679600b9a584f56c5d4790747c5256b9921bd3)

if(IOS)
    ExternalProject_add(
        mavlink
        GIT_REPOSITORY ${MAVLINK_GIT_URL}
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
        GIT_REPOSITORY ${MAVLINK_GIT_URL}
        GIT_TAG ${MAVLINK_GIT_HASH}
        PREFIX mavlink
        PATCH_COMMAND git checkout . && git apply ${PROJECT_SOURCE_DIR}/mavlink.patch
        CMAKE_ARGS "${CMAKE_ARGS}"
    )
endif()
