#include "program.h"
#include "textRendering.h"

namespace ste {


bool Program::Start() {
  // Start GLFW
  if (!glfwInit()) {
    PrintError("Failed to initialize GLFW");
    return false;
  }

  width = 720;
  height = 720;


  // Create window
  window = glfwCreateWindow(width, height, "Student Engagement", 0, 0);
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

  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  
  return true;
}





bool Program::Update() {
  glClearColor(0.2, 0.3, 0.3, 1.0);

  RenderTextInfo render = {
    .center = true,
    .scale = 1.f,
    .lineHeight = 1.f,
    .renderWidth = 2.0f,
    .framebufferWidth = width,
    .framebufferHeight = height,
    .position = {-1.0f, 1.0f},
    .color = {0,0,0,1}
  };

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT);

    text_factory::RenderText(this, "ABBABABABAbaba\nA", render);

    glfwSwapBuffers(window);
  }

  return true;
}





void Program::End() {
  glfwDestroyWindow(window);
  glfwTerminate();
}


}; // namespace ste