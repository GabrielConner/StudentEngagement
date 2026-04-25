#ifndef STUDENT_ENGAGEMENT_SRC_CONSTANTS_H
#define STUDENT_ENGAGEMENT_SRC_CONSTANTS_H

namespace ste {

// Pixel height of loaded font face
constexpr int _FONT_LOAD_SIZE = 64;
constexpr float _FONT_LINES_PER_SCREEN = 20.f;


constexpr int _PORT = 7071;
constexpr char const* _PORT_S = "7071";

constexpr int _SEND_BUF_LENGTH = 512;
constexpr int _MSG_HEADER = 16;
constexpr int _MSG_BODY = _SEND_BUF_LENGTH - _MSG_HEADER;



// --------------------------------------------------
// Headers
// --------------------------------------------------

constexpr char const _ADD_EVENT_MSG[_MSG_HEADER] = "AddEvent";

// --------------------------------------------------
// Headers
// --------------------------------------------------

}; // namespace ste


#endif