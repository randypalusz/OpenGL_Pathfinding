#ifndef CURSES_VISUALIZE_HPP
#define CURSES_VISUALIZE_HPP

#include <ncurses.h>

#include <unordered_map>
#include <vector>

#include "CharStruct.hpp"

class CursesVisualize {
 public:
  explicit CursesVisualize(CharStruct, int height, int width);
  ~CursesVisualize();
  void update(std::vector<std::vector<char>> &grid);
  void end(bool pathFound, int gridHeight);

 private:
  void initWindow();
  void initColors();
  void drawBorder(int height, int width);
  CharStruct chars;
  std::unordered_map<char, int> charToPairMap;
  const int gridTopLeftRow_ = 1;
  const int gridTopLeftColumn_ = 1;
  const int INHERIT_COLOR = -1;
  const int START_PAIR = 1;
  const int END_PAIR = 2;
  const int WALL_PAIR = 3;
  const int VALID_PAIR = 4;
  const int OPEN_PAIR = 5;
  const int CLOSED_PAIR = 6;
  const int PATH_PAIR = 7;
};

#endif
