// Copyright 2020 <randypalusz>
// Using this guide for reference:
// https://github.com/JoeyDeVries/LearnOpenGL
// https://learnopengl.com/

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <chrono>
#include <vector>

#include "A_Star.hpp"

void glfunctions();

void glfunctions() {
  bool running = true;
  glfwInit();
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "Window", NULL, NULL);
  if (!window) {
    glfwTerminate();
  }
  glfwMakeContextCurrent(window);
  while (running) {
    glfwPollEvents();
    if (glfwGetKey(window, GLFW_KEY_END) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
      running = false;
    }
  }
  glfwDestroyWindow(window);
}

int main(int argc, char** argv) {
  glfunctions();
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
  path.printGrid();
  path.calculateShortest();
  path.printGrid();
  glfwTerminate();
}