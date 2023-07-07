cmake_minimum_required(VERSION 3.19.2)

project(ssAnimator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

if(APPLE)
    if(("${CMAKE_BUILD_TYPE}" STREQUAL "Release") OR (NOT CMAKE_OSX_ARCHITECTURES))
        set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
    endif()
endif()

set(ssAnimator_SRCS
    ssplayer_cellmap.cpp
    ssplayer_PartState.cpp
    ssplayer_render.cpp
    ssplayer_matrix.cpp
    ssplayer_effect.cpp
    ssplayer_effectfunction.cpp
    ssplayer_animedecode.cpp
    ssplayer_effect2.cpp
    ssplayer_mesh.cpp
)

set(ssAnimator_HEADERS
    ssplayer_types.h
    ssplayer_cellmap.h
    ssplayer_PartState.h
    ssplayer_render.h
    ssplayer_matrix.h
    ssplayer_effect.h
    ssplayer_effectfunction.h
    MersenneTwister.h
    ssplayer_animedecode.h
    ssplayer_effect2.h
    ssplayer_mesh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../Loader/sstypes.h
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Loader)

add_library(ssAnimator ${ssAnimator_SRCS} ${ssAnimator_HEADERS})
