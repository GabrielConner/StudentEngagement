#ifndef STUDENT_ENGAGEMENT_SRC_UTILITY_H
#define STUDENT_ENGAGEMENT_SRC_UTILITY_H

#include <iostream>

namespace ste {

void PrintError(std::string err) {
  std::cerr << "[ERROR] : " << err << '\n';
}

}; // namespace ste

#endif