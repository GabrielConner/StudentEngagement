#include "program.h"
#include "textRendering.h"
#include "input.h"
#include "client.h"


namespace {
void glfwWindowResizedCallback(GLFWwindow* window, int Width, int Height);
void glfwMouseMoveCallback(GLFWwindow* window, double posX, double posY);
void glfwKeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void glfwMouseCallback(GLFWwindow* window, int mouse, int action, int mods);



int width = 1280;
int height = 720;
float aspect = 1280.f / 720.f;

::ste::Vector2 mousePos = 0;

std::map<int, ::ste::Input> inputMap;

}; // namespace


namespace ste {
namespace program {

float MouseX() { return mousePos.x; }
float MouseY() { return mousePos.y; }
Vector2 MousePos() { return mousePos; }

int ScreenWidth() { return width; }
int ScreenHeight() { return width; }
float ScreenAspect() { return aspect; }

::ste::Input GetInput(int button) { return inputMap[button]; }

}; // namespace program


bool Program::Start() {
  // Start GLFW
  if (!glfwInit()) {
    PrintError("Failed to initialize GLFW");
    return false;
  }


  // Create window
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  window = glfwCreateWindow(width, height, "Student Engagement", 0, 0);
  if (window == nullptr) {
    PrintError("Failed to create GLFW window");
    return false;
  }

  // Let OpenGL know of window
  glfwMakeContextCurrent(window);


  // Input callbacks
  glfwSetCursorPosCallback(window, glfwMouseMoveCallback);
  glfwSetMouseButtonCallback(window, glfwMouseCallback);
  glfwSetKeyCallback(window, glfwKeyboardCallback);
  glfwSetWindowSizeCallback(window, glfwWindowResizedCallback);


  // Load OpenGL
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    PrintError("Failed to load OpenGL");
    return false;
  }

  // Transparency
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


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
  

  if (!client::Start()) {
    PrintError("Failed to start client socket");
  }

  return true;
}





bool Program::Update() {

  Object* prevMouseIn = nullptr;

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT);

    glfwGetKeyScancode(GLFW_KEY_X);
    if (activeScene) {
      if (activeScene->update) activeScene->update(this);

      activeScene->Render(this);
      glClearColor(activeScene->backgroundColor.x, activeScene->backgroundColor.y, activeScene->backgroundColor.z, activeScene->backgroundColor.w);

      Object* mouseIn = dynamic_cast<Object*>(activeScene->PositionIn(mousePos));
      if (mouseIn != nullptr) {
        if (prevMouseIn != nullptr) {
          if (mouseIn != prevMouseIn) {
            prevMouseIn->MouseEvent(this, MouseEvent(MouseEvent::LEAVE, mousePos));
            mouseIn->MouseEvent(this, MouseEvent(MouseEvent::ENTER, mousePos));
            prevMouseIn = mouseIn;
          }
        } else {
          prevMouseIn = mouseIn;
          mouseIn->MouseEvent(this, MouseEvent(MouseEvent::ENTER, mousePos));
        }

        if (program::GetInput(GLFW_MOUSE_BUTTON_1).pressed) {
          mouseIn->MouseEvent(this, MouseEvent(MouseEvent::CLICK_PRESS, mousePos));
        }
        if (program::GetInput(GLFW_MOUSE_BUTTON_1).released) {
          mouseIn->MouseEvent(this, MouseEvent(MouseEvent::CLICK_RELEASE, mousePos));
        }
      } else if (prevMouseIn != nullptr) {
        prevMouseIn->MouseEvent(this, MouseEvent(MouseEvent::LEAVE, mousePos));
        prevMouseIn = nullptr;
      }
    }


    for (auto& pair : inputMap) {
      pair.second.pressed = false;
      pair.second.released = false;
    }

    glfwSwapBuffers(window);
  }

  return true;
}





void Program::End() {
  glfwDestroyWindow(window);
  glfwTerminate();

  registeredScenes.clear();
  client::End();
}


}; // namespace ste




namespace {

void glfwWindowResizedCallback(GLFWwindow* window, int Width, int Height) {
  width = Width;
  height = Height;

  aspect = float(width) / height;

  glViewport(0, 0, Width, Height);
}

void glfwMouseMoveCallback(GLFWwindow* window, double posX, double posY) {
  mousePos.x = aspect * (2 * posX / width - 1);
  mousePos.y = -(2 * posY / height - 1);
}


void glfwKeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  ::ste::Input& input = inputMap[scancode];
  if (action == GLFW_PRESS) {
    input.pressed = true;
    input.held = true;
    input.released = false;
  } else if (action == GLFW_RELEASE) {
    input.pressed = false;
    input.held = false;
    input.released = true;
  }
}


void glfwMouseCallback(GLFWwindow* window, int mouse, int action, int mods) {
  ::ste::Input& input = inputMap[mouse];
  if (action == GLFW_PRESS) {
    input.pressed = true;
    input.held = true;
    input.released = false;
  } else if (action == GLFW_RELEASE) {
    input.pressed = false;
    input.held = false;
    input.released = true;
  }
}

}; // namespace