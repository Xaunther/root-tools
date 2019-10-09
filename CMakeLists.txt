#Version
cmake_minimum_required(VERSION 3.3.2)

#Project name
project(aalfonso-tools)

#Set executable directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

#Add flags to compile using root libraries
execute_process(COMMAND root-config --cflags --libs OUTPUT_VARIABLE CMAKE_CXX_FLAGS)
string(REGEX REPLACE "\n$" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

#Bring the headers, such as Student.h into the project
include_directories(../Dictionaries ../Functions)

#My two libraries
file(GLOB DICTIONARIES "../Dictionaries/*.cxx")
file(GLOB FUNCTIONS "../Functions/*.cxx")
add_library(aalfonso-dict ${DICTIONARIES})
add_library(aalfonso-func ${FUNCTIONS})

#Add script files, that have the main function
add_executable(AddBranch.out AddBranch.C)
add_executable(AppendVars.out AppendVars.C)
add_executable(BDTApply.out BDTApply.C)
add_executable(BDTCut.out BDTCut.C)
add_executable(BDTTrain2.out BDTTrain2.C)
add_executable(Compare.out Compare.C)
add_executable(Compute_Value.out Compute_Value.C)
add_executable(CutEff.out CutEff.C)
add_executable(CutTree.out CutTree.C)
#add_executable(caca.out caca.C)

#Linking to compile
target_link_libraries(AddBranch.out aalfonso-dict aalfonso-func)
target_link_libraries(AppendVars.out aalfonso-dict aalfonso-func)
target_link_libraries(BDTApply.out aalfonso-dict aalfonso-func)
target_link_libraries(BDTCut.out aalfonso-dict aalfonso-func)
target_link_libraries(BDTTrain2.out aalfonso-dict aalfonso-func)
target_link_libraries(Compare.out aalfonso-dict aalfonso-func)
target_link_libraries(Compute_Value.out aalfonso-dict aalfonso-func)
target_link_libraries(CutEff.out aalfonso-dict aalfonso-func)
target_link_libraries(CutTree.out aalfonso-dict aalfonso-func)
#target_link_libraries(caca.out aalfonso-dict)
