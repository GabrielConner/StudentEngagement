#include "views\loginView.h"
#include "program.h"
#include "objectCallbacks.h"
#include "vector.h"


namespace ste {
namespace views {
namespace login_view {

void Initialize(Program* const prog) {
  auto login_view = std::make_shared<Scene>();
  login_view->backgroundColor = Color(0.2, 0.3, 0.3, 1.0);

  auto bigObj = std::make_shared<Object>();
  bigObj->scale = Vector2(0.9, 0.9);
  login_view->AddRenderable(bigObj);
  
  //Login header
  auto loginHeader = std::make_shared<Object>(Point2(-0.02, 0.6), Vector2(0.8, 0.12), Color(1), "Login");

  login_view->AddObject(loginHeader, bigObj);
  loginHeader->centerText = true;


 



  //OTC ID Box
  auto idBox = std::make_shared<Object>(Point2(-0.02, 0.2), Vector2(0.8, 0.12), Color(0, 0, 1, 0.8), "OTC ID");

  idBox->centerText = true;

  //smallObj1->vertCenterText = true;

  idBox->onClickPress = ::ste::callbacks::Darken;
  idBox->onClickRelease = ::ste::callbacks::Reset;
  idBox->SetCurrent();


  //login_view->AddRenderable(bigObj);
  login_view->AddObject(idBox, bigObj);

;


  //Email Box
  auto emailBox = std::make_shared<Object>(Point2(-0.02, -0.1), Vector2(0.8, 0.12), Color(0, 0, 1, 0.8), "Email");

  emailBox->centerText = true;
  //smallObj2->vertCenterText = true;

  emailBox->onClickPress = ::ste::callbacks::Darken;
  emailBox->onClickRelease = ::ste::callbacks::Reset;
  emailBox->SetCurrent();


  //login_view->AddRenderable(bigObj);
  login_view->AddObject(emailBox, bigObj);





  // Password Box
  auto passwordBox = std::make_shared<Object>(Point2(-0.02, -0.4), Vector2(0.8, 0.12), Color(0, 0, 1, 0.8), "Password");

  passwordBox->centerText = true;
  //smallObj3->vertCenterText = true;

  passwordBox->onClickPress = ::ste::callbacks::Darken;
  passwordBox->onClickRelease = ::ste::callbacks::Reset;
  passwordBox->SetCurrent();

  login_view->AddObject(passwordBox, bigObj);


  /*				login_view->AddRenderable(bigObj);
      login_view->AddRenderable(smallObj1);
      login_view->AddRenderable(smallObj2);
      login_view->AddRenderable(smallObj3);*/




  //Submit box
  auto submitBox = std::make_shared<Object>(Point2(-0.02, -0.8), Vector2(0.8, 0.12), Color(0.2, 0.2, 0.2, 0.1), "Submit");

  login_view->AddObject(submitBox, bigObj);
  submitBox->centerText = true;

  submitBox->onClickPress = ::ste::callbacks::Darken;
  submitBox->onClickRelease = ::ste::callbacks::Reset;
  submitBox->SetCurrent();


  prog->RegisterScene("login_view", login_view);

}

void loginViewStart(Program* const prog) {

}




}; // namespace login_view
}; // namespace views
}; // namespace ste