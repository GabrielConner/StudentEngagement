#include "server.h"
#include "database.h"
#include "models.h"

using namespace ::ste;
using namespace ::ste::models;


int main() {
  database::Start();

  if (!server::Start())
    return 1;

  server::Run();
  server::End();


  database::End();
}