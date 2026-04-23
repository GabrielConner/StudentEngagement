#ifndef STUDENT_ENGAGEMENT_SRC_TEXT_RENDERING_H
#define STUDENT_ENGAGEMENT_SRC_TEXT_RENDERING_H

#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H


namespace ste {

class TextRenderingFactory {

FT_Library ftLib;
FT_Face ftFace;



public:

  bool StartText(std::string font);
  bool RenderText();
  void EndText();
};


}; // namespace ste


#endif