#include "program.h"
#include "textRendering.h"
#include "scene.h"
#include "objectCallbacks.h"
#include "views/Example.h"
#include "views\loginView.h"
#include "views\studentView.h"
#include "views\leaderboard.h"
#include "views\events.h"
#include "views\admin.h"

using namespace ::ste;


int main() {
  Program program;
  if (!program.Start()) return 1;

  text_factory::StartText("./fonts/CascadiaCode.ttf");

  views::admin::Initialize(&program);
  views::events::Initialize(&program);
  views::leaderboard::Initialize(&program);
  views::login_view::Initialize(&program);
  views::student_view::Initialize(&program);

  program.ChangeScene("login_view");


  if (!program.Update()) return 1;
  program.End();

  text_factory::EndText();

  return 0;
}