#include "server.h"
#include "constants.h"
#include "utility.h"
#include "models.h"
#include "database.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <semaphore>
#include <atomic>

using namespace ::ste;
using namespace ::ste::models;

namespace {

void PrintErrorN(std::string msg);
void PrintErrorN(std::string msg, int n);

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType);

void HandleRequest(SOCKET client, std::shared_ptr<std::binary_semaphore> semaphore);



std::atomic<bool> running(true);
std::mutex writeDataMutex;
SOCKET sock;

}; // namespace


namespace ste {
namespace server {

bool Start() {

  // Start WSA
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

  // Start socket
  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock == INVALID_SOCKET) {
    PrintError("Failed to start socket");
    return false;
  }

  // Nonblocking mode
  u_long mode = 1;
  ioctlsocket(sock, FIONBIO, &mode);

  // Will spend max 250ms before dropping a client
  DWORD timeout = 250;
  setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

  // Bind to address
  result = bind(sock, addr->ai_addr, addr->ai_addrlen);
  if (result != 0) {
    PrintErrorN("Failed to bind socket");
    closesocket(sock);
    return false;
  }
  freeaddrinfo(addr);


  // Start listening for clients, max 10 in queue
  result = listen(sock, 10);
  if (result != 0) {
    PrintErrorN("Failed to put socket in listen state");
    closesocket(sock);
    return false;
  }

  // Stop ctrl-c from force closing
  SetConsoleCtrlHandler(CtrlHandler, true);

  return true;
}





void Run() {
  
  sockaddr clientAddr;
  int length = sizeof(sockaddr);

  // Get threads ready
  std::thread activeThreads[_MAX_THREADS];
  std::vector<std::shared_ptr<std::binary_semaphore>> semaphores;
  for (int i = 0; i < _MAX_THREADS; i++) {
    semaphores.emplace_back(std::make_shared<std::binary_semaphore>(ptrdiff_t(1)));
  }


  while (running) {
    // Accept a client or continues if none
    SOCKET client = accept(sock, &clientAddr, &length);
    if (client == INVALID_SOCKET) {
      continue;
    }

    // Find first thread that finished
    bool started = false;
    for (int i = 0; i < _MAX_THREADS; i++) {
      if (semaphores[i]->try_acquire()) {
        if (activeThreads[i].joinable())
          activeThreads[i].join();

        semaphores[i]->release();
        activeThreads[i] = std::thread(HandleRequest, client, semaphores[i]);
        started = true;
      }
    }

    // Or just wait for the first thread to finish up
    if (!started) {
      activeThreads[0].join();
      activeThreads[0] = std::thread(HandleRequest, client, semaphores[0]);
    }
  }

  // Brings threads back in
  for (int i = 0; i < _MAX_THREADS; i++) {
    if (activeThreads[i].joinable())
      activeThreads[i].join();
  }
}





void End() {
  closesocket(sock);
  WSACleanup();
}

}; // namespace server
}; // namespace ste






namespace {

void HandleRequest(SOCKET client, std::shared_ptr<std::binary_semaphore> semaphore) {
  // Get receive buffer ready
  char recvBuf[_RECV_BUF_LENGTH];
  memset(recvBuf, 0, _RECV_BUF_LENGTH);


  // Read message
  int result = recv(client, recvBuf, _RECV_BUF_LENGTH, 0);
  if (result == SOCKET_ERROR) {
    // No message
    closesocket(client);
    semaphore->release();
    return;
  }

  // Start comparing to all known messages
  if (memcmp(recvBuf, _ADD_EVENT_MSG, _MSG_HEADER) == 0) {
    Event* event = (Event*)(recvBuf + _MSG_HEADER);
    database::AddEvent(*event);
  }


  // Close socket
  closesocket(client);
  semaphore->release();
}



void PrintErrorN(std::string msg) {
  std::cerr << "[ERROR (" << WSAGetLastError() << ")] : " << msg << '\n';
}
void PrintErrorN(std::string msg, int n) {
  std::cerr << "[ERROR (" << n << ")] : " << msg << '\n';
}


BOOL WINAPI CtrlHandler(DWORD fdwCtrlType) {

  // Makes shutdown go through proper steps
  if (fdwCtrlType == CTRL_C_EVENT) {
    running = false;
    return true;
  }
  return false;
}

}; // namespace