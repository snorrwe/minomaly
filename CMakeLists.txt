cmake_minimum_required(VERSION 3.0)

set(MINOMALY minomaly)

project(${MINOMALY} VERSION 0.1.0 LANGUAGES CXX)

#[ Engine ]

add_library(${MINOMALY} STATIC
    ${PROJECT_SOURCE_DIR}/src/minomaly.cpp
    ${PROJECT_SOURCE_DIR}/src/system/window_system.cpp
    ${PROJECT_SOURCE_DIR}/src/system/sdl_subsystems.cpp
    ${PROJECT_SOURCE_DIR}/src/system/input_system.cpp
    ${PROJECT_SOURCE_DIR}/src/logger.cpp
    ${PROJECT_SOURCE_DIR}/src/system/render_system.cpp
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
target_compile_features(${MINOMALY} PRIVATE cxx_noexcept)

set(SILENCED_WARNINGS -Wno-pragmas
                      -Wno-unused-command-line-argument
                      -Wno-unknown-argument
                      -Wno-deprecated-declarations
                      -Wno-c++17-extensions)

set(MINO_COMPILE_OPTIONS "")
if(MSVC)
    if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang") # LLVM toolchain support
        set(MINO_COMPILE_OPTIONS /WX ${SILENCED_WARNINGS})
    else()
        set(MINO_COMPILE_OPTIONS /WX)
    endif()
else(MSVC)
    set(MINO_COMPILE_OPTIONS -Wall -Wextra -pedantic -Werror ${SILENCED_WARNINGS})
endif(MSVC)

target_compile_options(${MINOMALY} PRIVATE ${MINO_COMPILE_OPTIONS})

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

#[ Spdlog ]

include_directories(${PROJECT_SOURCE_DIR}/external/spdlog/include)

#[ Link external dependencies ]

target_link_libraries(${MINOMALY}
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_DIR}/${VC_LIB_PATH_SUFFIX}/SDL2_image.lib
    ${SDL2_MIXER_DIR}/${VC_LIB_PATH_SUFFIX}/SDL2_mixer.lib
    ${SDL2_TTF_DIR}/${VC_LIB_PATH_SUFFIX}/SDL2_ttf.lib
    )

#[ Tests ]

enable_testing()

#[ Gtest ]

find_package (Threads)
set(GTEST_ROOT_DIR  "${PROJECT_SOURCE_DIR}/external/googletest")
include_directories("${GTEST_ROOT_DIR}/googletest" "${GTEST_ROOT_DIR}/googletest/include")
add_library(gtest STATIC "${GTEST_ROOT_DIR}/googletest/src/gtest-all.cc"
                         "${GTEST_ROOT_DIR}/googletest/src/gtest_main.cc")
set_property(TARGET gtest PROPERTY CXX_STANDARD 11)
set_property(TARGET gtest PROPERTY CXX_STANDARD_REQUIRED ON)

#[ Test runner ]

add_executable(tests "${PROJECT_SOURCE_DIR}/test/test_manager.cpp"
                     "${PROJECT_SOURCE_DIR}/test/test_matrix.cpp"
    )
target_link_libraries(tests gtest ${MINOMALY} ${CMAKE_THREAD_LIBS_INIT})
set_property(TARGET tests PROPERTY CXX_STANDARD 17)
set_property(TARGET tests PROPERTY CXX_STANDARD_REQUIRED ON)

add_test(NAME run-all-tests COMMAND tests)

#[ Test app ]

add_executable(app "${PROJECT_SOURCE_DIR}/test/test_app.cpp")
set_property(TARGET app PROPERTY CXX_STANDARD 17)
set_property(TARGET app PROPERTY CXX_STANDARD_REQUIRED ON)
target_compile_options(app PRIVATE ${MINO_COMPILE_OPTIONS})
target_link_libraries(app PRIVATE ${MINOMALY})
COPY_SDL_DLLS(app)
# Copy the assets next to the executable and also the parent directory
add_custom_command(
    TARGET app
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/test/data
    ${PROJECT_SOURCE_DIR}/.data
    )
add_custom_command(
    TARGET app
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/test/data
    $<TARGET_FILE_DIR:app>/.data
    )

