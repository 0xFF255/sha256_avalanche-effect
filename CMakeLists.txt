cmake_minimum_required(VERSION 3.21)

project(AVA LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(avalance)

target_sources(avalance
	PUBLIC include/string_functions.hpp

			src/main.cpp
			src/string_functions.cpp
)

target_compile_options(avalance PRIVATE -Wall -Wextra -Wpedantic)

target_include_directories(avalance PUBLIC include/)
target_link_libraries(avalance -lcrypto)
