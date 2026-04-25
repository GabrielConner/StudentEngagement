#ifndef STUDENT_ENGAGEMENT_SRC_MODELS_H
#define STUDENT_ENGAGEMENT_SRC_MODELS_H

namespace ste {
namespace models {

struct Event {
  int event_id;
  char title[41];
  char location[51];
  char time[17];
  int exp_attendance;
  bool audio_video;
  int budget;
};


struct Resource {
  int resource_id;
  int event_id;
  char name[31];
};


struct Badge {
  int badge_id;
  int points_req;
  char title[51];
};


struct EventStudent {
  int event_id;
  int student_id;
};


struct BadgeStudent {
  int badge_id;
  int student_id;
};


struct Student {
  int student_id;
  int points;
  int user_id;
  char name[30];
};


struct User {
  int user_id;
  char email[256];
  char password[33];
  int permission;
};


}; // namespace model
}; // namespace ste

#endif