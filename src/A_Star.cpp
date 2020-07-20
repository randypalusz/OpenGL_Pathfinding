#include "A_Star.hpp"

#include <math.h>

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

    if (currentNode->equalsNode(endNode_)) {
      // backtrack through parents, return grid updated with shortest path
      backtrack(currentNode);
      return;
    }

    // calculate coords of valid children
    // (valid == (not a wall) && (not out of bounds))
    auto children = getNeighbors(currentNode);

    for (int i = 0; i < children.size(); i++) {
      bool childInCloseList = false;
      Node* child = children[i];
      for (int j = 0; j < closeList_.size(); j++) {
        if (child->equalsNode(closeList_[j])) {
          childInCloseList = true;
          break;
        }
      }
      if (childInCloseList) {
        continue;
      }
      child->setG(currentNode->getG() + getDistance(currentNode, child));
      child->setH(getDistance(child, endNode_));
      child->setF(child->getG() + child->getH());

      // check if the current child exists in the closeList_ by position
      // if so, move on to the next child
      bool childInList = false;
      for (int j = 0; j < openList_.size(); j++) {
        Node* openNode = openList_[j];
        if (child->equalsNode(openNode)) {
          childInList = true;
          if (child->getG() < openNode->getG()) {
            openList_.erase(openList_.begin() + j);
            openList_.push_back(child);
          }
          break;
        }
      }
      if (!childInList) {
        openList_.push_back(child);
      }
    }
  }
  return;
}

void A_Star::backtrack(Node* currentNode) {
  currentNode = currentNode->getParent();
  do {
    auto row = currentNode->getPosition().first;
    auto column = currentNode->getPosition().second;
    grid_[row][column] = 'p';
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
