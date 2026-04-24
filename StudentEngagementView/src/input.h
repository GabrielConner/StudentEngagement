#ifndef STUDENT_ENGAGEMENT_SRC_INPUT_H
#define STUDENT_ENGAGEMENT_SRC_INPUT_H

namespace ste {

struct Input {
  int button;
  bool pressed;
  bool held;
  bool released;
};

}; // namespace ste


#endif