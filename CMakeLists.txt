cmake_minimum_required(VERSION 3.10)

project("TIN")

if(APPLE)
    message(FATAL_ERROR "OSX is not supported by this project.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
enable_testing()
