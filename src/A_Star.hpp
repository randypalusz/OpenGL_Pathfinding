// Copyright 2020 <randypalusz>

#ifndef A_Star_header
#define A_Star_header

#include <vector>

#include "Node.hpp"

class A_Star {
 public:
  // input to constructor will be the the desired search grid
  explicit A_Star(std::vector<std::vector<char>> &input);
  auto findShortest() -> std::vector<std::vector<char>>;
  void printGrid();

 private:
  // 0 = ground
  // 1 = wall
  // s = start
  // e = end
  std::vector<std::vector<char>> grid_;
  std::vector<Node *> openList_;
  std::vector<Node *> closeList_;
  Node *startNode_;
  Node *endNode_;
};

#endif