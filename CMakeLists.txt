cmake_minimum_required(VERSION 3.16)

project(AVA LANGUAGES CXX)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 12)
        message(FATAL_ERROR "GCC version must be at least 12!  " ${CMAKE_CXX_COMPILER_VERSION})
    endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 14)
		message(FATAL_ERROR "Clang version must be at least 14!  " ${CMAKE_CXX_COMPILER_VERSION})
    endif()
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(avalanche)

target_sources(avalanche
PUBLIC 
	include/string_functions.hpp
	src/main.cpp
)

target_compile_options(avalanche PRIVATE -Wall -Wextra -Wpedantic)

target_include_directories(avalanche PUBLIC include/)
target_link_libraries(avalanche -lcrypto)
