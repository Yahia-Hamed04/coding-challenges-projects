#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

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

 int yes = 1;
 setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);

 bind(sockfd, info->ai_addr, info->ai_addrlen);
 listen(sockfd, 20);

 sockaddr_storage client_addr;
 socklen_t addr_size = sizeof client_addr;
 int client_fd = accept(sockfd, (sockaddr *)&client_addr, &addr_size);

 char* nil = new char[1001];
 recv(client_fd, nil, 1000, 0);

 char* msg = new char[1000 + sizeof("HTTP/1.1 200 OK\r\n\r\n")];
 sprintf(msg, "HTTP/1.1 200 OK\r\n\r\n%s", nil);
 send(client_fd, msg, strlen(msg), 0);
 
 close(client_fd);
 close(sockfd);
 freeaddrinfo(info);
}