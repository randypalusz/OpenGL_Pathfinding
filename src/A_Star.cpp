#include "A_Star.hpp"

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
  startNode_->printParams();
  endNode_->printParams();
}

void A_Star::printGrid() {
  std::cout << "Current Grid:" << std::endl;
  for (auto row = grid_.begin(); row != grid_.end(); row++) {
    for (auto column = row->begin(); column != row->end(); column++) {
      std::cout << *column;
    }
    std::cout << std::endl;
  }
}

void A_Star::calculateShortest() {
  openList_.push_back(startNode_);
  while (openList_.size() > 0) {
    auto currentNode = openList_[0];
    int currentIndex = 0;
    for (int i = 0; i < openList_.size(); i++) {
      auto tempNode = openList_[i];
      if (tempNode->getF() < currentNode->getF()) {
        currentNode = tempNode;
        currentIndex = i;
      }
    }
    // pop Node with lowest F score off of openList_, add to closedList_
    openList_.erase(openList_.begin() + currentIndex);
    closeList_.push_back(currentNode);

    if (currentNode == endNode_) {
      // backtrack through parents, return grid updated with shortest path
      backtrack(currentNode);
      return;
    }

    // calculate coords of valid children
    // (valid == (not a wall) && (not out of bounds))
    auto children = getNeighbors(currentNode);

    // TODO: add f, g, h, parent updating
  }
  return;
}

void A_Star::backtrack(Node* currentNode) {
  do {
    auto row = currentNode->getPosition().first;
    auto column = currentNode->getPosition().second;
    grid_[row][column] = 'p';
    currentNode = currentNode->getParent();
  } while (currentNode != startNode_);
  return;
}

auto A_Star::getNeighbors(Node* currentNode) -> std::vector<std::pair<int, int>> {
  // check all neighbors of the currentNode
  // starting at the north, going clockwise to the north-east
  // <row, col> = (0,0) = currentNode position
  int centerRow = currentNode->getPosition().first;
  int centerColumn = currentNode->getPosition().second;
  std::vector<std::pair<int, int>> relativePositions;
  std::vector<std::pair<int, int>> validNeighbors;
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
    validNeighbors.push_back(std::make_pair(centerRow + itRow, centerColumn + itColumn));
  }
  return validNeighbors;
}