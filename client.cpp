#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {

  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(8080);
  serverAddr.sin_addr.s_addr = INADDR_ANY;

  connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

  std::string httpRequest =
    "GET / HTTP/1.1\r\n"
    "Host: herbert.com\r\n"
    "Connection: close\r\n"
    "\r\n";

    send(clientSocket, httpRequest.c_str(), httpRequest.size(), 0);

  close(clientSocket);

  return 0;
}
