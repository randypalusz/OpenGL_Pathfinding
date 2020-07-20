// Copyright 2020 <randypalusz>
// Using this guide for reference:
// https://medium.com/@nicholas.w.swift/easy-a-star-pathfinding-7e6689c7f7b2

#include <vector>

#include "A_Star.hpp"

int main() {
  std::vector<std::vector<char>> grid{};
  grid.push_back(std::vector<char>{'0', 'w', '0', '0', 'e', '0'});
  grid.push_back(std::vector<char>{'0', 'w', 'w', '0', '0', '0'});
  grid.push_back(std::vector<char>{'s', '0', 'w', '0', '0', '0'});
  grid.push_back(std::vector<char>{'0', '0', 'w', '0', '0', '0'});
  grid.push_back(std::vector<char>{'0', 'w', 'w', 'w', '0', '0'});
  grid.push_back(std::vector<char>{'0', '0', '0', '0', '0', '0'});

  A_Star path{grid};
  path.printGrid();
  path.calculateShortest();
  path.printGrid();
}