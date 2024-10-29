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
## Crypto
set(CRYPTO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Crypto)
file(GLOB CRYPTO_SOURCES ${CRYPTO_DIR}/*.c)
list(FILTER CRYPTO_SOURCES EXCLUDE REGEX \\.t\\.c)
foreach (item IN LISTS CRYPTO_SOURCES)
    message(STATUS "[${PROJECT_NAME}] CRYPTO_SOURCES: ${item}")
endforeach ()
target_sources(${PROJECT_NAME} PRIVATE ${CRYPTO_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CRYPTO_DIR})

##############################################################################################################
## Network

file(GLOB NETWORK_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Network/*.c)
list(FILTER NETWORK_SOURCES EXCLUDE REGEX \\.t\\.c)
foreach (item IN LISTS NETWORK_SOURCES)
    message(STATUS "[${PROJECT_NAME}] NETWORK_SOURCES: ${item}")
endforeach ()
target_sources(${PROJECT_NAME} PRIVATE ${NETWORK_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Network)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(${PROJECT_NAME} PUBLIC ws2_32)
endif ()

##############################################################################################################
## GameAI

file(GLOB GAME_AI_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/GameAI/*.c)
list(FILTER GAME_AI_SOURCES EXCLUDE REGEX \\.t\\.c)
foreach (item IN LISTS GAME_AI_SOURCES)
    message(STATUS "[${PROJECT_NAME}] GAME_AI_SOURCES: ${item}")
endforeach ()
target_sources(${PROJECT_NAME} PRIVATE ${GAME_AI_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/GameAI)


##############################################################################################################
## TEST
file(GLOB TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.t.c)
foreach (item IN LISTS TEST_SOURCES)
    get_filename_component(TestUnit ${item} NAME_WLE)
    message(STATUS "[${PROJECT_NAME}] TEST_SOURCES: ${TestUnit} - ${item}")
    add_executable(${TestUnit} ${item})
    target_link_libraries(${TestUnit} PRIVATE ${PROJECT_NAME})
endforeach ()

##############################################################################################################
## CRYPTO TEST
file(GLOB CRYPTO_TEST_SOURCES ${CRYPTO_DIR}/*.t.c)
foreach (item IN LISTS CRYPTO_TEST_SOURCES)
    get_filename_component(TestUnit ${item} NAME_WLE)
    message(STATUS "[${PROJECT_NAME}] CRYPTO_TEST_SOURCES: ${TestUnit} - ${item}")
    add_executable(${TestUnit} ${item})
    target_link_libraries(${TestUnit} PRIVATE ${PROJECT_NAME})
endforeach ()


##############################################################################################################
## NETWORK TEST
file(GLOB NETWORK_TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Network/*.t.c)
foreach (item IN LISTS NETWORK_TEST_SOURCES)
    get_filename_component(TestUnit ${item} NAME_WLE)
    message(STATUS "[${PROJECT_NAME}] NETWORK_TEST_SOURCES: ${TestUnit} - ${item}")
    add_executable(${TestUnit} ${item})
    target_link_libraries(${TestUnit} PRIVATE ${PROJECT_NAME})
endforeach ()

##############################################################################################################
## GameAI TEST
file(GLOB GAME_AI_TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/GameAI/*.t.c)
foreach (item IN LISTS GAME_AI_TEST_SOURCES)
    get_filename_component(TestUnit ${item} NAME_WLE)
    message(STATUS "[${PROJECT_NAME}] GAME_AI_TEST_SOURCES: ${TestUnit} - ${item}")
    add_executable(${TestUnit} ${item})
    target_link_libraries(${TestUnit} PRIVATE ${PROJECT_NAME})
endforeach ()


##############################################################################################################
## Lua
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/Pub/Lua-5.4.7)