#include "program.h"


namespace ste {


bool Program::Start() {
  // Start GLFW
  if (!glfwInit()) {
    PrintError("Failed to initialize GLFW");
    return false;
  }


  // Create window
  window = glfwCreateWindow(1280, 720, "Student Engagement", 0, 0);
  if (window == nullptr) {
    PrintError("Failed to create GLFW window");
    return false;
  }

  // Let OpenGL know of window
  glfwMakeContextCurrent(window);


  // Load OpenGL
  if (!gladLoadGL()) {
    PrintError("Failed to load OpenGL");
    return false;
  }
  
  return true;
}





bool Program::Update() {
  glClearColor(0.2, 0.3, 0.3, 1.0);
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
  }

  return true;
}





void Program::End() {
  glfwDestroyWindow(window);
  glfwTerminate();
}


}; // namespace ste