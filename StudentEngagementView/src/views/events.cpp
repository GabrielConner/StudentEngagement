#include "views/events.h"
#include "program.h"
#include "objectCallbacks.h"
#include "vector.h"
#include "object.h"
#include "client.h"
#include <format>

using namespace ::ste;
using namespace ::ste::models;
using namespace ::ste::callbacks;


namespace {

std::shared_ptr<Scene> scene;
std::shared_ptr<Object> board;

}; // namespace


namespace ste {
namespace views {
namespace events {

void Initialize(Program* const prog) {
  scene = std::make_shared<Scene>();
  scene->start = EventsStart;

  scene->backgroundColor = Color(1.0f);

  // Board
  board = std::make_shared<Object>(Point2(0), Vector2(1.2f, 0.9f), Color(0.4, 0.6, 0.3, 1.0), "");
  scene->AddRenderable(board);

  // Back
  auto back = std::make_shared<Object>(Point2(1.5, 0.8), Vector2(0.25, 0.2), Color(1, 0, 0, 1), "Back");

  back->centerText = true;
  back->vertCenterText = true;

  back->data = "student_view";
  back->onClickRelease = SwitchScene;
  back->cycle = ButtonCycle;
  back->SetCurrent();
  scene->AddRenderable(back);


  prog->RegisterScene("events", scene);
}

void EventsStart(Program* const prog) {
  std::vector<Event> top = client::GetUpcomingEvents();

  // Remove references to board current top
  for (auto child : board->children) {
    auto f = std::find(scene->renderList.begin(), scene->renderList.end(), child);
    if (f == scene->renderList.end()) {
      continue;
    }
    scene->renderList.erase(f);
  }
  board->children.clear();


  for (int i = 0; i < top.size(); i++) {
    auto cell = std::make_shared<Object>(Point2(0, 0.68 - (0.45 * i)),
                                         Vector2(0.9, 0.2), Color(0.2, 0.2, 0.2, 1.0f),
                                         std::format("{:} in {:} at {:}", top[i].title, top[i].location, top[i].time));
    cell->textColor = Vector4(1.0f);
    cell->centerText = true;
    cell->vertCenterText = true;

    cell->SetCurrent();
    scene->AddObject(cell, board);
  }

}


}; // namespace events
}; // namespace views
}; // namespace ste