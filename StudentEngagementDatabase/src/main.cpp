#include "server.h"

using namespace ::ste;


int main() {
  if (!server::Start())
    return 1;

  server::Run();
  server::End();
}