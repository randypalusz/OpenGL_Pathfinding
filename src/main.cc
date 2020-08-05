// Copyright 2020 <randypalusz>
// Using this guide for reference:
// https://github.com/JoeyDeVries/LearnOpenGL
// https://learnopengl.com/

#include <chrono>
#include <vector>

#include "A_Star.hpp"

int main(int argc, char** argv) {
  // std::vector<std::vector<char>> grid{};
  // grid.push_back(
  //     std::vector<char>{'.', '#', '.', '.', '.', '.', '.', '#', '.', '.', '.', '.'});
  // grid.push_back(
  //     std::vector<char>{'.', '.', '.', '#', '#', '.', '.', '#', '.', '.', '#', '.'});
  // grid.push_back(
  //     std::vector<char>{'.', '.', '#', '#', '.', '.', '.', '.', '.', '.', '#', 's'});
  // grid.push_back(
  //     std::vector<char>{'e', '#', '.', '#', '.', '.', '.', '#', '.', '.', '#', '.'});
  // grid.push_back(
  //     std::vector<char>{'.', '.', '.', '.', '.', '.', '#', '#', '.', '.', '.', '.'});
  // grid.push_back(
  //     std::vector<char>{'.', '#', '.', '.', '.', '.', '#', '.', '.', '.', '.', '.'});

  // A_Star path{grid};
  A_Star path{"grid.txt"};
  path.printGrid();
  path.calculateShortest();
  path.printGrid();
}