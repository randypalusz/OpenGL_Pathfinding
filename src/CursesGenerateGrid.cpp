#include "CursesGenerateGrid.hpp"

#if _WIN32 
#include <curses.h>
#else
#include <ncurses.h>
#endif

#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>

CursesGenerateGrid::CursesGenerateGrid() {
  int tempWidth;
  int tempHeight;
  std::cout << "Enter Grid Width: ";
  std::cin >> tempWidth;
  std::cout << std::endl << "Enter Grid Height: ";
  std::cin >> tempHeight;
  std::cout << std::endl;
  width_ = std::clamp(tempWidth, minGridWidth_, maxGridWidth_);
  height_ = std::clamp(tempHeight, minGridHeight_, maxGridHeight_);
  // if (tempWidth > maxGridWidth_) {
  //   tempWidth = maxGridWidth_;
  // }
  // if (tempHeight > maxGridHeight_) {
  //   tempHeight = maxGridHeight_;
  // }
  // width_ = tempWidth;
  // height_ = tempHeight;

  for (int i = 0; i < height_; i++) {
    grid_.push_back(std::vector<char>{});
    for (int j = 0; j < width_; j++) {
      grid_[i].push_back(' ');
    }
  }

  currentState_ = PlaceStart;

  charToPairMap.insert({{'s', START_PAIR},
                        {'e', END_PAIR},
                        {'?', POTENTIAL_WALL_PAIR},
                        {'#', WALL_PAIR},
                        {' ', VALID_PAIR},
                        {'x', DELETE_PAIR}});
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
  init_pair(START_PAIR, COLOR_BLACK, COLOR_GREEN);
  init_pair(END_PAIR, COLOR_BLACK, COLOR_MAGENTA);
  init_pair(POTENTIAL_WALL_PAIR, COLOR_BLACK, COLOR_CYAN);
  init_pair(WALL_PAIR, COLOR_BLUE, COLOR_BLUE);
  init_pair(VALID_PAIR, INHERIT_COLOR, INHERIT_COLOR);
  init_pair(HELP_PAIR, COLOR_BLACK, COLOR_YELLOW);
  init_pair(DELETE_PAIR, COLOR_BLACK, COLOR_RED);
  init_pair(SELECT_PAIR, COLOR_BLACK, COLOR_GREEN);
  init_pair(UNSELECT_PAIR, INHERIT_COLOR, INHERIT_COLOR);
}

auto CursesGenerateGrid::run() -> GenReturnStruct {
  PlaceShapeParams placeData;
  initWindow();
  while (currentState_ != Exit) {
    // clears the screen (any leftover potential walls/shapes)
    erase();
    update();
    switch (currentState_) {
      case PlaceStart:
        if (placeMarker('s')) {
          currentState_ = PlaceEnd;
        }
        break;
      case PlaceEnd:
        if (placeMarker('e')) {
          currentState_ = CreateShape;
        }
        break;
      case CreateShape:
        placeData = createShape();
        if (placeData.placeShape) {
          currentState_ = PlaceShape;
        } else {
          currentState_ = ChooseVisualization;
        }
        break;
      case PlaceShape:
        placeShape(placeData);
        currentState_ = CreateShape;
        break;
      case ChooseVisualization:
        chooseVisualization();
        currentState_ = Exit;
        break;
      case Exit:
        end();
        break;
      default:
        break;
    }
  }
  end();
  return GenReturnStruct{grid_, visualizationMethod_};
}

void CursesGenerateGrid::end() {
  erase();
  endwin();
}

auto CursesGenerateGrid::placeMarker(char marker) -> bool {
  if ((marker != 's') && (marker != 'e')) {
    // if not s/e, bad marker
    return false;
  }
  int keyIn;
  // row/column is the position in the grid_
  int row = 0;
  int column = 0;
  std::string markerString;
  int markerColorPair;
  bool finalize = false;
  bool updated = true;
  auto update = [&](int widthIncrement, int heightIncrement) {
    if ((column + widthIncrement >= width_) || (column + widthIncrement < 0)) {
      return;
    }
    if ((row + heightIncrement >= height_) || (row + heightIncrement < 0)) {
      return;
    }
    row += heightIncrement;
    column += widthIncrement;
    move(row + gridTopLeftRow_, column + gridTopLeftColumn_);
    refresh();
  };

  auto placeChar = [&]() {
    if (grid_[row][column] == ' ') {
      grid_[row][column] = marker;
      finalize = true;
    }
  };

  switch (marker) {
    case ('s'):
      markerString = "Placing Start...";
      markerColorPair = START_PAIR;
      break;
    case ('e'):
      markerString = "Placing End...";
      markerColorPair = END_PAIR;
      break;
    default:
      markerString = "Undefined";
      break;
  }

  attron(COLOR_PAIR(markerColorPair));
  mvprintw(LINES - 8, 0, "%s", &(markerString[0]));
  attroff(COLOR_PAIR(markerColorPair));
  mvprintw(LINES - 1, 0, "%s", "Arrow/Vim Keys to Move");
  mvprintw(LINES - 2, 0, "%s", "Enter to Place");
  attron(COLOR_PAIR(HELP_PAIR));
  mvprintw(LINES - 3, 0, "%s", "==========HELP==========");
  attroff(COLOR_PAIR(HELP_PAIR));

  curs_set(1);
  move(gridTopLeftRow_, gridTopLeftColumn_);
  refresh();

  while (!finalize) {
    keyIn = getch();
    switch (keyIn) {
      case KEY_RIGHT:
      case 'l':
        update(1, 0);
        break;
      case KEY_LEFT:
      case 'h':
        update(-1, 0);
        break;
      case KEY_UP:
      case 'k':
        update(0, -1);
        break;
      case KEY_DOWN:
      case 'j':
        update(0, 1);
        break;
      case '\n':
        // enter pressed, attempt to place marker at row/column
        placeChar();
        break;
      default:
        break;
    }
  }

  return true;
}

