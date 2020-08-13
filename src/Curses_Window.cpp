#include "Curses_Window.hpp"

#include <ncurses.h>
#include <iostream>
#include <vector>

CursesWindow::CursesWindow(const char startChar, const char endChar, const char wallChar,
                           const char validChar, const char openChar,
                           const char closedChar, const char pathChar) {
  startChar_ = startChar;
  endChar_ = endChar;
  wallChar_ = wallChar;
  validChar_ = validChar;
  openChar_ = openChar;
  closedChar_ = closedChar;
  pathChar_ = pathChar;
  charToPairMap.insert({{startChar, START_PAIR},
                        {endChar, END_PAIR},
                        {wallChar, WALL_PAIR},
                        {validChar, VALID_PAIR},
                        {openChar, OPEN_PAIR},
                        {closedChar, CLOSED_PAIR},
                        {pathChar, PATH_PAIR}});
  initWindow();
}

CursesWindow::~CursesWindow() { endwin(); }

void CursesWindow::initWindow() {
  initscr();
  use_default_colors();
  initColors();
  curs_set(0);
  cbreak();
  timeout(0);
  keypad(stdscr, TRUE);
}

void CursesWindow::initColors() {
  start_color();
  init_pair(START_PAIR, COLOR_GREEN, COLOR_GREEN);
  init_pair(END_PAIR, COLOR_YELLOW, COLOR_YELLOW);
  init_pair(WALL_PAIR, COLOR_BLUE, COLOR_BLUE);
  init_pair(VALID_PAIR, INHERIT_COLOR, INHERIT_COLOR);
  init_pair(OPEN_PAIR, COLOR_YELLOW, COLOR_YELLOW);
  init_pair(CLOSED_PAIR, COLOR_RED, COLOR_RED);
  init_pair(PATH_PAIR, COLOR_WHITE, COLOR_WHITE);
}

void CursesWindow::update(std::vector<std::vector<char>>& grid) {
  char currentChar;
  int currentPair;
  int num_rows = grid.size();
  int num_columns = grid[0].size();
  for (int i = 0; i < num_rows; i++) {
    for (int j = 0; j < num_columns; j++) {
      currentChar = grid[i][j];
      currentPair = charToPairMap.at(currentChar);
      attron(COLOR_PAIR(currentPair));
      mvprintw(i, j, "%c", currentChar);
      attroff(COLOR_PAIR(currentPair));
    }
  }
  refresh();
}

void CursesWindow::end(bool pathFound, int gridHeight) {
  if (pathFound) {
    mvprintw(gridHeight + 3, 0, "%s", "Path Found!");
  } else {
    mvprintw(gridHeight + 3, 0, "%s", "Path Not Found... :(");
  }
  mvprintw(gridHeight + 4, 0, "%s", "Press 'x' to exit...");
  while (true) {
    int c = getch();
    if (c == 'x') {
      break;
    }
  }
}