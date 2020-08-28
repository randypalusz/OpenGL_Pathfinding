#include "CursesVisualize.hpp"

#include <ncurses.h>
#include "CharStruct.hpp"

#include <iostream>
#include <vector>

CursesVisualize::CursesVisualize(CharStruct chars, int height, int width) {
  charToPairMap.insert({{chars.start, START_PAIR},
                        {chars.end, END_PAIR},
                        {chars.wall, WALL_PAIR},
                        {chars.valid, VALID_PAIR},
                        {chars.open, OPEN_PAIR},
                        {chars.close, CLOSED_PAIR},
                        {chars.path, PATH_PAIR}});
  initWindow();
  drawBorder(height, width);
}

CursesVisualize::~CursesVisualize() { endwin(); }

void CursesVisualize::initWindow() {
  initscr();
  use_default_colors();
  initColors();
  curs_set(0);
  cbreak();
  noecho();
  timeout(0);
  keypad(stdscr, TRUE);
  erase();
}

void CursesVisualize::initColors() {
  start_color();
  init_pair(START_PAIR, COLOR_GREEN, COLOR_GREEN);
  init_pair(END_PAIR, COLOR_MAGENTA, COLOR_MAGENTA);
  init_pair(WALL_PAIR, COLOR_BLUE, COLOR_BLUE);
  init_pair(VALID_PAIR, INHERIT_COLOR, INHERIT_COLOR);
  init_pair(OPEN_PAIR, COLOR_YELLOW, COLOR_YELLOW);
  init_pair(CLOSED_PAIR, COLOR_RED, COLOR_RED);
  init_pair(PATH_PAIR, COLOR_WHITE, COLOR_WHITE);
}

void CursesVisualize::drawBorder(int height, int width) {
  // draw wall width of grid+1 at top
  std::string temp(width + 2, '#');
  char* topBottomWall = &(temp[0]);
  attron(COLOR_PAIR(WALL_PAIR));
  mvprintw(0, 0, "%s", topBottomWall);
  mvprintw(height + 1, 0, "%s", topBottomWall);
  for (int i = 0; i < height; i++) {
    mvprintw(i + 1, 0, "%c", '#');
    mvprintw(i + 1, width + 1, "%c", '#');
  }
  attroff(COLOR_PAIR(WALL_PAIR));
}

void CursesVisualize::update(std::vector<std::vector<char>>& grid) {
  char currentChar;
  int currentPair;
  int num_rows = grid.size();
  int num_columns = grid[0].size();
  for (int i = 0; i < num_rows; i++) {
    for (int j = 0; j < num_columns; j++) {
      currentChar = grid[i][j];
      currentPair = charToPairMap.at(currentChar);
      attron(COLOR_PAIR(currentPair));
      mvprintw(i + 1, j + 1, "%c", currentChar);
      attroff(COLOR_PAIR(currentPair));
    }
  }
  refresh();
}

void CursesVisualize::end(bool pathFound, int gridHeight) {
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