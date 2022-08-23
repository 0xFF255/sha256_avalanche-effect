cmake_minimum_required(VERSION 3.16)

project(AVA LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(avalanche)

target_sources(avalanche
	PUBLIC include/string_functions.hpp

			src/main.cpp
			src/string_functions.cpp
)

target_compile_options(avalanche PRIVATE -Wall -Wextra -Wpedantic)

target_include_directories(avalanche PUBLIC include/)
target_link_libraries(avalanche -lcrypto)
