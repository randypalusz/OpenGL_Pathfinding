#include "A_Star.hpp"

#include <math.h>

#include <algorithm>
#include <chrono>
#if _WIN32 
#include <experimental/filesystem>
#else
#include <filesystem>
#endif
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_set>
#include <utility>
#include <vector>

#include "CharStruct.hpp"
#include "CursesVisualize.hpp"
#include "Node.hpp"

A_Star::A_Star(std::vector<std::vector<char>>& inputGrid, bool hugWalls) {
  hugWalls_ = hugWalls;
  loadGridFromVector(inputGrid);
}

A_Star::A_Star(std::string fileName, bool hugWalls) {
  hugWalls_ = hugWalls;
  loadGridFromFile(fileName);
}

void A_Star::printGrid() {
  std::cout << "Current Grid:" << std::endl;
  for (auto row = grid_.begin(); row != grid_.end(); row++) {
    for (auto column = row->begin(); column != row->end(); column++) {
      std::cout << *column;
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void A_Star::calculateShortest(std::string visualizationMethod) {
  if (visualizationMethod == "console") {
    calculateShortestPerf();
  } else if (visualizationMethod == "curses") {
    calculateShortestNcurses();
  } else {
    calculateShortestPerf();
  }
}

void A_Star::calculateShortestPerf() {
  auto start = std::chrono::high_resolution_clock::now();
  std::vector<Node*> validNeighbors;
  addToOpenList(startNode_);
  while (openList_.size() > 0) {
    auto currentNode = openList_[0];
    openList_.erase(openList_.begin());
    // closeList_.push_back(currentNode);
    closeList_.insert(currentNode->getPosition());

    if (currentNode->equalsNode(endNode_)) {
      // backtrack through parents, return grid updated with shortest path
      printSearchTime(start, std::chrono::high_resolution_clock::now());
      std::cout << "Path Found!" << std::endl << std::endl;
      backtrack(currentNode);
      printGrid();
      return;
    }

    // calculate coords of valid neighbors
    // (valid == (not a wall) && (not out of bounds))
    getNeighbors(validNeighbors, currentNode);

    for (int i = 0; i < validNeighbors.size(); i++) {
      Node* neighbor = validNeighbors[i];

      // continue if child in the visited list
      if (closeList_.find(neighbor->getPosition()) != closeList_.end()) {
        continue;
      }

      neighbor->setG(currentNode->getG() + getDistance(currentNode, neighbor));
      neighbor->setH(getDistance(neighbor, endNode_));
      neighbor->setF(neighbor->getG() + neighbor->getH());

      // At this point, we know the current neighbor hasn't been visited_.
      // Check if the current neighbor exists in the openList_ by position -
      // if so, move on to the next neighbor
      bool neighborInList = false;
      for (int j = 0; j < openList_.size(); j++) {
        Node* openNode = openList_[j];
        // if neighbor (by position) is in the openList_...
        if (neighbor->equalsNode(openNode)) {
          // if the neighbor g_score is less than the openNode's g_score...
          // overwrite that node in the openList_
          if (neighbor->getG() < openNode->getG()) {
            openList_.erase(openList_.begin() + j);
            addToOpenList(neighbor);
          }
          neighborInList = true;
          break;
        }
      }

      if (!neighborInList) {
        addToOpenList(neighbor);
      }
    }
  }
  printSearchTime(start, std::chrono::high_resolution_clock::now());
  std::cout << "Path not found..." << std::endl << std::endl;
  return;
}

void A_Star::calculateShortestNcurses() {
  CharStruct charStruct{start_, end_, wall_, valid_, open_, close_, path_};
  CursesVisualize w{charStruct, height_, width_};

  std::vector<Node*> validNeighbors;
  addToOpenList(startNode_);
  while (openList_.size() > 0) {
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    w.update(grid_);
    auto currentNode = openList_[0];
    eraseFromOpenList(openList_.begin());
    addToCloseList(currentNode);

    if (currentNode->equalsNode(endNode_)) {
      // backtrack through parents, return grid updated with shortest path
      backtrackNcurses(currentNode);
      w.update(grid_);
      w.end(true, height_);
      return;
    }

    // calculate coords of valid neighbors
    // (valid == (not a wall) && (not out of bounds))
    getNeighbors(validNeighbors, currentNode);

    for (int i = 0; i < validNeighbors.size(); i++) {
      Node* neighbor = validNeighbors[i];

      // continue if child in the visited list
      if (closeList_.find(neighbor->getPosition()) != closeList_.end()) {
        continue;
      }

      neighbor->setG(currentNode->getG() + getDistance(currentNode, neighbor));
      neighbor->setH(getDistance(neighbor, endNode_));
      neighbor->setF(neighbor->getG() + neighbor->getH());

      // At this point, we know the current neighbor hasn't been visited_.
      // Check if the current neighbor exists in the openList_ by position -
      // if so, move on to the next neighbor
      bool neighborInList = false;
      for (int j = 0; j < openList_.size(); j++) {
        Node* openNode = openList_[j];
        // if neighbor (by position) is in the openList_...
        if (neighbor->equalsNode(openNode)) {
          // if the neighbor g_score is less than the openNode's g_score...
          // overwrite that node in the openList_
          if (neighbor->getG() < openNode->getG()) {
            eraseFromOpenList(openList_.begin() + j);
            addToOpenList(neighbor, true);
          }
          neighborInList = true;
          break;
        }
      }

      if (!neighborInList) {
        addToOpenList(neighbor, true);
      }
    }
  }
  w.end(false, height_);
  return;
}

void A_Star::loadGridFromFile(const std::string fileName) {
  std::vector<std::vector<char>> v;
  #if _WIN32
  auto path = std::experimental::filesystem::current_path();
  #else
  auto path = std::filesystem::current_path();
  #endif
  path = path.append("resources");
  path = path.append(fileName);
  // std::cout << "File: " << path << std::endl;
  #if _WIN32
  std::ifstream is(path.string());
  #else
  std::ifstream is(path);
  #endif
  char c;
  int row = 0;
  v.push_back(std::vector<char>{});
  while (is.get(c)) {
    // std::cout << "Character: " << c << std::endl;
    if (c == '\n') {
      row++;
      v.push_back(std::vector<char>{});
      continue;
    }
    v[row].push_back(c);
  }
  loadGridFromVector(v);
}

void A_Star::loadGridFromVector(const std::vector<std::vector<char>>& inputGrid) {
  grid_ = inputGrid;
  char currentChar;
  int startFound = 0;
  int endFound = 0;
  for (int row = 0; row < grid_.size(); row++) {
    for (int column = 0; column < grid_[0].size(); column++) {
      currentChar = grid_[row][column];
      if (currentChar == start_) {
        startNode_ = new Node(nullptr, std::make_pair(row, column));
        startFound++;
      } else if (currentChar == end_) {
        endNode_ = new Node(nullptr, std::make_pair(row, column));
        endFound++;
      }
    }
  }
  if ((startFound != 1) || (endFound != 1)) {
    std::cout << "Invalid number of start/end points..." << std::endl;
    exit(1);
  }
  openList_.reserve(grid_.size() * grid_[0].size());
  height_ = grid_.size();
  width_ = grid_[0].size();
}

void A_Star::backtrack(Node* currentNode) {
  currentNode = currentNode->getParent();
  do {
    auto row = currentNode->getPosition().first;
    auto column = currentNode->getPosition().second;
    grid_[row][column] = 'o';
    currentNode = currentNode->getParent();
  } while (currentNode != startNode_);
  return;
}

void A_Star::backtrackNcurses(Node* currentNode) {
  currentNode = currentNode->getParent();
  do {
    auto row = currentNode->getPosition().first;
    auto column = currentNode->getPosition().second;
    grid_[row][column] = path_;
    currentNode = currentNode->getParent();
  } while (currentNode != startNode_);
  return;
}

// TODO: wallOrOOB not always used - find a way to make this optional while
// maintaining performance
auto A_Star::pushOnNeighborsList(Node* currentNode,
                                 const std::vector<std::pair<int, int>>& refPositions,
                                 std::vector<Node*>& validNeighbors)
    -> const std::vector<bool> {
  // wallOrOutOfBounds             N     E     S     W
  std::vector<bool> wallOrOOB = {true, true, true, true};
  auto centerRow = currentNode->getPosition().first;
  auto centerColumn = currentNode->getPosition().second;
  for (int i = 0; i < refPositions.size(); i++) {
    auto currentPos = refPositions[i];
    int itRow = currentPos.first;
    int itColumn = currentPos.second;
    int addedRelPos = itRow + itColumn;

    bool outOfBounds = ((centerRow + itRow) < 0) || ((centerColumn + itColumn) < 0) ||
                       ((centerRow + itRow) >= grid_.size()) ||
                       ((centerColumn + itColumn) >= grid_[0].size());
    if (outOfBounds) {
      continue;
    }

    bool isWall = (grid_[centerRow + itRow][centerColumn + itColumn] == wall_);
    if (isWall) {
      continue;
    }
    wallOrOOB[i] = false;
    std::pair<int, int> position =
        std::make_pair(centerRow + itRow, centerColumn + itColumn);
    validNeighbors.push_back(new Node(currentNode, position));
  }
  return wallOrOOB;
}

void A_Star::getNeighbors(std::vector<Node*>& validNeighbors, Node* currentNode) {
  validNeighbors.clear();
  std::vector<std::pair<int, int>> secondaryPositions;
  auto wallOrOOB = pushOnNeighborsList(currentNode, primaryPositions_, validNeighbors);

  if (hugWalls_) {
    if (!wallOrOOB[0] || !wallOrOOB[1])
      secondaryPositions.push_back(std::make_pair(-1, 1));  // NE

    if (!wallOrOOB[1] || !wallOrOOB[2])
      secondaryPositions.push_back(std::make_pair(1, 1));  // SE

    if (!wallOrOOB[2] || !wallOrOOB[3])
      secondaryPositions.push_back(std::make_pair(1, -1));  // SW

    if (!wallOrOOB[3] || !wallOrOOB[0])
      secondaryPositions.push_back(std::make_pair(-1, -1));  // NW
  } else {
    if (!wallOrOOB[0] && !wallOrOOB[1])
      secondaryPositions.push_back(std::make_pair(-1, 1));  // NE

    if (!wallOrOOB[1] && !wallOrOOB[2])
      secondaryPositions.push_back(std::make_pair(1, 1));  // SE

    if (!wallOrOOB[2] && !wallOrOOB[3])
      secondaryPositions.push_back(std::make_pair(1, -1));  // SW

    if (!wallOrOOB[3] && !wallOrOOB[0])
      secondaryPositions.push_back(std::make_pair(-1, -1));  // NW
  }

  pushOnNeighborsList(currentNode, secondaryPositions, validNeighbors);
}

auto A_Star::getDistance(Node* one, Node* two) const -> double {
  auto pos1 = one->getPosition();
  auto pos2 = two->getPosition();
  return std::sqrt(std::pow(pos2.second - pos1.second, 2) +
                   std::pow(pos2.first - pos1.first, 2));
}

void A_Star::addToOpenList(Node* node, bool ncurses) {
  auto compare = [](Node* lhs, Node* rhs) { return lhs->getF() < rhs->getF(); };
  auto it = std::lower_bound(openList_.begin(), openList_.end(), node, compare);
  openList_.insert(it, node);
  if (ncurses) {
    auto position = node->getPosition();
    if (node->equalsNode(startNode_)) {
      return;
    }
    if (node->equalsNode(endNode_)) {
      return;
    }
    grid_[position.first][position.second] = open_;
  }
  return;
}

void A_Star::eraseFromOpenList(std::vector<Node*>::iterator it) {
  openList_.erase(it);
  auto pos = (*it)->getPosition();
  if ((pos.first == startNode_->getPosition().first) &&
      (pos.second == startNode_->getPosition().second)) {
    return;
  }
  grid_[pos.first][pos.second] = valid_;
}

void A_Star::addToCloseList(Node* node) {
  closeList_.insert(node->getPosition());
  auto position = node->getPosition();
  if (node->equalsNode(startNode_)) {
    return;
  }
  if (node->equalsNode(endNode_)) {
    return;
  }
  grid_[position.first][position.second] = close_;
}

using time_point = std::chrono::high_resolution_clock::time_point;
void printSearchTime(time_point start, time_point end) {
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << "Search Time: " << duration.count() << " microseconds" << std::endl;
}

// void threadWrapper(CursesWindow& w, std::vector<std::vector<char>>& grid) {
//   while (true) {
//     w.update(grid);
//     std::this_thread::sleep_for(std::chrono::milliseconds(100));
//   }
// }
