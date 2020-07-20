// Copyright 2020 <randypalusz>

#ifndef A_Star_header
#define A_Star_header

#include <vector>

#include "Node.hpp"

class A_Star {
 public:
  // input to constructor will be the the desired search grid
  explicit A_Star(std::vector<std::vector<char>> &input);
  void calculateShortest();
  void printGrid();

 private:
  // 0 = ground
  // w = wall
  // s = start
  // e = end
  // p = shortest_path
  // updates shortest path on grid_
  void backtrack(Node *);
  auto getNeighbors(Node *) -> std::vector<Node *>;
  std::vector<std::vector<char>> grid_;
  std::vector<Node *> openList_;
  std::vector<Node *> closeList_;
  Node *startNode_;
  Node *endNode_;
  char wall_ = 'w';
  char valid_ = '0';
};

#endif