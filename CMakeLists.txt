cmake_minimum_required(VERSION 3.16)

project(lemon)

#find_package (Eigen3 3.3.8 REQUIRED NO_MODULE)
#find_package(Threads REQUIRED)

# TODO: Optionfy this
if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(NOT DEFINED LMN_BUILD_EXECUTABLES)
    option(LMN_BUILD_EXECUTABLES "Build executables (OFF by default, set to ON for building the test executable)" OFF)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(LMN_DEPENDENCY_INCLUDE_DIRS 
    ${EIGEN3_INCLUDE_DIRS}
)
set(LMN_INCLUDE_DIRS 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/lemon
    ${CMAKE_CURRENT_SOURCE_DIR}/lemon/impl
    ${LMN_DEPENDENCY_INCLUDE_DIRS}
    CACHE INTERNAL ""
)

if(LMN_BUILD_EXECUTABLES)
   add_subdirectory(src)
endif()