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


class Program;
namespace text_factory {

  bool StartText(std::string font);
  bool RenderText(Program const* const prog, std::string text, bool center, float scale, float lineHeight, float width, int viewWidth, int viewHeight, Vector2 position);
  void EndText();

  RenderedCharacter& GetCharacter(const char& c);

}; // namespace text_factory


}; // namespace ste


#endif