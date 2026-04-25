#include "views/studentView.h"
#include "program.h"
#include "objectCallbacks.h"
#include "vector.h"
#include "imageLoading.h"


namespace ste {
	namespace views {
		namespace student_view {

			void Initialize(Program* const prog) {
				auto student_view = std::make_shared<Scene>();
				student_view->backgroundColor = Color(1);

				auto bigObj = std::make_shared<Object>();
				bigObj->scale = 0.6f;

				//Add image banner
				auto imgObj = std::make_shared<Object>(Point2(0, .9), Vector2(3, 0.9), Color(0, 0, 1, 0.8), "");
				imgObj->texture = LoadImage(".\\img\\Ozark Technology.png");

				student_view->AddObject(imgObj, bigObj);

				//Events tab 
				auto EventsTab = std::make_shared<Object>(Point2(-1.5, -0.9), Vector2(0.8,0.7), Color(0, 0, 1, 0.8), "Upcoming Events");

				EventsTab->centerText = true;
				EventsTab->vertCenterText = true;

				EventsTab->cycle = ::ste::callbacks::ButtonCycle;
				EventsTab->SetCurrent();


				student_view->AddObject(EventsTab, bigObj);


				//Leaderboard tab
				
				auto LeaderBoard = std::make_shared<Object>(Point2(1.5, -0.9), Vector2(0.8, 0.7), Color(0, 0, 1, 0.8), "Leaderboard");

				LeaderBoard->centerText = true;
				LeaderBoard->vertCenterText = true;

				LeaderBoard->cycle = ::ste::callbacks::ButtonCycle;
				LeaderBoard->SetCurrent();


				student_view->AddObject(LeaderBoard, bigObj);


				prog->RegisterScene("student_view", student_view);

				//Log out button

				auto logOut = std::make_shared<Object>(Point2(0,-0.9), Vector2(0.4, 0.2), Color(1, 0, 0, 1), "Log Out");

				logOut->centerText = true;
				logOut->vertCenterText = true;

				logOut->cycle = ::ste::callbacks::ButtonCycle;
				logOut->SetCurrent();


				student_view->AddObject(logOut, bigObj);


			}

			void studentViewStart(Program* const prog) {

			}


		}; // namespace student_view
	}; // namespace views
}; // namespace ste