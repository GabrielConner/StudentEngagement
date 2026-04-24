#ifndef STUDENT_ENGAGEMENT_SRC_RENDERABLE_H
#define STUDENT_ENGAGEMENT_SRC_RENDERABLE_H

#include "vector.h"

namespace ste {

class Program;
class Renderable {
  public:

  virtual void Render(Program const* const prog) = 0;
  virtual Renderable* PositionIn(Vector2 pos) = 0;
};

}; // namespace ste

#endif