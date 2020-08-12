#include "Curses_Window.hpp"

#include <ncurses.h>
#include <iostream>
#include <vector>

CursesWindow::CursesWindow(const char startChar, const char endChar, const char wallChar,
                           const char validChar, const char openChar,
                           const char closedChar) {
  startChar_ = startChar;
  endChar_ = endChar;
  wallChar_ = wallChar;
  validChar_ = validChar;
  openChar_ = openChar;
  closedChar_ = closedChar;
  charToPairMap.insert({startChar, START_PAIR});
  charToPairMap.insert({endChar, END_PAIR});
  charToPairMap.insert({wallChar, WALL_PAIR});
  charToPairMap.insert({validChar, VALID_PAIR});
  charToPairMap.insert({openChar, OPEN_PAIR});
  charToPairMap.insert({closedChar, CLOSED_PAIR});
  initWindow();
}

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
  init_pair(START_PAIR, COLOR_GREEN, INHERIT_COLOR);
  init_pair(END_PAIR, COLOR_YELLOW, INHERIT_COLOR);
  init_pair(WALL_PAIR, COLOR_BLUE, COLOR_BLUE);
  init_pair(VALID_PAIR, INHERIT_COLOR, INHERIT_COLOR);
  init_pair(OPEN_PAIR, COLOR_YELLOW, COLOR_YELLOW);
  init_pair(CLOSED_PAIR, COLOR_RED, COLOR_RED);
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
