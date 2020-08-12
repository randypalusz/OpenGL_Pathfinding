#ifndef CURSES_WINDOW
#define CURSES_WINDOW

#include <ncurses.h>
#include <vector>
#include <unordered_map>

const int INHERIT_COLOR = -1;
const int START_PAIR = 1;
const int END_PAIR = 2;
const int WALL_PAIR = 3;
const int VALID_PAIR = 4;
const int OPEN_PAIR = 5;
const int CLOSED_PAIR = 6;

class CursesWindow {
 public:
  explicit CursesWindow(const char startChar, const char endChar, const char wallChar,
                        const char validChar, const char openChar, const char closedChar);
  void update(std::vector<std::vector<char>> &grid);

 private:
  void initWindow();
  void initColors();
  char startChar_;
  char endChar_;
  char wallChar_;
  char validChar_;
  char openChar_;
  char closedChar_;
  std::unordered_map<char, int> charToPairMap;
};

#endif
