// Copyright 2020 <randypalusz>
// Using this guide for reference:
// https://medium.com/@nicholas.w.swift/easy-a-star-pathfinding-7e6689c7f7b2

#include <chrono>
#include <vector>

#include "A_Star.hpp"

int main() {
  std::vector<std::vector<char>> grid{};
  grid.push_back(
      std::vector<char>{'.', '#', '.', '.', '.', '.', '.', '#', '.', '.', '.', '.'});
  grid.push_back(
      std::vector<char>{'.', '.', '.', '#', '#', '.', '.', '#', '.', '.', '#', '.'});
  grid.push_back(
      std::vector<char>{'.', '.', '#', '#', '.', '.', '.', '.', '.', '.', '#', 's'});
  grid.push_back(
      std::vector<char>{'e', '#', '.', '#', '.', '.', '.', '#', '.', '.', '#', '.'});
  grid.push_back(
      std::vector<char>{'.', '.', '.', '.', '.', '.', '#', '#', '.', '.', '.', '.'});
  grid.push_back(
      std::vector<char>{'.', '#', '.', '.', '.', '.', '#', '.', '.', '.', '.', '.'});

  A_Star path{grid};
  path.printGrid();
  path.calculateShortest();
  path.printGrid();
}