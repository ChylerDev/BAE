cmake_minimum_required(VERSION 3.8)

project(AudioEngine CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include_directories(${PROJECT_SOURCE_DIR}/inc)

if(WIN32)
  link_directories(BEFORE ${PROJECT_SOURCE_DIR}/lib/win)
  set(CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/lib/win ${CMAKE_PREFIX_PATH})
elseif(UNIX)
  link_directories(BEFORE ${PROJECT_SOURCE_DIR}/lib/linux)
  set(CMAKE_PREFIX_PATH ${PROJECT_SOURCE_DIR}/lib/linux ${CMAKE_PREFIX_PATH})
endif()

if(MSVC)
  set(COMPILER_FLAGS "/DWin32 /D_WINDOWS /W4 /WX /GR /EHsc ${CMAKE_CXX_FLAGS}" CACHE STRING "Set compiler flags here")
  set(DEBUG_FLAG "")
else()
  set(COMPILER_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wconversion\
  -Winline -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align\
  -Wwrite-strings -Wcast-qual -Wswitch-default -Wswitch-enum -Wunreachable-code\
  -Wduplicated-cond -Wnull-dereference -Wold-style-cast -Wdouble-promotion\
  -Wformat=2 -Wpedantic -Weffc++ -g -std=c++1z -fPIC ${CMAKE_CXX_FLAGS}"
                      CACHE STRING "Set compiler flags here")
  set(DEBUG_FLAG "-D_DEBUG -pg" CACHE STRING "\"-D_DEBUG -pg\" or \"-DNDEBUG -O4 -g\"")
endif()

set(CMAKE_CXX_FLAGS "${COMPILER_FLAGS} ${DEBUG_FLAGS}")

if(WIN32)
  find_library(PortAudio portaudio_x64)
else()
  find_library(PortAudio portaudio)
endif()

set(core_files
      ${CMAKE_SOURCE_DIR}/Core/Driver.cpp ${CMAKE_SOURCE_DIR}/Core/Driver.hpp
)
set(gen_files
      ${CMAKE_SOURCE_DIR}/Generators/Sine.cpp ${CMAKE_SOURCE_DIR}/Generators/Sine.hpp
)
set(trace_files
      ${CMAKE_SOURCE_DIR}/Trace/Trace.cpp ${CMAKE_SOURCE_DIR}/Trace/Trace.hpp
)
set(engine_files
      ${CMAKE_SOURCE_DIR}/Types.hpp ${CMAKE_SOURCE_DIR}/Macro.hpp
      ${CMAKE_SOURCE_DIR}/main.cpp
      ${CMAKE_SOURCE_DIR}/Engine.hpp
)

source_group("Core" FILES ${core_files})
source_group("Generators" FILES ${gen_files})
source_group("Trace" FILES ${trace_files})
source_group("Engine" FILES ${engine_files})

add_executable(${PROJECT_NAME}
                 ${core_files}
                 ${gen_files}
                 ${trace_files}
                 ${engine_files}
)

target_link_libraries(${PROJECT_NAME}
                        "${PortAudio}"
)

if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  set_target_properties(${PROJECT_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS true)
endif()

if(WIN32)
  add_custom_command(TARGET AudioEngine POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_directory
                     "${PROJECT_SOURCE_DIR}/lib/win"
                     $<TARGET_FILE_DIR:AudioEngine>
                     COMMENT "Copying run-time dependencies"
  )
else()
  add_custom_command(TARGET AudioEngine POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_directory
                     "${PROJECT_SOURCE_DIR}/lib/linux"
                     $<TARGET_FILE_DIR:AudioEngine>
                     COMMENT "Copying run-time dependencies"
  )
endif()
