cmake_minimum_required(VERSION 3.20)

set(CMAKE_BUILD_TYPE Release)
project(luac_viewer
	VERSION 1.0.0
	DESCRIPTION "some test to understand the lua bytecode format"
	LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PROJECT_LIB_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/lib")

add_compile_options("-O2")
add_compile_options("-stdlib=libstdc++")

add_subdirectory(src)
