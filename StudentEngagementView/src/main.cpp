#include "program.h"
#include "textRendering.h"
#include "scene.h"

using namespace ::ste;


int main() {
  Program program;
  if (!program.Start()) return 1;

  text_factory::StartText("./fonts/CascadiaCode.ttf");

  auto main = std::make_shared<Scene>();
  main->backgroundColor = Color(0.2, 0.3, 0.3, 1.0);

  auto bigObj = std::make_shared<Object>();
  bigObj->scale = 0.5f;
  auto smallObj = std::make_shared<Object>(Point2(0.5,0.5), Vector2(0.5f), Color(1,0,0,1), "Hi");
  smallObj->centerText = true;
  smallObj->vertCenterText = true;



  main->AddRenderable(bigObj);
  main->AddObject(smallObj, bigObj);

  program.RegisterScene("main", main);
  program.ChangeScene("main");



  if (!program.Update()) return 1;
  program.End();

  text_factory::EndText();

  return 0;
}