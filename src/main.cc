// Copyright 2020 <randypalusz>

// #include <SFML/Graphics.hpp>
#include <chrono>
#include <vector>

#include "A_Star.hpp"
#include "CursesGenerateGrid.hpp"
#include "CursesVisualize.hpp"


auto createAStarFromGrid(bool hugWalls) -> A_Star {
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
  return path;
}

auto createAStarFromFile(bool hugWalls) -> A_Star {
  A_Star path{"grid.txt"};
  return path;
}

int main(int argc, char** argv) {
  // sfmlTest();
  // A_Star path{"grid.txt"};
  // auto path = createAStarFromGrid(true);
  CursesGenerateGrid a{};
  auto genResult = a.run();
  A_Star path{genResult.grid};
  // auto path = createAStarFromFile(false);
  // path.calculateShortest("console");
  path.calculateShortest(genResult.visualizationMethod);
}
