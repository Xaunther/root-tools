#Version
cmake_minimum_required(VERSION 3.3.2)

#Project name
project(caca)

#Set executable directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

#Add flags to compile using root libraries
execute_process(COMMAND root-config --cflags --glibs OUTPUT_VARIABLE CMAKE_CXX_FLAGS)
string(REGEX REPLACE "\n$" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

#Bring the headers, such as Student.h into the project
include_directories(../Dictionaries ../Functions)

#My two libraries
file(GLOB DICTIONARIES "../Dictionaries/*.cxx")
file(GLOB FUNCTIONS "../Functions/*.cxx")
add_library(aalfonso-dict ${DICTIONARIES})
add_library(aalfonso-func ${FUNCTIONS})

#Add script files, that have the main function
add_executable(caca.out caca.C)

#Linking to compile
target_link_libraries(caca.out aalfonso-dict aalfonso-func)
