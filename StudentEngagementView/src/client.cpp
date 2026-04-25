#include "client.h"
#include "constants.h"
#include "utility.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>


using namespace ::ste::models;

namespace {

void PrintErrorN(std::string msg);
void PrintErrorN(std::string msg, int n);

sockaddr serverAddr;
SOCKET sock;


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
  closesocket(sock);
  WSACleanup();
}




void AddEvent(const Event& val) {
  // Start socket
  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock == INVALID_SOCKET) {
    PrintError("Failed to start socket");
    return;
  }


  int result = connect(sock, &serverAddr, sizeof(serverAddr));
  if (result == SOCKET_ERROR) {
    PrintErrorN("Failed to connect to server");
    return;
  }
  char sendBuf[_MSG_HEADER + sizeof(Event)];

  memcpy(sendBuf, _ADD_EVENT_MSG, _MSG_HEADER);
  memcpy(sendBuf + _MSG_HEADER, &val, sizeof(Event));

  result = send(sock, sendBuf, _MSG_HEADER + sizeof(Event), 0);
  if (result == SOCKET_ERROR) {
    PrintErrorN("Failed to send AddEvent to server");
  }

  shutdown(sock, SD_BOTH);
  closesocket(sock);
}

void AddResource(const Resource& val) {

}

void GiveUserBadge(int user, int badge) {

}

void AddStudentToEvent(int student, int event) {

}


}; // namespace client
}; // namespace ste






namespace {

void PrintErrorN(std::string msg) {
  std::cerr << "[ERROR (" << WSAGetLastError() << ")] : " << msg << '\n';
}
void PrintErrorN(std::string msg, int n) {
  std::cerr << "[ERROR (" << n << ")] : " << msg << '\n';
}

}; // namespace