cmake_minimum_required(VERSION 3.5)
project(SDK C)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_C_STANDARD 11)

##############################################################################################################
## OPTIONS

option(SDK_USE_TLSF "Use tlsf Allocator" ON)

##############################################################################################################
## TARGET

add_library(${PROJECT_NAME} "")

target_compile_definitions(${PROJECT_NAME} PUBLIC SDK_SIZEOF_VOID_P=${CMAKE_SIZEOF_VOID_P})

if (SDK_USE_TLSF)
    target_compile_definitions(${PROJECT_NAME} PUBLIC SDK_USE_TLSF=1)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Pub/TLSF-2.4.6 ${CMAKE_CURRENT_BINARY_DIR}/TLSF)
    target_link_libraries(${PROJECT_NAME} PUBLIC TLSF)
endif ()

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
list(FILTER SOURCES EXCLUDE REGEX \\.t\\.c)
foreach (item IN LISTS SOURCES)
    message(STATUS "[${PROJECT_NAME}] SOURCES: ${item}")
endforeach ()
target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

message(STATUS "CMAKE_C_COMPILER_ID:${CMAKE_C_COMPILER_ID}")
set(COMPILER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Compiler/${CMAKE_C_COMPILER_ID})
file(GLOB COMPILER_SOURCES ${COMPILER_SOURCE_DIR}/*.c)
foreach (item IN LISTS COMPILER_SOURCES)
    message(STATUS "[${PROJECT_NAME}] COMPILER_SOURCES: ${item}")
endforeach ()
target_sources(${PROJECT_NAME} PRIVATE ${COMPILER_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${COMPILER_SOURCE_DIR})


##############################################################################################################
## TEST
file(GLOB TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.t.c)
foreach (item IN LISTS TEST_SOURCES)
    get_filename_component(TestUnit ${item} NAME_WLE)
    message(STATUS "[${PROJECT_NAME}] TEST_SOURCES: ${TestUnit} - ${item}")
    add_executable(${TestUnit} ${item})
    target_link_libraries(${TestUnit} PRIVATE ${PROJECT_NAME})
endforeach ()

