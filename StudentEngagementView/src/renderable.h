#ifndef STUDENT_ENGAGEMENT_SRC_RENDERABLE_H
#define STUDENT_ENGAGEMENT_SRC_RENDERABLE_H

namespace ste {

class Program;
class Renderable {
  public:

  virtual void Render(Program const* const prog) = 0;
};

}; // namespace ste

#endif