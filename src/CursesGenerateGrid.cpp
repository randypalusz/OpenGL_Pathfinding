#include "CursesGenerateGrid.hpp"

#include <ncurses.h>

#include <iostream>
#include <string>

CursesGenerateGrid::CursesGenerateGrid() {
  std::cout << "Enter Grid Width: ";
  std::cin >> width_;
  std::cout << std::endl << "Enter Grid Height: ";
  std::cin >> height_;
  std::cout << std::endl;

  for (int i = 0; i < height_; i++) {
    grid_.push_back(std::vector<char>{});
    for (int j = 0; j < width_; j++) {
      grid_[i].push_back(' ');
    }
  }

  currentState_ = CreateShape;

  charToPairMap.insert({{'s', START_PAIR},
                        {'e', END_PAIR},
                        {'?', POTENTIAL_WALL_PAIR},
                        {'#', WALL_PAIR},
                        {' ', VALID_PAIR}});
}

void CursesGenerateGrid::initWindow() {
  initscr();
  use_default_colors();
  initColors();
  curs_set(0);
  cbreak();
  noecho();
  timeout(0);
  keypad(stdscr, TRUE);
}

void CursesGenerateGrid::initColors() {
  start_color();
  init_pair(START_PAIR, COLOR_GREEN, COLOR_GREEN);
  init_pair(END_PAIR, COLOR_MAGENTA, COLOR_MAGENTA);
  init_pair(POTENTIAL_WALL_PAIR, COLOR_BLACK, COLOR_CYAN);
  init_pair(WALL_PAIR, COLOR_BLUE, COLOR_BLUE);
  init_pair(VALID_PAIR, INHERIT_COLOR, INHERIT_COLOR);
}

auto CursesGenerateGrid::run() -> std::vector<std::vector<char>> {
  initWindow();
  while (currentState_ != Exit) {
    // clears the screen (any leftover potential walls/shapes)
    erase();
    update();
    switch (currentState_) {
      case PlaceStart:
        currentState_ = PlaceEnd;
        break;
      case PlaceEnd:
        currentState_ = CreateShape;
        break;
      case CreateShape:
        if (createShape()) {
          currentState_ = PlaceShape;
        } else {
          currentState_ = Exit;
        }
        break;
      case PlaceShape:
        currentState_ = CreateShape;
        break;
      case Exit:
        end();
        break;
      default:
        break;
    }
  }
  return grid_;
}

void CursesGenerateGrid::end() {
  erase();
  endwin();
}

auto CursesGenerateGrid::createShape() -> bool {
  int c;
  int shapeTopLeft = width_ + 10;
  int shapeWidth = 1;
  int shapeHeight = 1;
  int maxShapeWidth = 20;
  int maxShapeHeight = 20;
  bool finalize = false;
  bool updated = true;
  // update takes in width/height increment based on the key press
  // and increments the current shape width/height by the passed value
  auto update = [&](int widthIncrement, int heightIncrement) {
    if ((shapeWidth + widthIncrement > maxShapeWidth) ||
        (shapeWidth + widthIncrement < 1) || (shapeWidth + widthIncrement >= width_)) {
      return;
    }
    if ((shapeHeight + heightIncrement > maxShapeHeight) ||
        (shapeHeight + heightIncrement < 1) ||
        (shapeHeight + heightIncrement >= height_)) {
      return;
    }
    shapeWidth += widthIncrement;
    shapeHeight += heightIncrement;
    updated = true;
  };
  mvprintw(0, width_ + 3, "%s", "Shape: ");
  while (true) {
    c = getch();
    switch (c) {
      case KEY_RIGHT:
        update(1, 0);
        break;
      case KEY_LEFT:
        update(-1, 0);
        break;
      case KEY_UP:
        update(0, -1);
        break;
      case KEY_DOWN:
        update(0, 1);
        break;
      case KEY_ENTER:
        finalize = true;
        break;
      case 'x':
        return false;
      default:
        break;
    }

    if (updated) {
      char currentChar;
      for (int i = 0; i < maxShapeHeight; i++) {
        for (int j = 0; j < maxShapeWidth; j++) {
          if (j < shapeWidth && i < shapeHeight) {
            currentChar = '?';
          } else {
            currentChar = ' ';
          }
          attron(COLOR_PAIR(charToPairMap.at(currentChar)));
          mvprintw(i, shapeTopLeft + j, "%c", currentChar);
          attroff(COLOR_PAIR(charToPairMap.at(currentChar)));
        }
      }
      updated = false;
    }
    if (finalize) {
      return true;
    }
  }
}

void CursesGenerateGrid::update() {
  char currentChar;
  int currentPair;
  // draw wall width of grid+1 at top
  std::string temp(width_ + 2, '#');
  char* topBottomWall = &(temp[0]);
  attron(COLOR_PAIR(WALL_PAIR));
  mvprintw(0, 0, "%s", topBottomWall);
  attroff(COLOR_PAIR(WALL_PAIR));
  for (int i = 0; i < height_; i++) {
    attron(COLOR_PAIR(WALL_PAIR));
    mvprintw(i + 1, 0, "%c", '#');
    mvprintw(i + 1, width_ + 1, "%c", '#');
    attroff(COLOR_PAIR(WALL_PAIR));
    for (int j = 0; j < width_; j++) {
      currentChar = grid_[i][j];
      currentPair = charToPairMap.at(currentChar);
      attron(COLOR_PAIR(currentPair));
      mvprintw(i + 1, j + 1, "%c", currentChar);
      attroff(COLOR_PAIR(currentPair));
    }
  }
  attron(COLOR_PAIR(WALL_PAIR));
  mvprintw(height_ + 1, 0, "%s", topBottomWall);
  attroff(COLOR_PAIR(WALL_PAIR));
  refresh();
}