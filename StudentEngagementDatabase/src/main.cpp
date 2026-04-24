#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <thread>
#include <forward_list>
#include <mutex>
#include <vector>
#include <semaphore>


void PrintError(std::string msg);
void PrintErrorN(std::string msg);
void PrintErrorN(std::string msg, int n);

void Cleanup();
BOOL WINAPI CtrlHandler(DWORD fdwCtrlType);

void HandleRequest(SOCKET client, std::shared_ptr<std::binary_semaphore> semaphore);



constexpr int _PORT = 7071;
constexpr char const* _PORT_S = "7071";

constexpr int _MAX_THREADS = 10;

constexpr int _RECV_BUF_LENGTH = 512;
constexpr int _MSG_HEADER = 16;
constexpr int _MSG_BODY = _RECV_BUF_LENGTH - _MSG_HEADER;

constexpr char const _ADD_EVENT_MSG[_MSG_HEADER] = "AddEvent";



bool running = true;
std::mutex writeDataMutex;


int main() {
  std::atexit(Cleanup);

  WSAData wsaData;
  int result = WSAStartup(MAKEWORD(2,2), &wsaData);
  if (result != 0) {
    PrintErrorN("Failed to start WSA", result);
  }


  addrinfo hints;
  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  addrinfo* addr = nullptr;

  result = getaddrinfo("localhost", _PORT_S, &hints, &addr);
  if (result != 0 || addr == nullptr) {
    PrintErrorN("Failed to find localhost address");
    return 1;
  }


  SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock == INVALID_SOCKET) {
    PrintError("Failed to start socket");
    return 1;
  }

  u_long mode = 1;
  ioctlsocket(sock, FIONBIO, &mode);

  DWORD timeout = 250;
  setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));


  result = bind(sock, addr->ai_addr, addr->ai_addrlen);
  if (result != 0) {
    PrintErrorN("Failed to bind socket");
    closesocket(sock);
    return 1;
  }
  freeaddrinfo(addr);

  result = listen(sock, 10);
  if (result != 0) {
    PrintErrorN("Failed to put socket in listen state");
    closesocket(sock);
    return 1;
  }

  SetConsoleCtrlHandler(CtrlHandler, true);


  sockaddr clientAddr;
  int length = sizeof(sockaddr);


  std::thread activeThreads[_MAX_THREADS];
  std::vector<std::shared_ptr<std::binary_semaphore>> semaphores;
  for (int i = 0; i < _MAX_THREADS; i++) {
    semaphores.emplace_back(std::make_shared<std::binary_semaphore>(ptrdiff_t(1)));
  }


  while (running) {
    SOCKET client = accept(sock, &clientAddr, &length);
    if (client == INVALID_SOCKET) {
      continue;
    }

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
    if (!started) {
      activeThreads[0].join();
      activeThreads[0] = std::thread(HandleRequest, client, semaphores[0]);
    }
  }
  for (int i = 0; i < _MAX_THREADS; i++) {
    activeThreads[i].join();
  }

  closesocket(sock);

  std::cout << "Closing\n";

  return 0;
}




void HandleRequest(SOCKET client, std::shared_ptr<std::binary_semaphore> semaphore) {
  char recvBuf[_RECV_BUF_LENGTH];
  memset(recvBuf, 0, _RECV_BUF_LENGTH);

  int result = recv(client, recvBuf, _RECV_BUF_LENGTH, 0);
  if (result == SOCKET_ERROR) {
    closesocket(client);
    semaphore->release();
    return;
  }

  if (memcmp(recvBuf, _ADD_EVENT_MSG, _MSG_HEADER) == 0) {
    std::cout << "Add item some how\n";
  }


  closesocket(client);
  semaphore->release();
}






void PrintError(std::string msg) {
  std::cerr << "[ERROR] : " << msg << '\n';
}
void PrintErrorN(std::string msg) {
  std::cerr << "[ERROR (" << WSAGetLastError() << ")] : " << msg << '\n';
}
void PrintErrorN(std::string msg, int n) {
  std::cerr << "[ERROR (" << n << ")] : " << msg << '\n';
}

void Cleanup() {
  WSACleanup();
}

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType) {
  if (fdwCtrlType == CTRL_C_EVENT) {
    running = false;
    return true;
  }
  return false;
}