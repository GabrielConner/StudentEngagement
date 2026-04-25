#include "views\loginView.h"
#include "program.h"
#include "objectCallbacks.h"
#include "vector.h"
#include "client.h"
#include "models.h"

using namespace ::ste;
using namespace ::ste::models;
using namespace ::ste::callbacks;


namespace {

void SendLogin(Program* const prog, Object* obj, const MouseEvent& event);

std::shared_ptr<Object> emailBox;
std::shared_ptr<Object> passwordBox;
std::shared_ptr<Object> badInput;

}; // namespace



namespace ste {
namespace views {
namespace login_view {

void Initialize(Program* const prog) {
  auto login_view = std::make_shared<Scene>();
  login_view->start = loginViewStart;

  login_view->backgroundColor = Color(0.2, 0.3, 0.3, 1.0);

  auto bigObj = std::make_shared<Object>();
  bigObj->scale = Vector2(0.9, 0.9);
  login_view->AddRenderable(bigObj);



  badInput = std::make_shared<Object>(Point2(-0.02, 0.8), Vector2(0.8, 0.12), Color(1), "");
  badInput->textScale = 0.65f;
  badInput->textColor = Color(1,0,0,1);
  login_view->AddObject(badInput, bigObj);

  
  //Login header
  auto loginHeader = std::make_shared<Object>(Point2(-0.02, 0.6), Vector2(0.8, 0.12), Color(1), "Login");

  login_view->AddObject(loginHeader, bigObj);
  loginHeader->centerText = true;




  //OTC ID Box
  auto idBox = std::make_shared<Object>(Point2(-0.02, 0.2), Vector2(0.8, 0.12), Color(0, 0, 1, 0.8), "OTC ID");

  idBox->centerText = true;

  //smallObj1->vertCenterText = true;

  idBox->onClickRelease = InputFieldEnter;
  idBox->cycle = ButtonCycle;
  idBox->SetCurrent();


  //login_view->AddRenderable(bigObj);
  login_view->AddObject(idBox, bigObj);

;


  //Email Box
  emailBox = std::make_shared<Object>(Point2(-0.02, -0.1), Vector2(0.8, 0.12), Color(0, 0, 1, 0.8), "Email");

  emailBox->centerText = true;
  //smallObj2->vertCenterText = true;

  emailBox->onClickRelease = InputFieldEnter;
  emailBox->cycle = ButtonCycle;
  emailBox->SetCurrent();


  //login_view->AddRenderable(bigObj);
  login_view->AddObject(emailBox, bigObj);





  // Password Box
  passwordBox = std::make_shared<Object>(Point2(-0.02, -0.4), Vector2(0.8, 0.12), Color(0, 0, 1, 0.8), "Password");

  passwordBox->centerText = true;
  //smallObj3->vertCenterText = true;

  passwordBox->onClickRelease = InputFieldEnter;
  passwordBox->cycle = ::ste::callbacks::ButtonCycle;
  passwordBox->SetCurrent();

  login_view->AddObject(passwordBox, bigObj);



  //Submit box
  auto submitBox = std::make_shared<Object>(Point2(-0.02, -0.8), Vector2(0.8, 0.12), Color(1.0f), "Submit");

  login_view->AddObject(submitBox, bigObj);
  submitBox->centerText = true;

  submitBox->onClickRelease = SendLogin;
  submitBox->cycle = ::ste::callbacks::ButtonCycle;
  submitBox->SetCurrent();


  prog->RegisterScene("login_view", login_view);
}

void loginViewStart(Program* const prog) {
  emailBox->text = "Email";
  passwordBox->text = "Password";
  badInput->text = "";
}



}; // namespace login_view
}; // namespace views
}; // namespace ste




namespace {

void SendLogin(Program* const prog, Object* obj, const MouseEvent& event) {
  User ret = client::LoginUser(emailBox->text.c_str(), passwordBox->text.c_str());
  if (ret.user_id == -1) {
    badInput->text = "Invalid email or password";
    return;
  } 
  prog->currentUser = ret;

  if (ret.permission == 0) {
    prog->ChangeScene("student_view");
  } else if (ret.permission == 1) {
    prog->ChangeScene("admin");
  }
}

}; // namespace