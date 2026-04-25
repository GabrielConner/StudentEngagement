#include "views/example.h"
#include "program.h"
#include "objectCallbacks.h"
#include "vector.h"
#include "models.h"
#include "client.h"
#include <iomanip>

using namespace ::ste;
using namespace ::ste::models;
using namespace ::ste::callbacks;


namespace {

std::shared_ptr<Object> eventTitle;
std::shared_ptr<Object> eventLocation;
std::shared_ptr<Object> eventTime;

std::shared_ptr<Object> eventId;
std::shared_ptr<Object> studentId;


void RegisterEvent(Program const* prog, Object* obj, const MouseEvent& event) {
  Event ev;
  strcpy_s(ev.title, sizeof(ev.title), eventTitle->text.c_str());
  strcpy_s(ev.location, sizeof(ev.location), eventLocation->text.c_str());
  strcpy_s(ev.time, sizeof(ev.time), eventTime->text.c_str());

  client::AddEvent(ev);

  eventTitle->text = "Event Title";
  eventLocation->text = "Event Location";
  eventTime->text = "Event Time";
}



void RegisterStudent(Program const* prog, Object* obj, const MouseEvent& event) {
  int evid = std::stoi(eventId->text);
  if (evid == 0)
    return;
  int stid = std::stoi(studentId->text);
  if (stid == 0)
    return;

  bool ret = client::AddStudentToEvent(stid, evid);

  if (ret) {
    client::AddStudentPoints(stid, 4);
  }


  eventId->text = "Event ID";
  studentId->text = "Student ID";
}



}; // namespace


namespace ste {
namespace views {
namespace admin {

void Initialize(Program* const prog) {
  auto scene = std::make_shared<Scene>();
  scene->backgroundColor = Color(0.4f, 0.4f, 0.4f, 1.0f);


  // Event creation

  auto eventCreate = std::make_shared<Object>(Point2(-1.0, 0.88), Vector2(0.6, 0.1), Color(0.2), "Create Event");
  eventTitle = std::make_shared<Object>(Point2(-1.0, 0.7), Vector2(0.6, 0.12), Color(1.0f), "Event Title");
  eventLocation = std::make_shared<Object>(Point2(-1.0, 0.35), Vector2(0.6, 0.12), Color(1.0f), "Event Location");
  eventTime = std::make_shared<Object>(Point2(-1.0, 0.0), Vector2(0.6, 0.12), Color(1.0f), "Event Time");
  auto eventSubmit = std::make_shared<Object>(Point2(-1.0, -0.85), Vector2(0.6, 0.2), Color(0.3, 0.3, 0.8, 1.0), "Submit");

  eventSubmit->centerText = true;
  eventSubmit->vertCenterText = true;


  eventTitle->cycle = ButtonCycle;
  eventLocation->cycle = ButtonCycle;
  eventTime->cycle = ButtonCycle;
  eventSubmit->cycle = ButtonCycle;

  eventTitle->SetCurrent();
  eventLocation->SetCurrent();
  eventTime->SetCurrent();
  eventSubmit->SetCurrent();

  eventTitle->onClickRelease = InputFieldEnter;
  eventLocation->onClickRelease = InputFieldEnter;
  eventTime->onClickRelease = InputFieldEnter;


  scene->AddRenderable(eventCreate);
  scene->AddRenderable(eventTitle);
  scene->AddRenderable(eventLocation);
  scene->AddRenderable(eventTime);
  scene->AddRenderable(eventSubmit);




  // Event registering

  auto registerEvent = std::make_shared<Object>(Point2(1.0, 0.88), Vector2(0.6, 0.1), Color(0.2), "Register Student");
  eventId = std::make_shared<Object>(Point2(1.0, 0.7), Vector2(0.6, 0.12), Color(1.0f), "Event ID");
  studentId = std::make_shared<Object>(Point2(1.0, 0.35), Vector2(0.6, 0.12), Color(1.0f), "Student ID");
  auto registerSubmit = std::make_shared<Object>(Point2(1.0, -0.85), Vector2(0.6, 0.15), Color(0.3, 0.3, 0.8, 1.0), "Submit");

  registerSubmit->centerText = true;
  registerSubmit->vertCenterText = true;

  eventId->cycle = ButtonCycle;
  studentId->cycle = ButtonCycle;
  registerSubmit->cycle = ButtonCycle;

  eventId->SetCurrent();
  studentId->SetCurrent();
  registerSubmit->SetCurrent();

  eventId->onClickRelease = InputFieldEnter;
  studentId->onClickRelease = InputFieldEnter;

  scene->AddRenderable(registerEvent);
  scene->AddRenderable(eventId);
  scene->AddRenderable(studentId);
  scene->AddRenderable(registerSubmit);



  //Log out button

  auto logOut = std::make_shared<Object>(Point2(0, -0.9), Vector2(0.4, 0.2), Color(1, 0, 0, 1), "Log Out");

  logOut->centerText = true;
  logOut->vertCenterText = true;

  logOut->data = "login_view";
  logOut->onClickRelease = SwitchScene;
  logOut->cycle = ButtonCycle;
  logOut->SetCurrent();

  prog->RegisterScene("admin", scene);
}



void ExampleStart(Program* const prog) {

}


}; // namespace admin
}; // namespace views
}; // namespace ste