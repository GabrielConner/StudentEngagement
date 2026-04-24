#ifndef STUDENT_ENGAGEMENT_SRC_TEXT_RENDERING_H
#define STUDENT_ENGAGEMENT_SRC_TEXT_RENDERING_H

#include "vector.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <map>


namespace ste {

struct RenderedCharacter {
  unsigned int texture;

  IVector2 size;
  IVector2 bearing;
  int advance;

  RenderedCharacter() : texture(0), size(0), bearing(0), advance(0) {}
};

struct RenderTextInfo {
  bool center;
  bool vertCenter;
  float scale;
  float lineHeight;
  float renderWidth;
  int framebufferWidth;
  int framebufferHeight;
  Vector2 position;
  Vector4 color;
};


class Program;
namespace text_factory {

  bool StartText(std::string font);
  bool RenderText(Program const* const prog, std::string text, const RenderTextInfo& info);
  const RenderedCharacter& GetCharacter(const char& c);

  void EndText();


}; // namespace text_factory


}; // namespace ste


#endif