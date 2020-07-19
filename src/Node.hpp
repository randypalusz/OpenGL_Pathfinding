// Copyright 2020 <randypalusz>

#ifndef A_Star_Node
#define A_Star_Node

#include <iostream>
#include <utility>

class Node {
 public:
  explicit Node(Node* parent, std::pair<int, int> position) {
    parent_ = parent;
    position_ = position;
    g_ = 0.0;
    h_ = 0.0;
    f_ = 0.0;
  }
  auto getParent() -> Node* { return parent_; }
  auto getPosition() -> std::pair<int, int> { return position_; }
  auto getG() -> double { return g_; }
  auto getH() -> double { return h_; }
  auto getF() -> double { return f_; }
  void setParent(Node* parent) { parent_ = parent; }
  void setPosition(std::pair<int, int> position) { position_ = position; }
  void setG(double g) { g_ = g; }
  void setH(double h) { h_ = h; }
  void setF(double f) { f_ = f; }
  void printParams() {
    std::cout << "<Parent>, <Current>, <G, H, F>:\n";
    std::cout << "<" << parent_ << ">, <" << position_.first << ", " << position_.second << ">, <" << g_ << ", " << h_ << ", " << f_ << ">" << std::endl;
  }

 private:
  Node* parent_;
  std::pair<int, int> position_;
  double g_;
  double h_;
  double f_;
};

#endif