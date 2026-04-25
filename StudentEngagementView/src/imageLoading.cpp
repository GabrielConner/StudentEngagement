#include "imageLoading.h"
#include "stb_image.h"

#include <filesystem>

#include "glad\glad.h"

namespace ste {


unsigned int LoadImage(char const* path) {
  if (!std::filesystem::exists(path)) {
    return 0;
  }

  stbi_set_flip_vertically_on_load(true);

  int width = 0, height = 0, channels = 0, rChannels = 3;
  uint8_t* bytes = stbi_load(path, &width, &height, &channels, rChannels);
  if (bytes == nullptr) {
    return 0;
  }

  unsigned int tex = 0;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_image_free(bytes);
  return tex;
}

}; // namespace ste