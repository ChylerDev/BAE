cmake_minimum_required(VERSION 3.7)

project(Tests CXX)

set(CMAKE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/)
set(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/../../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(PROJECT_LIB_DIR ${PROJECT_SOURCE_DIR}/../../lib)
set(PROJECT_INC_DIR ${PROJECT_SOURCE_DIR}/../../inc)

include_directories(
  ${PROJECT_INC_DIR}
)

if(WIN32)
  set(PLATFORM_LIB_DIR ${PROJECT_LIB_DIR}/win)
elseif(UNIX)
  set(PLATFORM_LIB_DIR ${PROJECT_LIB_DIR}/linux)
else()
  set(PLATFORM_LIB_DIR ${PROJECT_LIB_DIR})
endif()

link_directories(BEFORE ${PLATFORM_LIB_DIR})
set(CMAKE_PREFIX_PATH ${PLATFORM_LIB_DIR} ${CMAKE_PREFIX_PATH})

set(CMAKE_CXX_STANDARD 17)

set(src_files
      ${CMAKE_SOURCE_DIR}/main.cpp
)

source_group("Source" FILES ${src_files})

add_executable(${PROJECT_NAME}
                 ${src_files}
)

target_link_libraries(${PROJECT_NAME}
                        OCAE
)

if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  set_target_properties(${PROJECT_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS true)
endif()
