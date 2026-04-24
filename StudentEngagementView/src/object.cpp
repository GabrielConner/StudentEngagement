#include "studentEngagement.h"
#include "object.h"
#include "textRendering.h"


namespace ste {

void Object::Render(Program const* const prog) {
  prog->objShader.Active();
  glBindVertexArray(prog->VAO);

  glm::mat4 model = glm::mat4(1);

  Shader::SetMat4("model", glm::value_ptr(model));
  Shader::SetVector4("color", backgroundColor);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  if (text.size() != 0) {
    RenderTextInfo render = {
      .center = centerText,
      .scale = textScale,
      .lineHeight = 1.0f,
      .renderWidth = scale.x,
      .framebufferWidth = prog->ScreenWidth(),
      .framebufferHeight = prog->ScreenHeight(),
      .position = Vector2(position.x, vertCenterText ? position.y - scale.y / 2 : position.y),
      .color = textColor
    };

    text_factory::RenderText(prog, text, render);
  }
}

}