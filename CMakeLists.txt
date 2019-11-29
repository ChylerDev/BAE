cmake_minimum_required(VERSION 3.8)

if(WIN32)
  cmake_policy(SET CMP0091 NEW)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
endif()

project(AudioEngine)

# Allow adding of custom compiler flags
if(MSVC)
	set(COMPILER_FLAGS "/W4 /WX /EHsc" CACHE STRING "Set custom MSVC flags here")
else()
	set(COMPILER_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wconversion\
		-Winline -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align\
		-Wwrite-strings -Wcast-qual -Wswitch-default -Wswitch-enum -Wunreachable-code\
		-Wnull-dereference -Wold-style-cast -Wdouble-promotion -Wformat=2 -Wpedantic\
		-Weffc++ -g -std=c++1z ${CMAKE_CXX_FLAGS}"
		CACHE STRING "Set custom compiler flags here"
	)
	# Add _DEBUG define and profiling to debug flags
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG -pg")
endif()

# Add the custom flags to the compile flags
set(CMAKE_CXX_FLAGS "${COMPILER_FLAGS} ${DEBUG_FLAGS}")

add_subdirectory(src/Engine)
add_subdirectory(src/Tests)
