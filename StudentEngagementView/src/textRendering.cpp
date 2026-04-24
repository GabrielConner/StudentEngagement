#include "textRendering.h"
#include "studentEngagement.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <stdexcept>


using namespace std::string_literals;



// anonymous namespace
namespace {

FT_Library ftLib;
FT_Face ftFace;

std::map<char, ::ste::RenderedCharacter> characterMap;

unsigned int VBO, VAO;

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
  // Default 96 DPI
  // Font size needs to be in 26.6 fractional pixel unit
  err = FT_Set_Char_Size(ftFace, 0, _FONT_LOAD_SIZE << 6, 96, 96);
  if (err) {
    PrintError("Failed to set char size in font face");
    return false;
  }

  if (!Shader::GenShader("./shaders/textShader.vert", "./shaders/textShader.frag", textShader)) {
    PrintError("Failed to generate shader");
  }



  // Generate buffer info
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





bool RenderText(std::string text, const RenderTextInfo& info) {

  // Get shader and intitial uniforms
  textShader.Active();
  glActiveTexture(GL_TEXTURE0);
  Shader::SetInt("texTarget", 0);
  Shader::SetVector4("color", info.color);

  glBindVertexArray(VAO);


  glm::mat4 model = glm::mat4(1);


  // ftFace->size->metrics.height is the 26.6 fractional pixel of the face height
  // Anytime the >> 6 it will probably mean 26.6 fractional pixel
  int faceHeight = ftFace->size->metrics.height >> 6;

  // Scale font to fit _FONT_LINES_PER_SCREEN onto screen, not frame
  float scaleToScreen = float(program::ScreenHeight()) / (faceHeight * _FONT_LINES_PER_SCREEN);

  // Starting position
  int lineJump = info.scale * info.lineHeight * faceHeight;
  IVector2 startPenPos = IVector2((info.position.x + 1.f) * info.framebufferWidth / 2.f, (info.position.y + 1.f) * info.framebufferHeight / 2.f);
  startPenPos /= scaleToScreen;

  IVector2 penPos = startPenPos;

  // Best with single lines like hori center
  // Doesn't consider total lines or anything like that
  // Moves start y to have first line characters be centered on starting line
  if (!info.vertCenter)
    penPos.y -= lineJump;
  else {
    penPos.y -= info.scale * faceHeight / 2;
  }



  // Farthest out a character can reach
  int maxX = (int(info.renderWidth * info.framebufferWidth / scaleToScreen) >> 2) + startPenPos.x;

  bool cr = false;


  // Basic center by getting largest line or ends if a line takes up whole area
  // Good for single lines
  // With multiple lines each line will still be left aligned to where largest line is
  int endOffset = 0;
  if (info.center) {
    int largestLine = 0;

    for (char c : text) {
      auto glyph = GetCharacter(c);
      if (penPos.x + glyph.size.x + glyph.bearing.x > maxX) {
        penPos.x = maxX;
        break;
      }

      if (c == '\r') {
        cr = true;
        if (penPos.x > largestLine) {
          largestLine = penPos.x;
        }
        penPos.x = startPenPos.x;
        penPos.y -= lineJump;
        continue;
      }
      if (c == '\n') {
        if (!cr) {
          if (penPos.x > largestLine) {
            largestLine = penPos.x;
          }
          penPos.x = startPenPos.x;
          penPos.y -= lineJump;
        }
        continue;
      }
      cr = false;

      penPos.x += glyph.advance;
    }


    if (penPos.x > largestLine) {
      largestLine = penPos.x;
    }

    int halfWidth = ((maxX - startPenPos.x) >> 1);
    endOffset = halfWidth - ((largestLine - startPenPos.x) >> 1);
    penPos = startPenPos;
    penPos.y -= lineJump;
    cr = false;
  }



  for (char c : text) {
    auto glyph = GetCharacter(c);


    // Does it reach out past max
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


    // Add bearing
    IVector2 tmpPen = penPos;
    tmpPen.x += glyph.bearing.x;
    tmpPen.y -= (glyph.size.y - glyph.bearing.y);


    /*
    Matrix multiplication goes in reverse order of calling :

    1. Scale up square to pixel size of character
    2. Move to pixel location with centering
    3. Scale to (0,0) - (2,2) space from pixel space
    4. Move to (-1,-1) - (1,1) space
    5. Adjust for aspect ratio

    */

    model = glm::mat4(1);
    model = glm::scale(model, glm::vec3(1.0f / program::ScreenAspect(), 1.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-1, -1, 0));
    model = glm::scale(model, glm::vec3(2.f * scaleToScreen / (info.framebufferWidth), 2.f * scaleToScreen / info.framebufferHeight, 1.0f));
    model = glm::translate(model, glm::vec3(tmpPen.x + endOffset, tmpPen.y, 0));
    model = glm::scale(model, glm::vec3(glyph.size.x, glyph.size.y, 1.0f));
    Shader::SetMat4("model", glm::value_ptr(model));

    // Bind character texture into texture location 0
    glBindTexture(GL_TEXTURE_2D, glyph.texture);

    // Draw
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


  // Load
  FT_Error err = FT_Load_Char(ftFace, c, FT_LOAD_RENDER);
  if (err) {
    throw new std::runtime_error("Failed to load chracter : "s + c);
  }


  // Set values
  RenderedCharacter ch = RenderedCharacter();
  ch.size = IVector2(ftFace->glyph->bitmap.width, ftFace->glyph->bitmap.rows);
  ch.bearing = IVector2(ftFace->glyph->bitmap_left, ftFace->glyph->bitmap_top);
  ch.advance = ftFace->glyph->advance.x >> 6;


  // Generate texture
  int packAlignment;
  glGetIntegerv(GL_UNPACK_ALIGNMENT, &packAlignment);

  // Since bitmap is stored as bytes the reading alignment needs to be 1 byte
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  // Create texture
  glGenTextures(1, &ch.texture);
  glBindTexture(GL_TEXTURE_2D, ch.texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, ftFace->glyph->bitmap.width, ftFace->glyph->bitmap.rows, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, ftFace->glyph->bitmap.buffer);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Restore to before this function was called
  // Only care about UNPACK because the others are set before immediate use anyways
  // UNPACK isn't
  glPixelStorei(GL_UNPACK_ALIGNMENT, packAlignment);


  // Insert into map and return
  return characterMap.insert(std::pair<wchar_t, RenderedCharacter>(c, ch)).first->second;
}



void EndText() {
  FT_Done_Face(ftFace);
  FT_Done_FreeType(ftLib);

  for (auto& p : characterMap) {
    glDeleteTextures(1, &p.second.texture);
  }
  characterMap.clear();

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(textShader);
}



}; // namespace text_factory
}; // namespace ste