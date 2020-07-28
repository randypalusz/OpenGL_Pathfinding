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
  auto getNeighbors(Node *) -> std::vector<Node *>;
  auto getDistance(Node *, Node *) -> double;
  void addToOpenList(Node *);
  std::vector<std::vector<char>> grid_;
  std::vector<Node *> openList_;
  std::unordered_set<std::pair<int, int>, hash_pair> closeList_;
  Node *startNode_;
  Node *endNode_;
  char wall_ = '#';
  char valid_ = '.';
};

using time_point = std::chrono::_V2::system_clock::time_point;
void printSearchTime(time_point start, time_point end);

#endif