#ifndef STUDENT_ENGAGEMENT_SRC_DATABASE_H
#define STUDENT_ENGAGEMENT_SRC_DATABASE_H

#include "models.h"

namespace ste {
namespace database {


bool Start();
void Fill();


void AddEvent(const models::Event& val);
void AddResource(const models::Resource& val);

void GiveUserBadge(int user, int badge);
void AddStudentToEvent(int student, int event);

char LoginUser(char email[255], char password[32]);

void End();

}; // namespace database
}; // namespace ste

#endif