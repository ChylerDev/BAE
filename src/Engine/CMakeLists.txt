cmake_minimum_required(VERSION 3.7)

project(AudioEngine CXX)

set(CMAKE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/)
set(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/../../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(PROJECT_LIB_DIR ${PROJECT_SOURCE_DIR}/../../lib)
set(PROJECT_INC_DIR ${PROJECT_SOURCE_DIR}/../../inc)

include_directories(${PROJECT_INC_DIR})

if(WIN32)
  set(PLATFORM_LIB_DIR ${PROJECT_LIB_DIR}/win)
elseif(UNIX)
  set(PLATFORM_LIB_DIR ${PROJECT_LIB_DIR}/linux)
else()
  set(PLATFORM_LIB_DIR ${PROJECT_LIB_DIR})
endif()

link_directories(BEFORE ${PLATFORM_LIB_DIR})
set(CMAKE_PREFIX_PATH ${PLATFORM_LIB_DIR} ${CMAKE_PREFIX_PATH})

if(WIN32)
  find_library(PortAudio portaudio_x64)
else()
  find_library(PortAudio portaudio)
endif()

set(CMAKE_CXX_STANDARD 17)

set(core_files
      ${CMAKE_SOURCE_DIR}/Core/Driver.cpp ${CMAKE_SOURCE_DIR}/Core/Driver.hpp
)
set(gen_files
      ${CMAKE_SOURCE_DIR}/Generators/Sine.cpp ${CMAKE_SOURCE_DIR}/Generators/Sine.hpp
      ${CMAKE_SOURCE_DIR}/Generators/Square.cpp ${CMAKE_SOURCE_DIR}/Generators/Square.hpp
      ${CMAKE_SOURCE_DIR}/Generators/WAV.cpp ${CMAKE_SOURCE_DIR}/Generators/WAV.hpp
)
set(modifier_files
      ${CMAKE_SOURCE_DIR}/Modifiers/GenericFilter.cpp ${CMAKE_SOURCE_DIR}/Modifiers/GenericFilter.hpp
      ${CMAKE_SOURCE_DIR}/Modifiers/LowPass.cpp ${CMAKE_SOURCE_DIR}/Modifiers/LowPass.hpp
      ${CMAKE_SOURCE_DIR}/Modifiers/BandPass.cpp ${CMAKE_SOURCE_DIR}/Modifiers/BandPass.hpp
      ${CMAKE_SOURCE_DIR}/Modifiers/Vocoder.cpp ${CMAKE_SOURCE_DIR}/Modifiers/Vocoder.hpp
      ${CMAKE_SOURCE_DIR}/Modifiers/Envelope.cpp ${CMAKE_SOURCE_DIR}/Modifiers/Envelope.hpp
)
set(tools_files
      ${CMAKE_SOURCE_DIR}/Tools/Input.cpp ${CMAKE_SOURCE_DIR}/Tools/Input.hpp
      ${CMAKE_SOURCE_DIR}/Tools/Resampler.cpp ${CMAKE_SOURCE_DIR}/Tools/Resampler.hpp
)
set(engine_files
      ${CMAKE_SOURCE_DIR}/Types.hpp ${CMAKE_SOURCE_DIR}/Macro.hpp
      ${CMAKE_SOURCE_DIR}/Engine.hpp
)

source_group("Core" FILES ${core_files})
source_group("Engine" FILES ${engine_files})
source_group("Generators" FILES ${gen_files})
source_group("Modifiers" FILES ${modifier_files})
source_group("Tools" FILES ${tools_files})

add_library(${PROJECT_NAME} STATIC
              ${core_files}
              ${engine_files}
              ${gen_files}
              ${modifier_files}
              ${tools_files}
)

target_link_libraries(${PROJECT_NAME}
                        ${PortAudio}
                        Trace
)

if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  set_target_properties(${PROJECT_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS true)
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                    "${PLATFORM_LIB_DIR}"
                    $<TARGET_FILE_DIR:${PROJECT_NAME}>
                    COMMENT "Copying run-time dependencies"
)
