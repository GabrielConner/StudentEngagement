#ifndef STUDENT_ENGAGEMENT_SRC_PROGRAM_H
#define STUDENT_ENGAGEMENT_SRC_PROGRAM_H

#include "studentEngagement.h"
#include "shader.h"
#include "scene.h"
#include "input.h"

#include <memory>
#include <map>
#include <string>

namespace ste {
namespace program {

float MouseX();
float MouseY();
Vector2 MousePos();

int ScreenWidth();
int ScreenHeight();
float ScreenAspect();

Input GetInput(int button);

}; // namespace program


class Program {

GLFWwindow* window;

unsigned int VBO;
std::map<std::string, std::shared_ptr<Scene>> registeredScenes;
std::shared_ptr<Scene> activeScene;


public:

unsigned int VAO;
Shader objShader;


bool Start();
bool Update();
void End();




void RegisterScene(std::string name, std::shared_ptr<Scene>& scene) {
  registeredScenes[name] = scene;
}
bool ChangeScene(std::string name) {
  auto search = registeredScenes.find(name);
  if (search == registeredScenes.end()) {
    return false;
  }
  activeScene = search->second;
  if (activeScene->start) activeScene->start(this);
}

};

}; // namespace ste

#endif