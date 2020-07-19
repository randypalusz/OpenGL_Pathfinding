cmake_minimum_required(VERSION 3.10) 

project(A_Star_Search)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(Sources
  A_Star.cpp 
)

set(Headers
  A_Star.hpp
  Node.hpp
)

add_library(A_Star_Search STATIC ${Sources} ${Headers})

add_executable(main.cc ${Sources} main.cc)
# target_link_libraries(game.cc Snake ${CURSES_LIBRARIES})