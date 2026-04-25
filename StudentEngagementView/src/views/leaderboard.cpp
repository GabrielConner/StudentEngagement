#include "views/leaderboard.h"
#include "program.h"
#include "objectCallbacks.h"
#include "vector.h"
#include "models.h"
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
namespace leaderboard {

void Initialize(Program* const prog) {
  scene = std::make_shared<Scene>();
  scene->start = LeaderboardStart;

  scene->backgroundColor = Color(1.0f);

  // Board
  board = std::make_shared<Object>(Point2(0), Vector2(1.2f, 0.9f), Color(0.1, 0.2, 0.6, 1.0), "");
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


  prog->RegisterScene("leaderboard", scene);
}

void LeaderboardStart(Program* const prog) {
  std::vector<Student> top = client::GetPointLeaderboard();

  // Remove references to board current top
  for (auto child : board->children) {
    auto f = std::find(scene->renderList.begin(), scene->renderList.end(), child);
    if (f == scene->renderList.end()) {
      continue;
    }
    scene->renderList.erase(f);
  }
  board->children.clear();


  // Toss in new top students
  for (int i = 0; i < top.size(); i++) {
    float yellow = float(top.size() - i) / top.size();

    auto cell = std::make_shared<Object>(Point2(0, 0.7 - (0.45 * i)),
                                         Vector2(0.6, 0.2), Color(yellow, yellow, 0, 0.8),
                                         std::format("#{:} {:} : {:} points", i + 1, top[i].name, top[i].points));
    cell->centerText = true;
    cell->vertCenterText = true;

    cell->SetCurrent();
    scene->AddObject(cell, board);
  }

}


}; // namespace leaderboard
}; // namespace views
}; // namespace ste