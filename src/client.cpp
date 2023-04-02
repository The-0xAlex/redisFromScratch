#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <unistd.h>

static void efail(const char *msg) {
  int err = errno;
  fprintf(stderr, "[%d] %s\n", err, msg);
  abort();
}

int main() {
  // TCP/IPv4 socket
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    efail("socket()");
  }

  struct sockaddr_in addr = {};
  addr.sin_family = AF_INET;
  addr.sin_port = ntohs(1234);
  // 127.0.0.1
  addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);

  if (connect(fd, (const struct sockaddr *)&addr, sizeof(addr)) < 0) {
    efail("connect");
  }

  char msg[] = "Hello client.";
  write(fd, msg, strlen(msg));

  char rbuf[64] = {};
  if (read(fd, rbuf, sizeof(rbuf) - 1) < 0) {
    efail("read");
  }
  printf("server says: %s\n", rbuf);

  close(fd);
  return 0;
}
