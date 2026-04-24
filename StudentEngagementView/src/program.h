#ifndef STUDENT_ENGAGEMENT_SRC_PROGRAM_H
#define STUDENT_ENGAGEMENT_SRC_PROGRAM_H

#include "studentEngagement.h"
#include "textRendering.h"

namespace ste {

class Program {

GLFWwindow* window;
TextRenderingFactory textFactory;

int width;
int height;

public:

bool Start();
bool Update();
void End();


int ScreenWidth() const { return width; }
int ScreenHeight() const { return height; }

};

}; // namespace ste

#endif