auto CursesGenerateGrid::createShape() -> PlaceShapeParams {
  int c;
  int shapeTopLeft = width_ + 10;
  int shapeWidth = 1;
  int shapeHeight = 1;
  int maxShapeWidth = 30;
  int maxShapeHeight = 30;
  bool finalize = false;
  bool updated = true;
  bool deleteWalls = false;
  int currentPair;
  char placeChar = '?';
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

  curs_set(0);
  mvprintw(0, width_ + 3, "%s", "Shape: ");
  mvprintw(LINES - 1, 0, "%s", "Arrow/Vim Keys - Modify Shape");
  mvprintw(LINES - 2, 0, "%s", "Enter to Place Shape");
  mvprintw(LINES - 3, 0, "%s", "SPACE to Finalize Grid");
  mvprintw(LINES - 4, 0, "%s", "'d' to Toggle 'Delete' Mode");
  attron(COLOR_PAIR(HELP_PAIR));
  mvprintw(LINES - 5, 0, "%s", "==========HELP==========");
  attroff(COLOR_PAIR(HELP_PAIR));

  while (!finalize) {
    c = getch();
    switch (c) {
      case KEY_RIGHT:
      case 'l':
        update(1, 0);
        break;
      case KEY_LEFT:
      case 'h':
        update(-1, 0);
        break;
      case KEY_UP:
      case 'k':
        update(0, -1);
        break;
      case KEY_DOWN:
      case 'j':
        update(0, 1);
        break;
      case 'd':
        placeChar = (placeChar == '?') ? 'x' : '?';
        deleteWalls = !deleteWalls;
        updated = true;
        break;
      case '\n':
        finalize = true;
        break;
      case ' ':
        return PlaceShapeParams{false, deleteWalls, -1, -1};
      default:
        break;
    }

    if (updated) {
      char currentChar;
      for (int i = 0; i < maxShapeHeight; i++) {
        for (int j = 0; j < maxShapeWidth; j++) {
          if (j < shapeWidth && i < shapeHeight) {
            currentChar = placeChar;
          } else {
            currentChar = ' ';
          }
          attron(COLOR_PAIR(charToPairMap.at(currentChar)));
          mvprintw(i, shapeTopLeft + j, "%c", currentChar);
          attroff(COLOR_PAIR(charToPairMap.at(currentChar)));
        }
      }

      currentPair = (deleteWalls) ? DELETE_PAIR : POTENTIAL_WALL_PAIR;

      attron(COLOR_PAIR(currentPair));
      mvprintw(LINES - 8, 0, "%s", "CREATING SHAPE");
      attroff(COLOR_PAIR(currentPair));

      updated = false;
    }
  }
  return PlaceShapeParams{true, deleteWalls, shapeWidth, shapeHeight};
}

