// Copyright 2020 <randypalusz>
// Using this guide for reference:
// https://github.com/JoeyDeVries/LearnOpenGL
// https://learnopengl.com/

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

int main(int argc, char** argv) {
  // std::vector<std::vector<char>> grid{};
  // grid.push_back(
  //     std::vector<char>{'.', '#', '.', '.', '.', '.', '.', '#', '.', '.', '.', '.'});
  // grid.push_back(
  //     std::vector<char>{'.', '.', '.', '#', '#', '.', '.', '#', '.', '.', '#', '.'});
  // grid.push_back(
  //     std::vector<char>{'.', '.', '#', '#', '.', '.', '.', '.', '.', '.', '#', 's'});
  // grid.push_back(
  //     std::vector<char>{'e', '#', '.', '#', '.', '.', '.', '#', '.', '.', '#', '.'});
  // grid.push_back(
  //     std::vector<char>{'.', '.', '.', '.', '.', '.', '#', '#', '.', '.', '.', '.'});
  // grid.push_back(
  //     std::vector<char>{'.', '#', '.', '.', '.', '.', '#', '.', '.', '.', '.', '.'});

  // A_Star path{grid};
  sfmlTest();
  A_Star path{"grid.txt"};
  path.printGrid();
  path.calculateShortest();
  path.printGrid();
}