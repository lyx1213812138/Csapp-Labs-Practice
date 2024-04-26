#include "csapp.h"

void listen_request(char *port);
void send_request();

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
	  exit(1);
  }

  listen_request(argv[1]);

  send_request();
  
  return 0;
}