#ifndef CURSES_VISUALIZE_HPP
#define CURSES_VISUALIZE_HPP

#include <ncurses.h>

#include <unordered_map>
#include <vector>

const int INHERIT_COLOR = -1;
const int START_PAIR = 1;
const int END_PAIR = 2;
const int WALL_PAIR = 3;
const int VALID_PAIR = 4;
const int OPEN_PAIR = 5;
const int CLOSED_PAIR = 6;
const int PATH_PAIR = 7;

class CursesVisualize {
 public:
  explicit CursesVisualize(const char startChar, const char endChar,
                           const char wallChar, const char validChar,
                           const char openChar, const char closedChar,
                           const char pathChar);
  ~CursesVisualize();
  void update(std::vector<std::vector<char>> &grid);
  void end(bool pathFound, int gridHeight);

 private:
  void initWindow();
  void initColors();
  char startChar_;
  char endChar_;
  char wallChar_;
  char validChar_;
  char openChar_;
  char closedChar_;
  char pathChar_;
  std::unordered_map<char, int> charToPairMap;
};

#endif
