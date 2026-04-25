#ifndef STUDENT_ENGAGEMENT_SRC_CLIENT_H
#define STUDENT_ENGAGEMENT_SRC_CLIENT_H

#include "models.h"

namespace ste {
namespace client {

bool Start();

void AddEvent(const models::Event& val);
void AddResource(const models::Resource& val);

void GiveUserBadge(int user, int badge);
void AddStudentToEvent(int student, int event);

char LoginUser(char const email[255], char const password[32]);


void End();


}; // namespace client
}; // namespace ste


#endif