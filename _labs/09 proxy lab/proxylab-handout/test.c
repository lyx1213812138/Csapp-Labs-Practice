#include "csapp.h"

int main(int argc, char **argv) {
  char host[MAXLINE], url[MAXLINE] = "hello";
  strncpy(host, "hello", 3);
  printf("%s\n", host);
  return 0;
}