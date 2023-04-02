#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

static void elog(const char *msg) { fprintf(stderr, "%s\n", msg); }

static void efail(const char *msg) {
  int err = errno;
  fprintf(stderr, "[%d] %s\n", err, msg);
  abort();
}

static void do_something(int fd) {
  char rbuf[64] = {};
  if (read(fd, rbuf, sizeof(rbuf) - 1) < 0)
    efail("read");

  printf("client says: %s\n", rbuf);

  char msg[] = "Hello server.";
  write(fd, msg, strlen(msg));
}

int main(int argc, const char **argv) {
  // TCP/IPv4 socket
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0)
    efail("socket()");

  int val = 1;
  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

  struct sockaddr_in addr = {};
  addr.sin_family = AF_INET;
  addr.sin_port = ntohs(1234);
  // wildcard address
  addr.sin_addr.s_addr = ntohl(0);

  if (bind(fd, (const struct sockaddr *)&addr, sizeof(addr)) < 0)
    efail("connect");

  if (listen(fd, SOMAXCONN) < 0)
    efail("listen");

  while (true) {
    struct sockaddr_in client_addr = {};
    socklen_t client_addr_len = sizeof(client_addr);

    // accept
    int client_fd =
        accept(fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd < 0)
      continue;

    do_something(client_fd);
    close(client_fd);
  }

  return 0;
}
