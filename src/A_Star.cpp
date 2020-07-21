#include "A_Star.hpp"

#include <math.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <utility>
#include <vector>

#include "Node.hpp"

A_Star::A_Star(std::vector<std::vector<char>>& inputGrid) {
  grid_ = inputGrid;
  bool startFound = false;
  bool endFound = false;
  for (int row = 0; row < grid_.size(); row++) {
    for (int column = 0; column < grid_[0].size(); column++) {
      if ((grid_[row][column] == 's') && (!startFound)) {
        startNode_ = new Node(nullptr, std::make_pair(row, column));
        startFound = true;
      }
      if ((grid_[row][column] == 'e') && (!endFound)) {
        endNode_ = new Node(nullptr, std::make_pair(row, column));
        endFound = true;
      }
    }
  }
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

// TODO: rewrite using this: https://en.wikipedia.org/wiki/A*_search_algorithm
void A_Star::calculateShortest() {
  auto start = std::chrono::high_resolution_clock::now();
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
      return;
    }

    // calculate coords of valid neighbors
    // (valid == (not a wall) && (not out of bounds))
    auto neighbors = getNeighbors(currentNode);

    for (int i = 0; i < neighbors.size(); i++) {
      Node* neighbor = neighbors[i];

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

auto A_Star::getNeighbors(Node* currentNode) -> std::vector<Node*> {
  // check all neighbors of the currentNode
  // starting at the north, going clockwise to the north-east
  // <row, col> = (0,0) = currentNode position
  int centerRow = currentNode->getPosition().first;
  int centerColumn = currentNode->getPosition().second;
  std::vector<std::pair<int, int>> relativePositions;
  std::vector<Node*> validNeighbors;
  relativePositions.push_back(std::make_pair(-1, 0));
  relativePositions.push_back(std::make_pair(-1, 1));
  relativePositions.push_back(std::make_pair(0, 1));
  relativePositions.push_back(std::make_pair(1, 1));
  relativePositions.push_back(std::make_pair(1, 0));
  relativePositions.push_back(std::make_pair(1, -1));
  relativePositions.push_back(std::make_pair(0, -1));
  relativePositions.push_back(std::make_pair(-1, -1));
  for (auto it = relativePositions.begin(); it != relativePositions.end(); it++) {
    int itRow = (*it).first;
    int itColumn = (*it).second;
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
    std::pair<int, int> position =
        std::make_pair(centerRow + itRow, centerColumn + itColumn);
    validNeighbors.push_back(new Node(currentNode, position));
  }
  return validNeighbors;
}

auto A_Star::getDistance(Node* one, Node* two) -> double {
  auto pos1 = one->getPosition();
  auto pos2 = two->getPosition();
  return std::sqrt(std::pow(pos2.second - pos1.second, 2) +
                   std::pow(pos2.first - pos1.first, 2));
}

void A_Star::addToOpenList(Node* node) {
  auto compare = [](Node* lhs, Node* rhs) { return lhs->getF() < rhs->getF(); };
  auto it = std::lower_bound(openList_.begin(), openList_.end(), node, compare);
  openList_.insert(it, node);
  return;
}

using time_point = std::chrono::_V2::system_clock::time_point;
void printSearchTime(time_point start, time_point end) {
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout << "Search Time: " << duration.count() << " microseconds" << std::endl;
}
