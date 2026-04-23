#include "program.h"

using namespace ::ste;


int main() {
  Program program;

  if (!program.Start()) return 1;
  if (!program.Update()) return 1;
  program.End();

  return 0;
}