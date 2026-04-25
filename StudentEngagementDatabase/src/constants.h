#ifndef STUDENT_ENGAGEMENT_SRC_CONSTANTS_H
#define STUDENT_ENGAGEMENT_SRC_CONSTANTS_H

namespace ste {

constexpr int _PORT = 7071;
constexpr char const* _PORT_S = "7071";

constexpr int _MAX_THREADS = 10;

constexpr int _RECV_BUF_LENGTH = 512;
constexpr int _MSG_HEADER = 16;
constexpr int _MSG_BODY = _RECV_BUF_LENGTH - _MSG_HEADER;

constexpr char const _ADD_EVENT_MSG[_MSG_HEADER] = "AddEvent";
constexpr char const _LOGIN_USER[_MSG_HEADER] = "LoginUser";

}; // namespace ste

#endif