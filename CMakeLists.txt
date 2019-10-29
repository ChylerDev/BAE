cmake_minimum_required(VERSION 3.8)

project(OCAE)

if(MSVC)
  set(COMPILER_FLAGS "/DWin32 /D_WINDOWS /W4 /WX /GR /EHsc" CACHE STRING "Set compiler flags here")
  set(DEBUG_FLAGS "")
else()
  set(COMPILER_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wconversion\
  -Winline -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align\
  -Wwrite-strings -Wcast-qual -Wswitch-default -Wswitch-enum -Wunreachable-code\
  -Wduplicated-cond -Wnull-dereference -Wold-style-cast -Wdouble-promotion\
  -Wformat=2 -Wpedantic -Weffc++ -g -std=c++1z -fPIC ${CMAKE_CXX_FLAGS}"
                      CACHE STRING "Set compiler flags here")
  set(DEBUG_FLAGS "-D_DEBUG -pg" CACHE STRING "\"-D_DEBUG -pg\" or \"-DNDEBUG -O4 -g\"")
endif()

set(CMAKE_CXX_FLAGS "${COMPILER_FLAGS} ${DEBUG_FLAGS}")

add_subdirectory(src/Engine)
add_subdirectory(src/Tests)
