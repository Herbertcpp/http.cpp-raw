#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <functional>
#include <sstream>
#include <fstream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void root(int &clientSock_FD) {

    std::ifstream readFile("./root.html", std::ios::binary);
    std::string buffer_HTML;
    buffer_HTML.resize(std::filesystem::file_size("./root.html"));
    readFile.read(buffer_HTML.data(), buffer_HTML.size());

    std::string rootReturnMessage =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html; charset=UTF-8\r\n"
    "Content-Length: " + std::to_string(buffer_HTML.size()) + "\r\n"
    "\r\n";

    send(clientSock_FD, rootReturnMessage.data(), rootReturnMessage.size(), 0);
    send(clientSock_FD, buffer_HTML.data(), buffer_HTML.size(), 0);

    std::cout << "Sent homepage to client\n";

    close(clientSock_FD);
}

void sendFavIcon(int &clientSock_FD) {


    std::ifstream favIcon("./favicon.ico", std::ios::binary);
    std::string buffer{};
    buffer.resize(std::filesystem::file_size("./favicon.ico"));

    favIcon.read(buffer.data(), buffer.size());

    std::string req =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: image/x-icon\r\n"
    "Content-Length: " + std::to_string(buffer.size()) + "\r\n"
    "\r\n";

    send(clientSock_FD, req.data(), req.size(), 0);
    send(clientSock_FD, buffer.data(), buffer.size(), 0);

    std::cout << "Sent favicon to client\n";

    close(clientSock_FD);
}

void handleJSON() {

}

void handleHTML() {

}

void handleRequest(const char* data, int &clientSock_FD) {
    std::istringstream header(data);
    std::string method, path, version;

    if(!(header >> method >> path >> version)) {
        std::cout << "Error parsing request";
        return;
    }

   static std::unordered_map<std::string, std::function<void(int)>> mapFunctions {

       {"/", [](int x){root(x);}},
       {"/favicon.ico", [] (int x) {sendFavIcon(x);}}
   };

   auto it = mapFunctions.find(path);

   if(it != mapFunctions.end()) {
       it->second(clientSock_FD);
   } else {
       std::cout << "Couldn't find requested Function: " << path << "\n";
   }

}

int main() {
  using namespace std;


  int handle = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(8080);
  inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

  if(bind(handle, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) > 0) {
      std::cout << "Error binding";
  };

  listen(handle, 10);

  std::string httpRequest =
    "GET / HTTP/1.1\r\n"
    "Host: herbert.com\r\n"
    "Connection: close\r\n"
    "\r\n";

  std::cout << "Waiting for client...\n";
  while (1) {
  int clientSocket = accept(handle, nullptr, nullptr);

  std::cout << "Client connecting...\n";

  char buffer[4096] = {0};
  char temp[128] = {0};

  recv(clientSocket, buffer, sizeof(buffer), 0);
    handleRequest(buffer, clientSocket);
  }

  close(handle);

  return 0;
}
