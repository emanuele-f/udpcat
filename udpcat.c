/*
 * Simple UDP program which outputs the received UDP data.
 *
 * This is the equivalent of:
 *  socat -b 65535 - udp4-listen:1234,reuseaddr
 */

// gcc -Wall -o udpcat udpcat.c

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFSIZE 65535

int main(int argc, char **argv) {
  int sock, port;
  int optval;
  struct sockaddr_in addr;
  char pktbuf[BUFSIZE];

  if(argc != 2) {
    fprintf(stderr, "Usage: %s <port>\n", argv[0]);
    return(1);
  }

  port = atoi(argv[1]);

  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if(sock < 0) {
    perror("SOCK_DGRAM open() failed");
    return(1);
  }

  optval = 1;
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int));

  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(port);

  if(bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
    perror("SOCK_DGRAM bind() failed");
    return(1);
  }

  while(1) {
    int len, sent;

    if((len = recv(sock, pktbuf, sizeof(pktbuf), 0)) < 0) {
      perror("recv() failed");
      return(1);
    }

    if((sent = write(STDOUT_FILENO, pktbuf, len)) != len) {
      perror("write() failed");
      return(1);
    }
  }

  close(sock);
  return(0);
}
