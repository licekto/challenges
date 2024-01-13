cmake_minimum_required(VERSION 3.21)

project(advent-of-code-2022)

find_package(Catch2 REQUIRED)
include(CTest)
include(Catch)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pedantic -Wall -Wextra -Wno-unknown-pragmas -O0 -ggdb -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
link_libraries(stdc++fs)
set(PROJECT_ROOT_DIR_DEF ${PROJECT_SOURCE_DIR})
configure_file(src/Paths.hpp.in ${CMAKE_BINARY_DIR}/generated/Paths.hpp)

add_executable(${PROJECT_NAME} src/main.cpp
                           src/01.cpp src/01.hpp
                           src/02.cpp src/02.hpp
                           src/03.cpp src/03.hpp
                           src/04.cpp src/04.hpp
                           src/05.cpp src/05.hpp
                           src/06.cpp src/06.hpp
                           src/07.cpp src/07.hpp
                           src/08.cpp src/08.hpp
                           src/09.cpp src/09.hpp
                           src/10.cpp src/10.hpp
                           src/11.cpp src/11.hpp
                           src/12.cpp src/12.hpp
                           src/13.cpp src/13.hpp
                           src/14.cpp src/14.hpp
                           src/15.cpp src/15.hpp
                           src/16.cpp src/16.hpp
                           src/17.cpp src/17.hpp
                           src/18.cpp src/18.hpp
                           src/19.cpp src/19.hpp
                           src/20.cpp src/20.hpp
                           src/21.cpp src/21.hpp
                           src/22.cpp src/22.hpp
                           src/23.cpp src/23.hpp
                           src/24.cpp src/24.hpp
                           src/25.cpp src/25.hpp
                           src/Utility.cpp src/Utility.hpp)

target_link_libraries(${PROJECT_NAME} Catch2::Catch2WithMain)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
include_directories(${CMAKE_BINARY_DIR}/generated/)
