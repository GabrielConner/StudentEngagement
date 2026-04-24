#include "program.h"
#include "textRendering.h"
#include "scene.h"

using namespace ::ste;


int main() {
  Program program;
  if (!program.Start()) return 1;

  text_factory::StartText("./fonts/CascadiaCode.ttf");

  auto main = std::make_shared<Scene>();



  main->AddRenderable(std::make_shared<Object>());

  program.RegisterScene("main", main);
  program.ChangeScene("main");



  if (!program.Update()) return 1;
  program.End();

  text_factory::EndText();

  return 0;
}