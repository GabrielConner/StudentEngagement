#include "program.h"
#include "textRendering.h"
#include "scene.h"
#include "objectCallbacks.h"
#include "views/Example.h"

using namespace ::ste;


int main() {
  Program program;
  if (!program.Start()) return 1;

  text_factory::StartText("./fonts/CascadiaCode.ttf");
  views::example::Initialize(&program);

  program.ChangeScene("example");


  if (!program.Update()) return 1;
  program.End();

  text_factory::EndText();

  return 0;
}