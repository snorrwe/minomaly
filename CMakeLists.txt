cmake_minimum_required(VERSION 3.0)

set(MINOMALY minomaly)

project(${MINOMALY} VERSION 0.1.0 LANGUAGES CXX)

#[ Engine ]

add_library(${MINOMALY} STATIC
                        ${PROJECT_SOURCE_DIR}/src/minomaly.cpp
                        ${PROJECT_SOURCE_DIR}/src/window_system.cpp
                        ${PROJECT_SOURCE_DIR}/src/sdl_subsystems.cpp
                        ${PROJECT_SOURCE_DIR}/src/input_system.cpp
)
target_include_directories(${MINOMALY}
    PUBLIC
    $<BUILD_INTERFACE: ${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>

    PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)
set_property(TARGET ${MINOMALY} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${MINOMALY} PROPERTY CXX_STANDARD_REQUIRED ON)

if(MSVC)
    target_compile_options(${MINOMALY} PRIVATE /W4 /WX)
else(MSVC)
    target_compile_options(${MINOMALY} PRIVATE -Wall -Wextra -pedantic -Werror)
endif(MSVC)

#[ log4cplus ]

if(WIN32)
    set(Win32)
    set(log4cplus_EXPORTS)
    set(UNICODE)
    set(_UNICODE)
endif()
set(LOG4CPLUS_BUILD_TESTING  OFF)
set(WITH_UNIT_TESTS   OFF)
add_subdirectory(${PROJECT_SOURCE_DIR}/external/log4cplus)
target_link_libraries(${MINOMALY} ${log4cplus})

#[ SDL ]

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(VC_LIB_PATH_SUFFIX lib/x64)
else()
  set(VC_LIB_PATH_SUFFIX lib/x86)
endif()

if(WIN32)
  set(SDL2_ROOT ${PROJECT_SOURCE_DIR}/external/sdl/windows-vs)
  set(SDL2_DIR ${SDL2_ROOT}/SDL2-2.0.7)
  set(SDL2_IMAGE_DIR ${SDL2_ROOT}/SDL2_image-2.0.2)
  set(SDL2_TTF_DIR ${SDL2_ROOT}/SDL2_ttf-2.0.14)
  set(SDL2_MIXER_DIR ${SDL2_ROOT}/SDL2_mixer-2.0.2)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_DIR}/include)
include_directories(${SDL2_TTF_DIR}/include)
include_directories(${SDL2_MIXER_DIR}/include)

target_link_libraries(${MINOMALY} ${SDL2_LIBRARIES}
                                  ${SDL2_IMAGE_DIR}/${VC_LIB_PATH_SUFFIX}/SDL2_image.lib 
                                  ${SDL2_MIXER_DIR}/${VC_LIB_PATH_SUFFIX}/SDL2_mixer.lib
                                  ${SDL2_TTF_DIR}/${VC_LIB_PATH_SUFFIX}/SDL2_ttf.lib
)

MACRO(COPY_SDL_DLLS name)
      # SDL core
      add_custom_command(TARGET ${name} POST_BUILD  
          COMMAND ${CMAKE_COMMAND} -E copy_if_different  
              "${SDL2_DIR}/${VC_LIB_PATH_SUFFIX}/SDL2.dll"
              $<TARGET_FILE_DIR:${name}>)

      # SDL Image
      add_custom_command(TARGET ${name} POST_BUILD  
          COMMAND ${CMAKE_COMMAND} -E copy_directory  
              "${SDL2_IMAGE_DIR}/${VC_LIB_PATH_SUFFIX}"
              $<TARGET_FILE_DIR:${name}>)
      
      # SDL TTF
      add_custom_command(TARGET ${name} POST_BUILD   
          COMMAND ${CMAKE_COMMAND} -E copy_directory  
              "${SDL2_TTF_DIR}/${VC_LIB_PATH_SUFFIX}"
              $<TARGET_FILE_DIR:${name}>)

      # SDL_Mixer
      add_custom_command(TARGET ${name} POST_BUILD   
          COMMAND ${CMAKE_COMMAND} -E copy_directory  
              "${SDL2_MIXER_DIR}/${VC_LIB_PATH_SUFFIX}"
              $<TARGET_FILE_DIR:${name}>)
ENDMACRO()

#[ Tests ]

add_executable(app ${PROJECT_SOURCE_DIR}/test/test_app.cpp)
set_property(TARGET app PROPERTY CXX_STANDARD 17)
set_property(TARGET app PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(app PRIVATE ${MINOMALY})
COPY_SDL_DLLS(app)

