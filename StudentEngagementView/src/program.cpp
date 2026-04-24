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


  float square[] = {
    -1.f, -1.f, 0.f, 0.f,
    1.f, -1.f, 1.f, 0.f,
    -1.f, 1.f, 0.f, 1.f,
    1.f, 1.f, 1.f, 1.f,
  };

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);


  if (!Shader::GenShader("./shaders/objShader.vert", "./shaders/objShader.frag", objShader)) {
    PrintError("Failed to generate object shader");
  }
  
  return true;
}





bool Program::Update() {
/*  RenderTextInfo render = {
    .center = true,
    .scale = 1.f,
    .lineHeight = 1.f,
    .renderWidth = 2.0f,
    .framebufferWidth = width,
    .framebufferHeight = height,
    .position = {-1.0f, 1.0f},
    .color = {0,0,0,1}
  };*/

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT);

    //text_factory::RenderText(this, "ABBABABABAbaba\nA", render);

    if (activeScene) {
      activeScene->Render(this);
      glClearColor(activeScene->backgroundColor.x, activeScene->backgroundColor.y, activeScene->backgroundColor.z, activeScene->backgroundColor.w);
    }

    glfwSwapBuffers(window);
  }

  return true;
}





void Program::End() {
  glfwDestroyWindow(window);
  glfwTerminate();
}


}; // namespace ste




namespace {

void glfwWindowResized(GLFWwindow* window, int Width, int Height) {

}


}; // namespace