#include <iostream>
#include <unordered_map>
#include <vector>
#include <functional>
#include <sstream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void root() {
    std::cout << "Welcome to the fucking server\n";
}

void handleParsedHTTP(std::vector<std::string> &header);

void parseHTTP(char* data) {
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
      handleParsedHTTP(words);
      std::cout << "Calling Get method";
  } else {
      std::cout << "not a get method";
  }
}

void handleParsedHTTP(std::vector<std::string> &header) {

   std::unordered_map<std::string, std::function<void(void)>> mapFunctions;

   mapFunctions["/"] = root;

   std::string path = header[1];

   auto it = mapFunctions.find(path);

   if(it != mapFunctions.end()) {
       it->second();
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

  while (1) {
  int clientSocket = accept(handle, nullptr, nullptr);

  char buffer[4096] = {0};
  char temp[128] = {0};

  recv(clientSocket, buffer, sizeof(buffer), 0);
    parseHTTP(buffer);
  }

  close(handle);

  return 0;
}
