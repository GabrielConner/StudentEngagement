#include "views/studentView.h"
#include "program.h"
#include "objectCallbacks.h"
#include "vector.h"


namespace ste {
	namespace views {
		namespace student_view {

			void Initialize(Program* const prog) {
				auto student_view = std::make_shared<Scene>();
				student_view->backgroundColor = Color(1);

				auto bigObj = std::make_shared<Object>();
				bigObj->scale = 0.5f;

				//Events 
				auto EventsTab = std::make_shared<Object>(Point2(-0.02, -0.4), Vector2(0.4, 0.12), Color(0, 0, 1, 0.8), "Upcoming Events");

				EventsTab->centerText = true;
				//smallObj3->vertCenterText = true;

				EventsTab->cycle = ::ste::callbacks::ButtonCycle;
				EventsTab->SetCurrent();


				student_view->AddRenderable(bigObj);
				student_view->AddObject(EventsTab, bigObj);

				prog->RegisterScene("student_view", student_view);
			}

			void studentViewStart(Program* const prog) {

			}


		}; // namespace student_view
	}; // namespace views
}; // namespace ste