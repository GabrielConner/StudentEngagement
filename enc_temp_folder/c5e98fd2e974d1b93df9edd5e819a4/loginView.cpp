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
				bigObj->scale = 0.9,0.1;

				auto smallObj1 = std::make_shared<Object>(Point2(-0.02, 0.5), Vector2(0.8, 0.1), Color(0, 0, 2, 0.8), "OTC ID");

				smallObj1->centerText = true;
				smallObj1->vertCenterText = true;

				smallObj1->onClickPress = ::ste::callbacks::Darken;
				smallObj1->onClickRelease = ::ste::callbacks::Reset;
				smallObj1->SetCurrent();


				login_view->AddRenderable(bigObj);
				login_view->AddObject(smallObj1, bigObj);

				prog->RegisterScene("login_view", login_view);


				//Email Box
				auto smallObj2 = std::make_shared<Object>(Point2(-0.02, -0.1), Vector2(0.8, 0.1), Color(0, 0, 2, 0.8), "Email");

				smallObj2->centerText = true;
				smallObj2->vertCenterText = true;

				smallObj2->onClickPress = ::ste::callbacks::Darken;
				smallObj2->onClickRelease = ::ste::callbacks::Reset;
				smallObj2->SetCurrent();


				login_view->AddRenderable(bigObj);
				login_view->AddObject(smallObj2, bigObj);

				prog->RegisterScene("login_view", login_view);




				// Password Box
				auto smallObj3 = std::make_shared<Object>(Point2(-0.02, -0.5), Vector2(0.8,0.1), Color(0, 0, 2, 0.8),"Password");

				smallObj3->centerText = true;
				smallObj3->vertCenterText = true;

				smallObj3->onClickPress = ::ste::callbacks::Darken;
				smallObj3->onClickRelease = ::ste::callbacks::Reset;
				smallObj3->SetCurrent();


				login_view->AddRenderable(bigObj);
				login_view->AddRenderable(smallObj1);
				login_view->AddRenderable(smallObj2);
				login_view->AddRenderable(smallObj3);
				//login_view->AddObject(smallObj2,smallObj1,smallObj3, bigObj);

				prog->RegisterScene("login_view", login_view);
			}

			void loginViewStart(Program* const prog) {

			}

			


		}; // namespace login_view
	}; // namespace views
}; // namespace ste