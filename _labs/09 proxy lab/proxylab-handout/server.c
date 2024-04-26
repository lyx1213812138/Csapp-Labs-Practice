#include "csapp.h"

void parse_request(int fd);
void read_headers(rio_t *rp);
void devide_uri(int fd, char *uri, char *host, char *port, char *path);
void response_error(int fd, char *cause, char *errnum, char *msg);
char headers[105][MAXLINE], host[MAXLINE], path[MAXLINE], port[MAXLINE];
int headers_cnt;

void listen_request(char *myport) {
  int listenfd, connectfd;
  char hostname[MAXLINE], port[MAXLINE]; // the hostname and port of client
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;

  listenfd = Open_listenfd(myport);
  while (1) {
    clientlen = sizeof(clientaddr);
    connectfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
    Getnameinfo((SA *)&clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0);
    printf("Accept client connection form %s: %s successfully\n", hostname, port);
    parse_request(connectfd);
    Close(connectfd);
  }
}

void parse_request(int fd) {
  rio_t rio;
  char buf[MAXLINE], method[MAXLINE], http_version[MAXLINE], uri[MAXLINE];

  Rio_readinitb(&rio, fd);
  if (!Rio_readlineb(&rio, buf, MAXLINE)) return;
  printf("%s", buf);

  // divide input string
  sscanf(buf, "%s %s %s", method, uri, http_version);
  if (strcasecmp(method, "GET")) {
    response_error(fd, method, "501", "Cannot implement other method except GET");
    return;
  }
  
  // divide uri into host and path
  devide_uri(fd, uri, host, path, port);
  printf("host: ");
  for (char *p = host; p != path; p++) putchar(*p);
  printf(", path: %s\n", path);

  read_headers(&rio);

}

void read_headers(rio_t *rp) {
  char buf[MAXLINE];
  do {
    Rio_readlineb(rp, buf, MAXLINE);
    printf("a header: %s", buf);
    // TODO: process each header
    if (strcmp(buf, "\r\n")) strcpy(headers[headers_cnt++], buf);
    else break;
  } while (1);
}

void devide_uri(int fd, char *uri, char *host, char *port, char *path) {
  char *protocol_end = strstr(uri, "://");
  if (protocol_end == NULL) {
    response_error(fd, uri, "400", "Request-URI wrong");
    return;
  }
  char *host_begin = protocol_end + 3;
  if (*host_begin == '\0') { // http://\0
    response_error(fd, uri, "400", "Request-URI wrong");
    return;
  }

  char *port_begin = strstr(host_begin, ":");
  char *path_begin = strstr(host_begin, "/");
  if (port_begin == NULL && path_begin == NULL) { // default 80
    strcpy(port, "80");
    strcpy(path, "/");
    strcpy(host, host_begin);
  } else if (port_begin == NULL) { // http://hostname/path
    strcpy(port, "80");
    strncpy(host, host_begin, path_begin - host_begin);
    strcpy(path, path_begin);
  } else if (path_begin == NULL) { // http://hostname:port
    strcpy(path, "/");
    strcpy(port, port_begin + 1);
    if (*port_begin == '\0') strcpy(port, "80");
    strncpy(host, host_begin, port_begin - host_begin);
  } else { // http://hostname:port/path
    strncpy(host, host_begin, port_begin - host_begin);
    strncpy(port, port_begin + 1, path_begin - port_begin - 1);
    strcpy(path, path_begin);
  }
}

void response_error(int fd, char *cause, char *errnum, char *msg) {
  char buf[MAXLINE];

  /* Print the HTTP response headers */
  sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, msg);
  Rio_writen(fd, buf, strlen(buf));
  sprintf(buf, "Content-type: text\r\n\r\n");
  Rio_writen(fd, buf, strlen(buf));

  /* Print the HTTP response body */
  sprintf(buf, "errnum: %s\r\nmsg: %s\r\ncause: %s\r\n\r\n", errnum, msg, cause);
  Rio_writen(fd, buf, strlen(buf));
}