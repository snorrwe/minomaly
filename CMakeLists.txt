cmake_minimum_required(VERSION 3.0)

set(MINOMALY minomaly)

project(${MINOMALY} VERSION 0.1.0 LANGUAGES CXX)

#[ library ]

add_library(${MINOMALY} ${PROJECT_SOURCE_DIR}/src/minomaly.cpp
                        ${PROJECT_SOURCE_DIR}/src/entity.cpp
    )
include_directories(${PROJECT_SOURCE_DIR}/include)
set_property(TARGET ${MINOMALY} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${MINOMALY} PROPERTY CXX_STANDARD_REQUIRED ON)

if(MSVC)
    target_compile_options(${MINOMALY} PRIVATE /W4 /WX)
else(MSVC)
    target_compile_options(${MINOMALY} PRIVATE -Wall -Wextra -pedantic -Werror)
endif(MSVC)

#[ tests ]

add_executable(app ${PROJECT_SOURCE_DIR}/test/test_app.cpp)
set_property(TARGET app PROPERTY CXX_STANDARD 17)
set_property(TARGET app PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(app ${MINOMALY})

