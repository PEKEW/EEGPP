cmake_minimum_required(VERSION 3.28)
project(EEGPP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_executable(EEGPP
        main.cpp
        pugixml/pugixml.cpp)

target_include_directories(EEGPP PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/pugixml)