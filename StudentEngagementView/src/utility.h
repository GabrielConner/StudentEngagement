#ifndef STUDENT_ENGAGEMENT_SRC_UTILITY_H
#define STUDENT_ENGAGEMENT_SRC_UTILITY_H

#include <iostream>
#include <string>

namespace ste {

inline void PrintError(std::string msg) {
  std::cerr << "[ERROR] : " << msg << '\n';
}

}; // namespace ste


#endif