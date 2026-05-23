cmake_minimum_required(VERSION 3.29)
project(OperatingSystemsA3)

set(CMAKE_CXX_STANDARD 20)

add_executable(OperatingSystemsA3 main.cpp
        log_helpers.cpp
        vaddr_tracereader.cpp
        PageTable.cpp
        PageNode.cpp
        PageTracker.cpp)
