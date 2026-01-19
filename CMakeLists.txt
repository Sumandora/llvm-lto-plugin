cmake_minimum_required(VERSION 4.0)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

project(lto-plugin)

set(LT_LLVM_INSTALL_DIR "" CACHE PATH "LLVM installation directory")
list(APPEND CMAKE_PREFIX_PATH "${LT_LLVM_INSTALL_DIR}/lib/cmake/llvm/")

find_package(LLVM CONFIG)
if("${LLVM_VERSION_MAJOR}" VERSION_LESS 21)
  message(FATAL_ERROR "Found LLVM ${LLVM_VERSION_MAJOR}, but need LLVM 21 or above")
endif()

include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 17 CACHE STRING "")

if(NOT LLVM_ENABLE_RTTI)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

add_library(DumpPass SHARED DumpPass.cpp)
target_compile_options(DumpPass PRIVATE "-ggdb3")

target_link_libraries(DumpPass
  "$<$<PLATFORM_ID:Darwin>:-undefined dynamic_lookup>")

