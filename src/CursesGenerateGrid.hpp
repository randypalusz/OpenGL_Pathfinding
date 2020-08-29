#ifndef CURSES_GENERATE_GRID_HPP
#define CURSES_GENERATE_GRID_HPP

#include <unordered_map>
#include <vector>

#include "GenReturnStruct.hpp"

class CursesGenerateGrid {
  enum State {
    CreateShape,
    PlaceShape,
    PlaceStart,
    PlaceEnd,
    ChooseVisualization,
    Exit
  };

  // placeShape set to false means to ignore the other two parameters, go to the
  // Exit state
  struct ShapeReturn {
    bool placeShape;
    int shapeWidth;
    int shapeHeight;
  };

 public:
  CursesGenerateGrid();
  // main state machine for application
  // returns created grid
  auto run() -> GenReturnStruct;

 private:
  void update();
  void initWindow();
  void initColors();
  auto createShape() -> ShapeReturn;
  // TODO: implement placeShape
  void placeShape(int shapeWidth, int shapeHeight);
  auto placeMarker(char) -> bool;
  void end();
  State currentState_;
  std::vector<std::vector<char>> grid_;
  int height_;
  int width_;
  const int gridTopLeftRow_ = 1;
  const int gridTopLeftColumn_ = 1;
  std::unordered_map<char, int> charToPairMap;
  const int INHERIT_COLOR = -1;
  const int START_PAIR = 1;
  const int END_PAIR = 2;
  const int POTENTIAL_WALL_PAIR = 3;
  const int WALL_PAIR = 4;
  const int VALID_PAIR = 5;
  const int HELP_PAIR = 6;
};

#endif