#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
using std::string;

string get_html(char *req_str) {
 string path;
 
 int space_cnt = 0;
 for (int i = 0; req_str[i] != 0 && space_cnt < 2; i++) {
  if (space_cnt == 1 && req_str[i] != ' ') {
   path += req_str[i];
  }
  
  space_cnt += req_str[i] == ' ';
 }

 path = "www" + path + "index.html";
 std::cout << path << '\n';

 std::stringstream html;
 std::ifstream html_file(path);

 if (html_file.is_open()) {
  html << html_file.rdbuf();
 } else {
  html << "404 File Not Found\n";
 }

 return html.str();
}

int main() {
 addrinfo *info, hints = {
  .ai_flags    = AI_PASSIVE,
  .ai_family   = AF_UNSPEC,
  .ai_socktype = SOCK_STREAM,
 };
 
 int status = getaddrinfo("127.0.0.1", "3490", &hints, &info);
 if (status) {
  std::cerr << "addrinfo error: " << gai_strerror(status) << std::endl;
  return 1;
 }

 int sockfd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);

 char yes = 1;
 setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, 1);

 char* req = new char[1001];
 string res;
 int client_fd;

 while (1) {
  bind(sockfd, info->ai_addr, info->ai_addrlen);
  listen(sockfd, 20);
 
  sockaddr_storage client_addr;
  socklen_t addr_size = sizeof client_addr;
  client_fd = accept(sockfd, (sockaddr *)&client_addr, &addr_size);
 
  recv(client_fd, req, 1000, 0);
 
  res += "HTTP/1.1 200 OK\r\n\r\n";
  res += get_html(req);
  
  send(client_fd, res.c_str(), res.length(), 0);

  res.clear();
  close(client_fd);
 }
 
 close(sockfd);
 freeaddrinfo(info);
}