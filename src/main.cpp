#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Game.h"

int main(int argc, char** argv)
{
  int samples = 0;
  if (argc > 1) {
    samples = atoi(argv[1]);
  }

  glfwInit();
  glfwSetTime(0);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, samples);
  glfwWindowHint(GLFW_RED_BITS, 8);
  glfwWindowHint(GLFW_GREEN_BITS, 8);
  glfwWindowHint(GLFW_BLUE_BITS, 8);
  glfwWindowHint(GLFW_ALPHA_BITS, 8);
  glfwWindowHint(GLFW_STENCIL_BITS, 8);
  glfwWindowHint(GLFW_DEPTH_BITS, 24);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "main", nullptr, nullptr);
  if (window == nullptr) {
    std::cerr << "Failed to create GLFW window." << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cerr << "Could not initialize GLAD" << std::endl;
    return -1;
  }
  glGetError(); // pull and ignore unhandled errors like GL_INVALID_ENUM

  glfwSwapInterval(1);
  glfwSwapBuffers(window);
  
  if (samples > 0) {
    glEnable(GL_MULTISAMPLE);
  }

  gp::Game* game = new gp::Game(window);
  game->run();
  delete game;

  // Terminate GLFW, clearing any resources allocated by GLFW.
  glfwTerminate();

	return 0;
}
