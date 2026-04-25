#ifndef STUDENT_ENGAGEMENT_SRC_CLIENT_H
#define STUDENT_ENGAGEMENT_SRC_CLIENT_H

#include "models.h"
#include <vector>

namespace ste {
namespace client {

bool Start();

void AddEvent(const models::Event& val);

models::User LoginUser(char const email[255], char const password[32]);
models::Student GetStudentAccount(int user_id);

std::vector<models::Student> GetPointLeaderboard();
std::vector<models::Event> GetStudentEvents(int student_id);

std::vector<models::Event> GetUpcomingEvents();

void UpdateStudentPoints(int student_id, int newPoints);


void End();


}; // namespace client
}; // namespace ste


#endif