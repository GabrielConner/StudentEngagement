#define GLFW_INCLUDE_NONE

#include "glfw/glfw3.h"
#include "glad/glad.h"

#include "utility.h"

using namespace ::ste;


int main() {
  // Start GLFW
  if (!glfwInit()) {
    PrintError("Failed to initialize GLFW");
    return 1;
  }


  // Create window
  GLFWwindow* window = glfwCreateWindow(1280, 720, "Student Engagement", 0, 0);
  if (window == nullptr) {
    PrintError("Failed to create GLFW window");
    return 1;
  }

  // Let OpenGL know of window
  glfwMakeContextCurrent(window);


  // Load OpenGL
  if (!gladLoadGL()) {
    PrintError("Failed to load OpenGL");
    return 1;
  }


  glClearColor(0.2, 0.3, 0.3, 1.0);
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
  }


  return 0;
}