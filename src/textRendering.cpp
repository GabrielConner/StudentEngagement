#include "textRendering.h"
#include "utility.h"
#include "constants.h"

#include <ft2build.h>
#include FT_FREETYPE_H





namespace ste {

bool TextRenderingFactory::StartText(std::string font) {
  FT_Error err = FT_Init_FreeType(&ftLib);
  if (err) {
    PrintError("Failed to initialize FreeType");
    return false;
  }


  err = FT_New_Face(ftLib, font.c_str(), 0, &ftFace);
  if (err) {
    PrintError("Failed to generate font face");
    return false;
  }

  FT_Set_Char_Size(ftFace, 0, _FONT_LOAD_SIZE << 6, 96, 96);
}

}; // namespace ste