#ifndef STUDENT_ENGAGEMENT_SRC_DATABASE_H
#define STUDENT_ENGAGEMENT_SRC_DATABASE_H

#include "models.h"
#include <vector>

namespace ste {
namespace database {


bool Start();
void Fill();


void AddEvent(const models::Event& val);
void AddResource(const models::Resource& val);

void GiveUserBadge(int user, int badge);
void AddStudentToEvent(int student, int event);

models::User LoginUser(char email[255], char password[32]);

models::Student GetStudentAccount(int user_id);

std::vector<models::Student> GetPointLeaderboard();

std::vector<models::Event> GetStudentEvents(int student_id);
std::vector<models::Event> GetUpcomingEvents();


void UpdateStudentPoints(int student_id, int newPoints);

void End();

}; // namespace database
}; // namespace ste

#endif