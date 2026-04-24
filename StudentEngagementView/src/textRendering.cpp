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

  if (!Shader::GenShader("./shaders/textShader.vert", "./shaders/textShader.frag", textShader)) {
    PrintError("Failed to generate shader");
  }

  glGenFramebuffers(1, &FBO);

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);


  float square[] = {
    0.f, 0.f,
    1.f, 0.f,
    0.f, 1.f,
    1.f, 1.f
  };

  
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);



  return true;
}





bool RenderText(Program const* const prog, std::string text, const RenderTextInfo& info) {

  //unsigned int finalTexture = 0;
  //glGenTextures(1, &finalTexture);
  //glBindTexture(GL_TEXTURE_2D, finalTexture);


  textShader.Active();
  glActiveTexture(GL_TEXTURE0);
  Shader::SetInt("texTarget", 0);
  Shader::SetVector4("color", info.color);

  glBindVertexArray(VAO);


  glm::mat4 model = glm::mat4(1);

  int faceHeight = ftFace->size->metrics.height >> 6;
  float scaleToScreen = float(prog->ScreenHeight()) / (faceHeight * _FONT_LINES_PER_SCREEN);


  int lineJump = info.scale * info.lineHeight * faceHeight;
  IVector2 startPenPos = IVector2((info.position.x + 1.f) * info.framebufferWidth / 2.f, (info.position.y + 1.f) * info.framebufferHeight / 2.f);
  startPenPos /= scaleToScreen;

  IVector2 penPos = startPenPos;
  penPos.y -= lineJump;

  int maxX = (int(info.renderWidth * info.framebufferWidth / (2.f * scaleToScreen)) >> 1) + startPenPos.x;


  bool cr = false;

  for (char c : text) {
    auto glyph = GetCharacter(c);

    if (penPos.x + glyph.size.x + glyph.bearing.x > maxX) {
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


    model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(-1, -1, 0));
    model = glm::scale(model, glm::vec3(2.f  * scaleToScreen / info.framebufferWidth, 2.f  * scaleToScreen / info.framebufferHeight, 1.0f));
    model = glm::translate(model, glm::vec3(penPos.x + glyph.bearing.x, penPos.y - (glyph.size.y - glyph.bearing.y), 0));
    model = glm::scale(model, glm::vec3(glyph.size.x, glyph.size.y, 1.0f));
    Shader::SetMat4("model", glm::value_ptr(model));

/*    glm::vec4 p = glm::vec4(1,0,0,1);
    p = model * p;
    std::cout << p.x << ", " << p.y << ", " << p.z << ", " << p.w << '\n';*/

    glBindTexture(GL_TEXTURE_2D, glyph.texture);


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
  int packAlignment, currentTexture;
  glGetIntegerv(GL_UNPACK_ALIGNMENT, &packAlignment);
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


  glGenTextures(1, &ch.texture);
  glBindTexture(GL_TEXTURE_2D, ch.texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, ftFace->glyph->bitmap.width, ftFace->glyph->bitmap.rows, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, ftFace->glyph->bitmap.buffer);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Restore
  glPixelStorei(GL_UNPACK_ALIGNMENT, packAlignment);
  glBindTexture(GL_TEXTURE_2D, currentTexture);


  // Insert into map and return
  return characterMap.insert(std::pair<wchar_t, RenderedCharacter>(c, ch)).first->second;
}



void EndText() {

}



}; // namespace text_factory
}; // namespace ste