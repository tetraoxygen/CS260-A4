cmake_minimum_required(VERSION 3.16)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(sorts)

add_executable(sorts src/main.cpp)

set (CMAKE_OSX_ARCHITECTURES "x86_64;arm64")

set_property(TARGET sorts PROPERTY CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works

install(TARGETS sorts)

IF (WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_link_libraries(sorts -static)
ENDIF()

if (MSVC)
	add_compile_options(/std:c++latest)
endif()