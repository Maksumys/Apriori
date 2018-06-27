cmake_minimum_required(VERSION 3.5)
project(test_app)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -Ofast)

set(
        SOURCES
        main.cpp
        commandLine.hpp
        command_line.cpp
        apriori.cpp apriori.hpp)

add_executable(test_app ${SOURCES})