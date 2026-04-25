#include "server.h"
#include "database.h"

using namespace ::ste;


int main() {
  database::Start();
  database::End();

/*  if (!server::Start())
    return 1;

  server::Run();
  server::End();*/
}