# from here: https://github.com/aaronmjacobs/InitGL

cmake_minimum_required(VERSION 3.10)

project(A_Star)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libraries)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(Sources
  ${SRC_DIR}/A_Star.cpp
  ${SRC_DIR}/main.cc
)

set(Headers
  ${SRC_DIR}/A_Star.hpp
  ${SRC_DIR}/Node.hpp
)

set(PROJECT_NAME ${PROJECT_NAME}.cc)

add_executable(${PROJECT_NAME} ${Sources})
target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_DIR})

# GLFW
set(GLFW_DIR "${LIB_DIR}/glfw")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory("${GLFW_DIR}")
target_link_libraries(${PROJECT_NAME} "glfw" "${GLFW_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLFW_DIR}/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")

# glad
set(GLAD_DIR ${LIB_DIR}/glad)
add_library("glad" ${GLAD_DIR}/src/glad.c)
target_include_directories("glad" PRIVATE ${GLAD_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${GLAD_DIR}/include)
target_link_libraries(${PROJECT_NAME} "glad" ${CMAKE_DL_LIBS})