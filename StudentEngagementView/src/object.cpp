#include "studentEngagement.h"
#include "object.h"
#include "textRendering.h"


namespace ste {

void Object::Render(Program const* const prog) {
  prog->objShader.Active();
  glBindVertexArray(prog->VAO);

  glm::mat4 model = glm::mat4(1);
  model = glm::scale(model, glm::vec3(1.0f / program::ScreenAspect(), 1.0f, 1.0f));
  model = glm::translate(model, glm::vec3(position.x, position.y, 0));
  model = glm::scale(model, glm::vec3(scale.x, scale.y, 1.0f));

  Shader::SetMat4("model", glm::value_ptr(model));
  Shader::SetVector4("color", backgroundColor);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  if (text.size() != 0) {
    RenderTextInfo render = {
      .center = centerText,
      .vertCenter = vertCenterText,
      .scale = textScale,
      .lineHeight = 1.0f,
      .renderWidth = 4 * scale.x,
      .framebufferWidth = program::ScreenWidth(),
      .framebufferHeight = program::ScreenHeight(),
      .position = Vector2(position.x - scale.x, vertCenterText ? position.y + scale.y / 2 : position.y + scale.y),
      .color = textColor
    };

    text_factory::RenderText(text, render);
  }
}

}