#ifndef STUDENT_ENGAGEMENT_SRC_SHADER_H
#define STUDENT_ENGAGEMENT_SRC_SHADER_H

#include "vector.h"
#include <string>

namespace ste {

class Shader {
  unsigned int shader;
  public:

  void Active();


  static bool GenShader(std::string vert, std::string frag, Shader& out);

  static void SetVector2(char const* loc, Vector2 const& val);
  static void SetVector3(char const* loc, Vector3 const& val);
  static void SetVector4(char const* loc, Vector4 const& val);

  static void SetFloat(char const* loc, float const& val);
  static void SetInt(char const* loc, int const& val);

  static void SetMat4(char const* loc, float const* val);


  operator unsigned int() {
    return shader;
  }


  Shader() : shader(0) {}
  Shader(unsigned int Shader) : shader(Shader) {}
};

}; // namespace ste

#endif