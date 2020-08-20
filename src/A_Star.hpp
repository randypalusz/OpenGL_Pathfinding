// Copyright 2020 <randypalusz>

#ifndef A_Star_header
#define A_Star_header

#include <chrono>
#include <unordered_set>
#include <vector>

#include "CursesVisualize.hpp"
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
  explicit A_Star(std::vector<std::vector<char>> &input, bool hugWalls = false);
  explicit A_Star(std::string path, bool hugWalls = false);
  void calculateShortest(std::string visualizationMethod);
  void printGrid();

 private:
  void calculateShortestPerf();
  void calculateShortestNcurses();
  void loadGridFromFile(const std::string fileName);
  void loadGridFromVector(const std::vector<std::vector<char>> &input);
  void backtrack(Node *);
  void backtrackNcurses(Node *);
  void getNeighbors(std::vector<Node *> &validNeighbors, Node *currentNode);
  auto pushOnNeighborsList(
      Node *currentNode,
      const std::vector<std::pair<int, int>> &referencePositions,
      std::vector<Node *> &validNeighbors) -> const std::vector<bool>;
  auto getDistance(Node *, Node *) const -> double;
  void addToOpenList(Node *, bool ncurses = false);
  void eraseFromOpenList(std::vector<Node *>::iterator it);
  void addToCloseList(Node *);
  std::vector<std::vector<char>> grid_;
  std::vector<Node *> openList_;
  std::unordered_set<std::pair<int, int>, hash_pair> closeList_;
  Node *startNode_;
  Node *endNode_;
  bool hugWalls_;
  const std::vector<std::pair<int, int>> primaryPositions_{
      std::make_pair(-1, 0),  // N
      std::make_pair(0, 1),   // E
      std::make_pair(1, 0),   // S
      std::make_pair(0, -1)   // W
  };
  char wall_ = '#';
  char valid_ = ' ';
  const char start_ = 's';
  const char end_ = 'e';
  char open_ = 'o';
  char close_ = 'c';
  char path_ = 'p';
};

using time_point = std::chrono::high_resolution_clock::time_point;
void printSearchTime(time_point start, time_point end);

#endif