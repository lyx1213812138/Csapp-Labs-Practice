#include "csapp.h"

extern char headers[105][MAXLINE], host[MAXLINE], path[MAXLINE], port[MAXLINE];
extern int headers_cnt;

static const char hd_useragent[] = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3",
 hd_conn[] = "Connection: close",
 hd_proxy[] = "Proxy-Connection: close";

void send_request() {
  int send_fd;
  rio_t rio;
  char buf[MAXLINE];
  int if_set_hosthd = 1;

  send_fd = Open_clientfd(host, port);
  Rio_readinitb(&rio, send_fd);

  // sent request line
  sprintf(buf, "GET %s HTTP/1.0\r\n");
  Rio_writen(send_fd, buf, strlen(buf));

  // send headers
  for (int i = 0; i < headers_cnt; i++) {
    char *hd = headers[i];
    if (strstr(hd, "Connection") == NULL 
      && strstr(hd, "User-Agent") == NULL 
      && strstr(hd, "Proxy-Connection") == NULL) {
        Rio_writen(send_fd, hd, strlen(hd));
    }
    if (strstr(hd, "Host") != NULL) if_set_hosthd = 0;
  }

  if (if_set_hosthd) {
    sprintf(buf, "Host: %s\r\n", host);
    Rio_writen(send_fd, buf, strlen(buf));
  }

  Rio_writen(send_fd, hd_useragent, strlen(hd_useragent));
  Rio_writen(send_fd, hd_conn, strlen(hd_conn));
  Rio_writen(send_fd, hd_proxy, strlen(hd_proxy));

}