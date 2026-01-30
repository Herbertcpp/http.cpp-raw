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
    "Content-length: " + std::to_string(buffer_HTML.size()) + "\r\n"
    "\r\n";

    send(clientSock_FD, rootReturnMessage.data(), rootReturnMessage.size(), 0);
    send(clientSock_FD, buffer_HTML.data(), buffer_HTML.size(), 0);

    std::cout << "Sent homepage to client\n";

    close(clientSock_FD);
}

void handleJSON() {

}

void handleHTML() {

}

void handleParsedHTTP(std::vector<std::string> &header, int &clientSock_FD);

void parseHTTP(char* data, int& clientSock_FD) {
  const char* req = data;

  std::string headers[10]{};

  int currentHeader = 0;

  while(*data) {

    headers[currentHeader] += *data;
    ++data;

    if(*data == '\r') {
        currentHeader++;
    }

  }

  std::istringstream header(headers[0]);
  std::vector<std::string> words;
  std::string temp;

  while(header >> temp) {
      words.push_back(temp);
  }

  std::string method = words[0];

  if (method == "GET") {
      handleParsedHTTP(words, clientSock_FD);
      std::cout << "Calling Get method";
  } else {
      std::cout << "not a get method";
  }
}

void handleParsedHTTP(std::vector<std::string> &header, int &clientSock_FD) {

   std::unordered_map<std::string, std::function<void(int)>> mapFunctions;

   mapFunctions["/"] = [](int sock){ root(sock); };

   std::string path = header[1];

   auto it = mapFunctions.find(path);

   if(it != mapFunctions.end()) {
       it->second(clientSock_FD);
   } else {
       std::cout << "Couldn't find requested Function";
   }

}

int main() {
  using namespace std;


  int handle = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(8080);
  inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

  bind(handle, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

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
    parseHTTP(buffer, clientSocket);
  }

  close(handle);

  return 0;
}
