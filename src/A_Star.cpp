#include "A_Star.hpp"

#include <iostream>
#include <utility>
#include <vector>

#include "Node.hpp"

A_Star::A_Star(std::vector<std::vector<char>> &inputGrid) {
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

auto A_Star::findShortest() -> std::vector<std::vector<char>> {
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
      return grid_;
    }
  }
  return grid_;
}