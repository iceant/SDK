cmake_minimum_required(VERSION 3.5)
project(TLSF C)

option(TLSF_USE_PRINTF "TLSF Use printf" ON)
option(TLSF_USE_LOCKS "TLSF Use Locks" OFF)
option(TLSF_STATISTIC "TLSF Statistic feature" OFF)
option(TLSF_USE_MMAP "TLSF Use mmap" OFF)
option(TLSF_USE_SBRK "TLSF Use sbrk" OFF)

add_library(${PROJECT_NAME} "")

set(TLSF_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB TLSF_SOURCES ${TLSF_DIR}/*.c)
foreach (item IN LISTS TLSF_SOURCES)
    message(STATUS "[${PROJECT_NAME}] TLSF_SOURCES: ${item}")
endforeach ()
target_sources(${PROJECT_NAME} PUBLIC ${TLSF_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${TLSF_DIR})

if(TLSF_USE_PRINTF)
    target_compile_options(${PROJECT_NAME} PUBLIC -DUSE_PRINTF=1)
endif ()

if(TLSF_USE_LOCKS)
    target_compile_options(${PROJECT_NAME} PUBLIC TLSF_USE_LOCKS=1)
endif ()

if(TLSF_STATISTIC)
    target_compile_options(${PROJECT_NAME} PUBLIC TLSF_STATISTIC=1)
endif ()

if(TLSF_USE_MMAP)
    target_compile_options(${PROJECT_NAME} PUBLIC USE_MMAP=1)
endif ()

if(TLSF_USE_SBRK)
    target_compile_options(${PROJECT_NAME} PUBLIC USE_SBRK=1)
endif ()
