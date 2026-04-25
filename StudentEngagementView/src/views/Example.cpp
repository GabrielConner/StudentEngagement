#include "views/Example.h"
#include "program.h"
#include "objectCallbacks.h"
#include "vector.h"


namespace ste {
namespace views {
namespace example {

void Initialize(Program* const prog) {
  auto example = std::make_shared<Scene>();
  example->backgroundColor = Color(0.2, 0.3, 0.3, 1.0);

  auto bigObj = std::make_shared<Object>();
  bigObj->scale = 0.5f;
  auto smallObj = std::make_shared<Object>(Point2(0.5, 0.5), Vector2(0.5f), Color(1, 0, 0, 1), "Hi");
  smallObj->centerText = true;
  smallObj->vertCenterText = true;

  smallObj->cycle = ::ste::callbacks::ButtonCycle;
  smallObj->SetCurrent();


  example->AddRenderable(bigObj);
  example->AddObject(smallObj, bigObj);

  prog->RegisterScene("example", example);
}

void ExampleStart(Program* const prog) {

}


}; // namespace example
}; // namespace views
}; // namespace ste