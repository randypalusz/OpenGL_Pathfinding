// Copyright 2020 <randypalusz>
// Using this guide for reference:
// https://github.com/JoeyDeVries/LearnOpenGL
// https://learnopengl.com/

// OpenGL includes must be in this order
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <chrono>
#include <vector>

#include "A_Star.hpp"

void glfunctions();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_END) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void glfunctions() {
  glfwInit();
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "Window", NULL, NULL);
  if (!window) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
  }
  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    glfwSwapBuffers(window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwPollEvents();
  }
  glfwTerminate();
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