void CursesGenerateGrid::placeShape(const PlaceShapeParams& placeData) {
  int shapeWidth = placeData.shapeWidth;
  int shapeHeight = placeData.shapeHeight;
  bool deleteWalls = placeData.deleteWalls;
  char placeChar = placeData.deleteWalls ? 'x' : '?';
  int c;
  // keep track of position by topLeft corner
  int topLeftRow = 0;
  int topLeftColumn = 0;
  bool updated = false;
  bool finalize = false;

  // lambda to update the position of the walls to be placed
  auto updatePosition = [&](int widthIncrement, int heightIncrement) {
    // return if shape would not be visible at all on screen
    // at minimum, a single corner of the wall should be shown
    if ((topLeftColumn + shapeWidth + widthIncrement < gridTopLeftColumn_) ||
        (topLeftColumn + widthIncrement >= (width_ - 1) + gridTopLeftColumn_) ||
        (topLeftRow + shapeHeight + heightIncrement < gridTopLeftRow_) ||
        (topLeftRow + heightIncrement >= (height_ - 1) + gridTopLeftRow_)) {
      return;
    }

    topLeftColumn += widthIncrement;
    topLeftRow += heightIncrement;
    updated = true;
  };

  // lambda to show a preview of the walls to be placed
  auto preview = [&]() {
    int startRow = std::max(0, topLeftRow);
    int startColumn = std::max(0, topLeftColumn);
    int endRow = std::min(height_, topLeftRow + shapeHeight);
    int endColumn = std::min(width_, topLeftColumn + shapeWidth);
    int currentPair = deleteWalls ? DELETE_PAIR : POTENTIAL_WALL_PAIR;
    const char* placeString = deleteWalls ? "DELETING WALLS" : "PLACING WALLS";
    // update to restore the grid to its actual state
    update();
    // draw preview walls over the grid
    for (int i = startRow; i < endRow; i++) {
      for (int j = startColumn; j < endColumn; j++) {
        if ((grid_[i][j] == 's') || (grid_[i][j] == 'e')) {
          continue;
        }
        attron(COLOR_PAIR(charToPairMap.at(placeChar)));
        mvprintw(i + gridTopLeftRow_, j + gridTopLeftColumn_, "%c", placeChar);
        attroff(COLOR_PAIR(charToPairMap.at(placeChar)));
      }
    }
    // quick clear of the previous string
    attron(COLOR_PAIR(UNSELECT_PAIR));
    mvprintw(LINES - 8, 0, "%s", "               ");
    attroff(COLOR_PAIR(UNSELECT_PAIR));

    // update the delete/place string
    attron(COLOR_PAIR(currentPair));
    mvprintw(LINES - 8, 0, "%s", placeString);
    attroff(COLOR_PAIR(currentPair));
    updated = false;
  };

  // lambda to update the grid_[][] with the walls
  auto place = [&]() {
    for (int i = topLeftRow; i < topLeftRow + shapeHeight; i++) {
      for (int j = topLeftColumn; j < topLeftColumn + shapeWidth; j++) {
        if ((i < 0) || (i >= height_) || (j < 0) || (j >= width_)) {
          continue;
        }
        if ((grid_[i][j] == 's') || (grid_[i][j] == 'e')) {
          continue;
        }
        if (deleteWalls) {
          grid_[i][j] = ' ';
        } else {
          grid_[i][j] = '#';
        }
      }
    }
  };

  mvprintw(LINES - 1, 0, "%s", "Arrow/Vim Keys to Move Preview");
  mvprintw(LINES - 2, 0, "%s", "Enter to Place/Delete Shape");
  mvprintw(LINES - 3, 0, "%s", "BACKSPACE/SPACE to go to create screen");
  mvprintw(LINES - 4, 0, "%s", "'d' to Toggle 'Delete' Mode");
  attron(COLOR_PAIR(HELP_PAIR));
  mvprintw(LINES - 5, 0, "%s", "==========HELP==========");
  attroff(COLOR_PAIR(HELP_PAIR));

  curs_set(0);
  preview();

  while (!finalize) {
    c = getch();
    switch (c) {
      case KEY_RIGHT:
      case 'l':
        updatePosition(1, 0);
        break;
      case KEY_LEFT:
      case 'h':
        updatePosition(-1, 0);
        break;
      case KEY_UP:
      case 'k':
        updatePosition(0, -1);
        break;
      case KEY_DOWN:
      case 'j':
        updatePosition(0, 1);
        break;
      case 'd':
        placeChar = (placeChar == 'x') ? '?' : 'x';
        deleteWalls = !deleteWalls;
        updated = true;
        break;
      case '\n':
        place();
        break;
      // cancel placement
      case '\b':
      case 127:
      case KEY_BACKSPACE:
      case ' ':
        finalize = true;
        break;
      default:
        break;
    }
    if (updated) {
      preview();
    }
  }
}

void CursesGenerateGrid::chooseVisualization() {
  int c;
  int selected = false;
  int selectedRow = 1;
  int numMethods = 2;
  std::unordered_map<int, const char*> rowToString{{1, "NCurses Animation"},
                                                   {2, "Console (Performance)"}};
  std::unordered_map<int, std::string> rowToReturn{{1, "curses"}, {2, "console"}};

  auto select = [&](int rowChange) {
    int newRow = selectedRow + rowChange;
    if ((newRow < 1) || (newRow > numMethods)) {
      return;
    }
    for (int i = 1; i <= numMethods; i++) {
      if (i == newRow) {
        attron(COLOR_PAIR(SELECT_PAIR));
        mvprintw(i, 0, "%s", rowToString.at(i));
        attroff(COLOR_PAIR(SELECT_PAIR));
      } else {
        attron(COLOR_PAIR(UNSELECT_PAIR));
        mvprintw(i, 0, "%s", rowToString.at(i));
        attroff(COLOR_PAIR(UNSELECT_PAIR));
      }
    }
    selectedRow = newRow;
    refresh();
  };

  clear();
  attron(COLOR_PAIR(HELP_PAIR));
  mvprintw(0, 0, "%s", "=====Choose a Visuzliation Method=====");
  attroff(COLOR_PAIR(HELP_PAIR));
  select(0);

  while (!selected) {
    c = getch();
    switch (c) {
      case KEY_UP:
      case 'k':
        select(-1);
        break;
      case KEY_DOWN:
      case 'j':
        select(1);
        break;
      case '\n':
        selected = true;
        break;
      default:
        break;
    }
  }

  visualizationMethod_ = rowToReturn.at(selectedRow);
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