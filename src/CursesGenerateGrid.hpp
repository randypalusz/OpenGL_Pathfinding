#ifndef CURSES_GENERATE_GRID_HPP
#define CURSES_GENERATE_GRID_HPP

#include <unordered_map>
#include <vector>

enum State { CreateShape, PlaceShape, PlaceStart, PlaceEnd, Exit };

class CursesGenerateGrid {
 public:
  CursesGenerateGrid();
  // main state machine for application
  // returns created grid
  auto run() -> std::vector<std::vector<char>>;

 private:
  void update();
  void initWindow();
  void initColors();
  // Returns false on exit condition
  auto createShape() -> bool;
  auto placeMarker(char) -> bool;
  void end();
  State currentState_;
  std::vector<std::vector<char>> grid_;
  int height_;
  int width_;
  std::unordered_map<char, int> charToPairMap;
  const int INHERIT_COLOR = -1;
  const int START_PAIR = 1;
  const int END_PAIR = 2;
  const int POTENTIAL_WALL_PAIR = 3;
  const int WALL_PAIR = 4;
  const int VALID_PAIR = 5;
};

#endif