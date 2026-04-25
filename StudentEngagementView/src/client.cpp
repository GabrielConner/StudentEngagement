#include "client.h"
#include "constants.h"
#include "utility.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>


using namespace ::ste;
using namespace ::ste::models;

namespace {

SOCKET Send(char* data, int dataLen);
void PrintErrorN(std::string msg);
void PrintErrorN(std::string msg, int n);

sockaddr serverAddr;


}; // namespace


namespace ste {
namespace client {

bool Start() {
  WSAData wsaData;
  int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (result != 0) {
    PrintErrorN("Failed to start WSA", result);
    return false;
  }

  // Get address for server
  addrinfo hints;
  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  addrinfo* addr = nullptr;
  result = getaddrinfo("localhost", _PORT_S, &hints, &addr);
  if (result != 0 || addr == nullptr) {
    PrintErrorN("Failed to find localhost address");
    return false;
  }
  serverAddr = *addr->ai_addr;

  freeaddrinfo(addr);

  return true;
}


void End() {
  WSACleanup();
}




void AddEvent(const Event& val) {
  char sendBuf[_MSG_HEADER + sizeof(Event)];
  memcpy(sendBuf, _ADD_EVENT_MSG, _MSG_HEADER);
  memcpy(sendBuf + _MSG_HEADER, &val, sizeof(Event));
  Send(sendBuf, sizeof(sendBuf));
}


void AddResource(const Resource& val) {

}


void GiveUserBadge(int user, int badge) {

}


void AddStudentToEvent(int student, int event) {

}


User LoginUser(char const email[255], char const password[32]) {
  User ret;
  ret.user_id = -1;

  char sendBuf[_MSG_HEADER + 255 + 32];
  memcpy(sendBuf, _LOGIN_USER, _MSG_HEADER);
  memcpy(sendBuf + _MSG_HEADER, email, 30);
  memcpy(sendBuf + _MSG_HEADER + 255, password, 32);
  SOCKET sock = Send(sendBuf, sizeof(sendBuf));
  if (sock == 0) {
    return ret;
  }

  int result = recv(sock, (char*)&ret, sizeof(ret), 0);
  if (result == SOCKET_ERROR || result != sizeof(ret)) {
    PrintErrorN("Failed to receive from server");
    return ret;
  }

  return ret;
}


Student GetStudentAccount(int user_id) {
  Student ret;
  ret.student_id = -1;

  char sendBuf[_MSG_HEADER + sizeof(user_id)];
  memcpy(sendBuf, _GET_STUDENT, _MSG_HEADER);
  memcpy(sendBuf + _MSG_HEADER, (char*)&user_id, sizeof(user_id));

  SOCKET sock = Send(sendBuf, sizeof(sendBuf));
  if (sock == 0) {
    return ret;
  }

  int result = recv(sock, (char*)&ret, sizeof(ret), 0);
  if (result == SOCKET_ERROR || result != sizeof(ret)) {
    PrintErrorN("Failed to receive from server");
    return ret;
  }

  return ret;
}


}; // namespace client
}; // namespace ste






namespace {


SOCKET Send(char* data, int dataLen) {
  // Start socket
  SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock == INVALID_SOCKET) {
    PrintError("Failed to start socket");
    return 0;
  }
  DWORD timeo = 250;
  //setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeo, sizeof(DWORD));

  int result = connect(sock, &serverAddr, sizeof(serverAddr));
  if (result == SOCKET_ERROR) {
    PrintErrorN("Failed to connect to server");
    closesocket(sock);
    return 0;
  }

  result = send(sock, data, dataLen, 0);
  if (result == SOCKET_ERROR) {
    PrintErrorN("Failed to send AddEvent to server");
  } else if (result < dataLen) {
    PrintErrorN("Partial Send");
  }

  return sock;
}


void PrintErrorN(std::string msg) {
  std::cerr << "[ERROR (" << WSAGetLastError() << ")] : " << msg << '\n';
}
void PrintErrorN(std::string msg, int n) {
  std::cerr << "[ERROR (" << n << ")] : " << msg << '\n';
}

}; // namespace