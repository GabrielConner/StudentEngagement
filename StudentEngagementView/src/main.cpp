#include "program.h"
#include "textRendering.h"


using namespace ::ste;


int main() {
  Program program;
  if (!program.Start()) return 1;



  text_factory::StartText("./fonts/CascadiaCode.ttf");



  if (!program.Update()) return 1;
  program.End();

  text_factory::EndText();

  return 0;
}