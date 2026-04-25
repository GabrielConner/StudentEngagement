#ifndef STUDENT_ENGAGEMENT_SRC_DATABASE_H
#define STUDENT_ENGAGEMENT_SRC_DATABASE_H

#include "models.h"

namespace ste {
namespace database {

void Start();


void AddEvent(const models::Event& val);
void AddResource(const models::Resource& val);

void GiveUserBadge(int user, int badge);
void AddStudentToEvent(int user, int event);


void End();

}; // namespace database
}; // namespace ste

#endif