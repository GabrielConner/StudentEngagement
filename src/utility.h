#ifndef STUDENT_ENGAGMENT_SRC_UTILITY_H
#define STUDENT_ENGAGMENT_SRC_UTILITY_H

#include <iostream>
#include <string>

namespace ste {

void PrintError(std::string msg) {
  std::cerr << "[ERROR] : " << msg << '\n';
}


}


#endif