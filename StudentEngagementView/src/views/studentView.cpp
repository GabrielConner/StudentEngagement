#include "views/studentView.h"
#include "program.h"
#include "objectCallbacks.h"
#include "vector.h"
#include "imageLoading.h"
#include "models.h"
#include "client.h"
#include <exception>
#include <format>


using namespace ::ste;
using namespace ::ste::models;
using namespace ::ste::callbacks;


namespace {

void Logout(Program* const prog, Object* obj, const MouseEvent& event) {
  prog->ChangeScene("login_view");
}



Student loggedStudent;

std::shared_ptr<Object> pointsDisplay;

}; // namespace



namespace ste {
namespace views {
namespace student_view {

void Initialize(Program* const prog) {
  auto student_view = std::make_shared<Scene>();
  student_view->start = studentViewStart;


  student_view->backgroundColor = Color(1);

  auto bigObj = std::make_shared<Object>();
  bigObj->scale = 0.6f;

  //Add image banner
  auto imgObj = std::make_shared<Object>(Point2(0, .9), Vector2(3, 0.9), Color(0, 0, 1, 0.8), "");
  imgObj->texture = LoadImage(".\\img\\Ozark Technology.png");

  student_view->AddObject(imgObj, bigObj);

  //Events tab 
  auto EventsTab = std::make_shared<Object>(Point2(-1.5, -0.9), Vector2(0.8, 0.7), Color(0, 0, 1, 0.8), "Upcoming Events");

  EventsTab->data = "events";
  EventsTab->onClickRelease = SwitchScene;
  EventsTab->centerText = true;
  EventsTab->vertCenterText = true;

  EventsTab->cycle = ::ste::callbacks::ButtonCycle;
  EventsTab->SetCurrent();

  student_view->AddObject(EventsTab, bigObj);


  //Leaderboard tab

  auto LeaderBoard = std::make_shared<Object>(Point2(1.5, -0.9), Vector2(0.8, 0.7), Color(0, 0, 1, 0.8), "Leaderboard");

  LeaderBoard->data = "leaderboard";
  LeaderBoard->onClickRelease = SwitchScene;
  LeaderBoard->centerText = true;
  LeaderBoard->vertCenterText = true;


  LeaderBoard->cycle = ButtonCycle;
  LeaderBoard->SetCurrent();
  student_view->AddObject(LeaderBoard, bigObj);



  //Log out button

  auto logOut = std::make_shared<Object>(Point2(0, -0.9), Vector2(0.4, 0.2), Color(1, 0, 0, 1), "Log Out");

  logOut->centerText = true;
  logOut->vertCenterText = true;

  logOut->onClickRelease = Logout;
  logOut->cycle = ButtonCycle;
  logOut->SetCurrent();


  student_view->AddObject(logOut, bigObj);


  // Points display
  pointsDisplay = std::make_shared<Object>(Point2(0, -1.4), Vector2(0.6, 0.2), Color(0), "");
  pointsDisplay->centerText = true;

  student_view->AddObject(pointsDisplay, bigObj);


  prog->RegisterScene("student_view", student_view);
}




void studentViewStart(Program* const prog) {
  Student studentAccount = client::GetStudentAccount(prog->currentUser.user_id);
  if (studentAccount.student_id == -1) {
    throw std::logic_error("User not found to be a student");
  }

  pointsDisplay->text = std::format("Points : {:}", studentAccount.points);
}


}; // namespace student_view
}; // namespace views
}; // namespace ste