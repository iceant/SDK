cmake_minimum_required(VERSION 3.5)
project(Lua C)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
list(REMOVE_ITEM SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/lua.c)
list(REMOVE_ITEM SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/luac.c)

foreach (item IN LISTS SOURCES)
    message(STATUS "[${PROJECT_NAME}] SOURCES: ${item}")
endforeach ()

add_library(${PROJECT_NAME} ${SOURCES})

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_options(${PROJECT_NAME} PUBLIC LUA_USE_LINUX)
    target_compile_options(${PROJECT_NAME} PUBLIC LUA_USE_READLINE)
    target_link_libraries(${PROJECT_NAME} PUBLIC dl)
    target_link_libraries(${PROJECT_NAME} PUBLIC readline)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_compile_options(${PROJECT_NAME} PUBLIC LUA_USE_MACOSX)
    target_compile_options(${PROJECT_NAME} PUBLIC LUA_USE_READLINE)
    target_link_libraries(${PROJECT_NAME} PUBLIC readline)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")

elseif (CMAKE_SYSTEM_NAME STREQUAL "AIX")
    target_compile_options(${PROJECT_NAME} PUBLIC LUA_USE_POSIX)
    target_compile_options(${PROJECT_NAME} PUBLIC LUA_USE_DLOPEN)
    target_link_libraries(${PROJECT_NAME} PUBLIC dl)
    target_link_options(${PROJECT_NAME} PUBLIC -brtl)
    target_link_options(${PROJECT_NAME} PUBLIC -bexpall)
endif ()


add_executable(luac ${CMAKE_CURRENT_SOURCE_DIR}/src/luac.c)
target_link_libraries(luac ${PROJECT_NAME})

add_executable(lua ${CMAKE_CURRENT_SOURCE_DIR}/src/lua.c)
target_link_libraries(lua ${PROJECT_NAME})
