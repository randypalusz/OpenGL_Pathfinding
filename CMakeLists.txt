# from here: https://github.com/aaronmjacobs/InitGL

cmake_minimum_required(VERSION 3.17)

project(A_Star)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")


#set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libraries)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(Sources
  ${SRC_DIR}/A_Star.cpp
  ${SRC_DIR}/main.cc
  ${SRC_DIR}/CursesVisualize.cpp
  ${SRC_DIR}/CursesGenerateGrid.cpp
)

set(Headers
  ${SRC_DIR}/A_Star.hpp
  ${SRC_DIR}/Node.hpp
  ${SRC_DIR}/CursesVisualize.hpp
  ${SRC_DIR}/CursesGenerateGrid.hpp
  ${SRC_DIR}/CharStruct.hpp
  ${SRC_DIR}/GenReturnStruct.hpp
)

set(PROJECT_NAME ${PROJECT_NAME}.cc)

add_executable(${PROJECT_NAME} ${Sources})

if(APPLE)
  message("No linking stdc++fs on MAC OS X")
endif()

if(NOT APPLE)
  message("Linking stdc++fs on Linux")
  target_link_libraries(${PROJECT_NAME} stdc++fs)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_DIR})

# NCURSES
find_package(Curses REQUIRED)
set(CURSES_USE_NCURSES TRUE)
include_directories(${CURSES_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})

# SFML
# Install SFML via package manager
find_package(SFML 2.5 COMPONENTS audio graphics window system REQUIRED)
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system sfml-audio)

## GLFW
## Add glfw as a submodule in libraries/glfw
#set(GLFW_DIR "${LIB_DIR}/glfw")
#set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
#set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
#set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
#set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
#add_subdirectory("${GLFW_DIR}")
#target_link_libraries(${PROJECT_NAME} "glfw" "${GLFW_LIBRARIES}")
#target_include_directories(${PROJECT_NAME} PRIVATE "${GLFW_DIR}/include")
#target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")

## glad
## https://github.com/aaronmjacobs/InitGL
#set(GLAD_DIR ${LIB_DIR}/glad)
#add_library("glad" ${GLAD_DIR}/src/glad.c)
#target_include_directories("glad" PRIVATE ${GLAD_DIR}/include)
#target_include_directories(${PROJECT_NAME} PRIVATE ${GLAD_DIR}/include)
#target_link_libraries(${PROJECT_NAME} "glad" ${CMAKE_DL_LIBS})

##// OpenGL includes must be in this order
##include <glad/glad.h>
##include <GLFW/glfw3.h>
