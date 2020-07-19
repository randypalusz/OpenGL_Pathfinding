cmake_minimum_required(VERSION 3.10)

project(A_Star_Search)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# add src subdirectory above enable_testing() call
# doesn't make src libraries otherwise...
add_subdirectory(src)