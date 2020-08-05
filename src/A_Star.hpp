// Copyright 2020 <randypalusz>

#ifndef A_Star_header
#define A_Star_header

#include <chrono>
#include <unordered_set>
#include <vector>

#include "Node.hpp"

struct hash_pair {
  template <class T1, class T2>
  size_t operator()(const std::pair<T1, T2> &p) const {
    auto hash1 = std::hash<T1>{}(p.first);
    auto hash2 = std::hash<T2>{}(p.second);
    return hash1 ^ hash2;
  }
};

class A_Star {
 public:
  // input to constructor will be the the desired search grid
  explicit A_Star(std::vector<std::vector<char>> &input);
  explicit A_Star(std::string path);
  void calculateShortest();
  void printGrid();

 private:
  // 0 = ground
  // w = wall
  // s = start
  // e = end
  // p = shortest_path
  // updates shortest path on grid_
  void loadGridFromFile(std::string fileName);
  void loadGridFromVector(std::vector<std::vector<char>> &input);
  void backtrack(Node *);
  void getNeighbors(std::vector<Node *> &validNeighbors, Node *);
  void pushOnNeighborsList(Node *currentNode,
                           std::vector<std::pair<int, int>> &referencePositions,
                           std::vector<Node *> &validNeighbors,
                           std::vector<bool> &wallOrOOB);
  auto getDistance(Node *, Node *) -> double;
  void addToOpenList(Node *);
  std::vector<std::vector<char>> grid_;
  std::vector<Node *> openList_;
  std::unordered_set<std::pair<int, int>, hash_pair> closeList_;
  Node *startNode_;
  Node *endNode_;
  std::vector<std::pair<int, int>> primaryPositions_{
      std::make_pair(-1, 0),  // N
      std::make_pair(0, 1),   // E
      std::make_pair(1, 0),   // S
      std::make_pair(0, -1)   // W
  };
  char wall_ = '#';
  char valid_ = '.';
};

using time_point = std::chrono::_V2::system_clock::time_point;
void printSearchTime(time_point start, time_point end);

#endif