#include "textRendering.h"
#include "utility.h"
#include "constants.h"
#include "program.h"
#include "shader.h"

#include "glm/common.hpp"
#include "glm/ext.hpp"
#include "glad/glad.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <stdexcept>


using namespace std::string_literals;



// anonymous namespace
namespace {

FT_Library ftLib;
FT_Face ftFace;

std::map<char, ::ste::RenderedCharacter> characterMap;

unsigned int FBO, VBO, VAO;

::ste::Shader textShader;

}; // namespace




namespace ste {
namespace text_factory {


bool StartText(std::string font) {
  EndText();

  // Initialization
  FT_Error err = FT_Init_FreeType(&ftLib);
  if (err) {
    PrintError("Failed to initialize FreeType");
    return false;
  }


  // Load font
  err = FT_New_Face(ftLib, font.c_str(), 0, &ftFace);
  if (err) {
    PrintError("Failed to generate font face");
    return false;
  }


  // Set rendered 'size', not really size but used for scaling
  err = FT_Set_Char_Size(ftFace, 0, _FONT_LOAD_SIZE << 6, 96, 96);
  if (err) {
    PrintError("Failed to set char size in font face");
    return false;
  }


  // Remove all characters from map
  characterMap.clear();

  return true;
}



bool RenderText(Program const* const prog, std::string text, bool center, float scale, float lineHeight, float width, int viewWidth, int viewHeight, Vector2 position) {

  unsigned int finalTexture = 0;
  glGenTextures(1, &finalTexture);
  glBindTexture(GL_TEXTURE_2D, finalTexture);

  glm::mat4 model = glm::mat4(1);
  model = glm::scale(model, glm::vec3(2.0f / viewWidth, 2.0f / viewHeight, 1.0f));



  int maxX = int(width * viewWidth) >> 1;

  int lineJump = scale * lineHeight * _FONT_LOAD_SIZE;
  IVector2 startPenPos = IVector2((position.x * viewWidth) + viewWidth, (position.y * viewHeight) + viewHeight);
  IVector2 penPos = startPenPos;
  penPos.y -= lineJump;

  bool cr = false;

  for (char c : text) {
    auto glyph = GetCharacter(c);

    if (penPos.x + glyph.size.x > maxX) {
      penPos.x = startPenPos.x;
      penPos.y -= lineJump;
    }

    if (c == '\r') {
      cr = true;
      penPos.x = startPenPos.x;
      penPos.y -= lineJump;
      continue;
    }
    if (c == '\n') {
      if (!cr) {
        penPos.x = startPenPos.x;
        penPos.y -= lineJump;
      }
      continue;
    }
    cr = false;

    if (c == ' ') {
      penPos.x += glyph.advance;
      continue;
    }

    IVector2 tmpPen = penPos;
    tmpPen.x += glyph.bearing.x;
    tmpPen.y -= (glyph.size.y - glyph.bearing.y);
    float square[] = {
      tmpPen.x, tmpPen.y, 0.f, 0.f,
      tmpPen.x + glyph.size.x, tmpPen.y, 1.f, 0.f,
      tmpPen.x, tmpPen.y + glyph.size.y, 0.f, 1.0f,
      tmpPen.x + glyph.size.x, tmpPen.y + glyph.size.y, 1.f, 1.f
    };

    // Buffer orphaning
    glBufferData(VBO, sizeof(square), nullptr, GL_STREAM_DRAW);
    glBufferSubData(VBO, 0, sizeof(square), square);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    penPos.x += glyph.advance;
  }


  return true;
}



const RenderedCharacter& GetCharacter(const char& c) {

  // Find character
  auto search = characterMap.find(c);
  if (search != characterMap.end()) {
    return search->second;
  }



  // Get index of char
  auto ind = FT_Get_Char_Index(ftFace, c);
  if (ind == 0) {
    throw new std::invalid_argument("Failed to find character : "s + c);
  }


  // Load
  FT_Error err = FT_Load_Glyph(ftFace, ind, FT_LOAD_RENDER);
  if (err) {
    throw new std::runtime_error("Failed to load chracter : "s + c);
  }


  // Set values
  RenderedCharacter ch = RenderedCharacter();
  ch.size = IVector2(ftFace->glyph->bitmap.width, ftFace->glyph->bitmap.rows);
  ch.bearing = IVector2(ftFace->glyph->bitmap_left, ftFace->glyph->bitmap_top);
  ch.advance = ftFace->glyph->advance.x >> 6;


  // Generate texture
  glGenTextures(1, &ch.texture);
  glBindTexture(GL_TEXTURE_2D, ch.texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED_INTEGER, ftFace->glyph->bitmap.width, ftFace->glyph->bitmap.rows, 0, GL_RED_INTEGER, GL_BYTE, ftFace->glyph->bitmap.buffer);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


  // Insert into map and return
  return characterMap.insert(std::pair<wchar_t, RenderedCharacter>(c, ch)).first->second;
}


}; // namespace text_factory
}; // namespace ste