#include "shader.h"
#include "glad\glad.h"
#include "vector.h"
#include  "utility.h"

#include <filesystem>
#include <fstream>
#include <sstream>

using namespace std::string_literals;


// anonymous namespace
namespace {

unsigned int currentShader;

}; // namespace


namespace ste {

void Shader::Active() {
  glUseProgram(shader);
}




bool Shader::GenShader(std::string vert, std::string frag, Shader& out) {
  // Files need to exist
  if (!std::filesystem::exists(vert) || !std::filesystem::exists(frag)) {
    return false;
  }
  

  // Open files
  std::fstream vertFile(vert.c_str(), std::ios::in);
  std::fstream fragFile(vert.c_str(), std::ios::in);


  // Get buffer
  std::stringstream vertStream;
  std::stringstream fragStream;
  vertStream << vertFile.rdbuf();
  fragStream << fragFile.rdbuf();


  // Read as string
  std::string vertString = vertStream.str();
  std::string fragString = fragStream.str();


  // Get raw c string
  char const* vertC = vertString.c_str();
  char const* fragC = fragString.c_str();

  // Close
  vertFile.close();
  fragFile.close();


  // Create program and shaders
  unsigned int program = glCreateProgram();
  unsigned int vertShader = glCreateShader(GL_VERTEX_SHADER), fragShader = glCreateShader(GL_FRAGMENT_SHADER);

  int success = 0;
  char msg[256];


  // Vertex shader
  glShaderSource(vertShader, 1, &vertC, 0);
  glCompileShader(vertShader);
  glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    glGetShaderInfoLog(vertShader, 256, nullptr, msg);
    PrintError("[VERTEX]"s + msg);
  }


  // Fragment shader
  glShaderSource(fragShader, 1, &fragC, 0);
  glCompileShader(fragShader);
  glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    glGetShaderInfoLog(fragShader, 256, nullptr, msg);
    PrintError("[FRAGMENT]"s + msg);
  }



  // Attach then link shaders to program
  glAttachShader(program, vertShader);
  glAttachShader(program, fragShader);
  glLinkProgram(program);

  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (success == GL_FALSE) {
    glGetProgramInfoLog(program, 256, nullptr, msg);
    PrintError("[PROGRAM]"s + msg);
  }


  // Delete shaders, they are linked into program
  glDeleteShader(vertShader);
  glDeleteShader(fragShader);

  return true;
}



// Uniforms

void Shader::SetVector2(char const* loc, Vector2 const& val) {
  glUniform2f(glGetUniformLocation(currentShader, loc), val.x, val.y);
}
void Shader::SetVector3(char const* loc, Vector3 const& val) {
  glUniform3f(glGetUniformLocation(currentShader, loc), val.x, val.y, val.z);
}
void Shader::SetVector4(char const* loc, Vector4 const& val) {
  glUniform4f(glGetUniformLocation(currentShader, loc), val.x, val.y, val.z, val.w);
}

void Shader::SetFloat(char const* loc, float const& val) {
  glUniform1f(glGetUniformLocation(currentShader, loc), val);
}
void Shader::SetInt(char const* loc, int const& val) {
  glUniform1i(glGetUniformLocation(currentShader, loc), val);
}

void Shader::SetMat4(char const* loc, float const* val) {
  glUniform4fv(glGetUniformLocation(currentShader, loc), 1, val);
}

}; // namespace ste