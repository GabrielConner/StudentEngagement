#ifndef STUDENT_ENGAGEMENT_SRC_PROGRAM_H
#define STUDENT_ENGAGEMENT_SRC_PROGRAM_H

#include "studentEngagement.h"
#include "shader.h"
#include "scene.h"

#include <memory>
#include <map>
#include <string>

namespace ste {

class Program {

GLFWwindow* window;

int width;
int height;

unsigned int VBO;
std::map<std::string, std::shared_ptr<Scene>> registeredScenes;
std::shared_ptr<Scene> activeScene;


public:

unsigned int VAO;
Shader objShader;


bool Start();
bool Update();
void End();

int ScreenWidth() const { return width; }
int ScreenHeight() const { return height; }


void RegisterScene(std::string name, std::shared_ptr<Scene>& scene) {
  registeredScenes[name] = scene;
}
bool ChangeScene(std::string name) {
  auto search = registeredScenes.find(name);
  if (search == registeredScenes.end()) {
    return false;
  }
  activeScene = search->second;
}

};

}; // namespace ste

#endif