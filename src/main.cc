// Copyright 2020 <randypalusz>

#include <SFML/Graphics.hpp>

#include <chrono>
#include <vector>

#include "A_Star.hpp"

void sfmlTest() {
  sf::RenderWindow window(sf::VideoMode(200, 200), "SFML Window");
  sf::CircleShape shape(100.f);
  shape.setFillColor(sf::Color::Green);
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }
    window.clear();
    window.draw(shape);
    window.display();
  }
}

auto createAStarFromGrid() -> A_Star {
  std::vector<std::vector<char>> grid{};
  grid.push_back(
      std::vector<char>{'.', '#', '.', '.', '.', '.', '.', '#', '.', '.', '.', '.'});
  grid.push_back(
      std::vector<char>{'.', '.', '.', '#', '#', '.', '.', '#', '.', '.', '#', '.'});
  grid.push_back(
      std::vector<char>{'.', '.', '#', '#', '.', '.', '.', '.', '.', '.', '#', 's'});
  grid.push_back(
      std::vector<char>{'e', '#', '.', '#', '.', '.', '.', '#', '.', '.', '#', '.'});
  grid.push_back(
      std::vector<char>{'.', '.', '.', '.', '.', '.', '#', '#', '.', '.', '.', '.'});
  grid.push_back(
      std::vector<char>{'.', '#', '.', '.', '.', '.', '#', '.', '.', '.', '.', '.'});

  A_Star path{grid};
  return path;
}

auto createAStarFromFile() -> A_Star {
  A_Star path{"grid.txt"};
  return path;
}

int main(int argc, char** argv) {
  sfmlTest();
  // A_Star path{"grid.txt"};
  // auto path = createAStarFromGrid();
  auto path = createAStarFromFile();
  path.printGrid();
  path.calculateShortest();
  path.printGrid();
}