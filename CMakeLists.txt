cmake_minimum_required(VERSION 3.8)
project(SFC2017)

set(CMAKE_CXX_STANDARD 17)

#set(SOURCE_FILES main.cpp)
#add_executable(SFC2017 ${SOURCE_FILES})


file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "src/*.hpp")

add_executable (SFC2017 ${SOURCES} ${HEADERS})
