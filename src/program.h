#ifndef STUDENT_ENGAGEMENT_SRC_PROGRAM_H
#define STUDENT_ENGAGEMENT_SRC_PROGRAM_H

#include "studentEngagement.h"
#include "textRendering.h"

namespace ste {

class Program {

GLFWwindow* window;
TextRenderingFactory textFactory;

public:

bool Start();
bool Update();
void End();

};

}; // namespace ste

#endif