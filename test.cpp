#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  //for strcat
#include <unistd.h>  //for getopt, fork
// for struct evkeyvalq
#include <event.h>
#include <sys/queue.h>
// for http
//#include <evhttp.h>
#include <event2/http.h>
#include <event2/http_compat.h>
#include <event2/http_struct.h>
#include <event2/util.h>
#include <signal.h>
//  #define httpd_option_listen "0.0.0.0"
#define MYHTTPD_SIGNATURE "myhttpd v 0.0.1"
//处理模块
void httpd_handler(struct evhttp_request *req, void *arg) {
  char output[2048] = "\0";
  char tmp[1024];
  struct evbuffer *input = evhttp_request_get_input_buffer(req);
  int len = evbuffer_get_length(input);
  printf("pthread [%ld]\n", pthread_self());
  printf("evbuffer_get_length len = %d\n", len);
  printf("evhttp_find_header Content-Length = %s\n",
         evhttp_find_header(req->input_headers, "Content-Length"));
  printf("Content-Type = %s\n",
         evhttp_find_header(req->input_headers, "Content-Type"));
 
  //获取客户端请求的URI(使用evhttp_request_uri或直接req->uri)
  const char *uri;
//   uri = evhttp_request_uri(req);
  uri = req->uri;
  sprintf(tmp, "uri=%s\n", uri);
  strcat(output, tmp);
  sprintf(tmp, "uri=%s\n", req->uri);
  strcat(output, tmp);
  // decoded uri
  char *decoded_uri;
  decoded_uri = evhttp_decode_uri(uri);
  sprintf(tmp, "decoded_uri=%s\n", decoded_uri);
  strcat(output, tmp);
  //解析URI的参数(即GET方法的参数)
  struct evkeyvalq params;
  //将URL数据封装成key-value格式,q=value1, s=value2
  evhttp_parse_query(decoded_uri, &params);
  //得到q所对应的value
  sprintf(tmp, "Action=%s\n", evhttp_find_header(&params, "Action"));
  strcat(output, tmp);
  //得到s所对应的value
  sprintf(tmp, "s=%s\n", evhttp_find_header(&params, "s"));
  strcat(output, tmp);
  free(decoded_uri);
  //获取POST方法的数据
  char *post_data = (char *) EVBUFFER_DATA(req->input_buffer);
  printf("post data = %s\n", post_data);
  printf("output = %s\n", output);
  /*
  具体的：可以根据GET/POST的参数执行相应操作，然后将结果输出
  ...
  */
  /* 输出到客户端 */
  // HTTP header
  evhttp_add_header(req->output_headers, "Server", MYHTTPD_SIGNATURE);
  evhttp_add_header(req->output_headers, "Content-Type",
                    "text/plain; charset=UTF-8");
  evhttp_add_header(req->output_headers, "Connection", "close");
  evhttp_add_header(req->output_headers, "User-Agent", "curl");
  //输出的内容
  struct evbuffer *buf;
  buf = evbuffer_new();
  char temp[] = "testing";
  evbuffer_add_printf(buf, "It works!\n%s\n", temp);
  evhttp_send_reply(req, HTTP_OK, "OK", buf);
  evbuffer_free(buf);
}
 
//当向进程发出SIGTERM/SIGHUP/SIGINT/SIGQUIT的时候，终止event的事件侦听循环
void signal_handler(int sig) {
  switch (sig) {
    case SIGTERM:
    case SIGHUP:
    case SIGQUIT:
    case SIGINT:
        exit(-1);
      break;
  }
}
 
int http_init(int port) {
  int nfd = socket(AF_INET, SOCK_STREAM, 0);
  if (nfd < 0) {
    return -1;
  }
 
  const int one = 1;
  setsockopt(nfd, SOL_SOCKET, SO_REUSEADDR, (char *) &one, sizeof(int));
 
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(port);
 
  if (bind(nfd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
    return -1;
  }
 
  if (listen(nfd, 128) < 0) {
    return -1;
  }
 
  int flags = 0;
  if ((flags = fcntl(nfd, F_GETFL, 0)) < 0 ||
      fcntl(nfd, F_SETFL, flags | O_NONBLOCK) < 0) {
    return -1;
  }
 
  return nfd;
}
 
void *worker_func(void *arg) {
  printf("start pthread [%ld]\n", pthread_self());
  event_base_dispatch((struct event_base *) arg);
  event_base_free((struct event_base *) arg);
}
 
#define MAX_WORK_PTHREAD 3
 
int main(int argc, char *argv[]) {
  //默认参数
  char httpd_option_listen[] = "0.0.0.0";
  int httpd_option_port = 8080;
  int httpd_option_timeout = 120;  // in seconds
 
  pthread_t thread_list[MAX_WORK_PTHREAD];
  struct event_base *bases[MAX_WORK_PTHREAD];
  struct evhttp *httpds[MAX_WORK_PTHREAD];
 
  signal(SIGHUP, signal_handler);
  signal(SIGTERM, signal_handler);
  signal(SIGINT, signal_handler);
  signal(SIGQUIT, signal_handler);
  int fd = http_init(httpd_option_port);
  if (fd < 0) {
    return -1;
  }
 
  for (int i = 0; i < MAX_WORK_PTHREAD; i++) {
    bases[i] = event_init();
    httpds[i] = evhttp_new(bases[i]);
    if (evhttp_accept_socket(httpds[i], fd) != 0) {
      return -1;
    }
    evhttp_set_timeout(httpds[i], httpd_option_timeout);
    evhttp_set_gencb(httpds[i], httpd_handler, NULL);
    if (pthread_create(&thread_list[i], NULL, worker_func, (void *) bases[i]) !=
        0) {
      return -1;
    }

    
  }
  for (int i = 0; i < MAX_WORK_PTHREAD; ++i) {
    pthread_join(thread_list[i], NULL);
    // pthread_detach(thread_list[i]);
  }
 
  for (int i = 0; i < MAX_WORK_PTHREAD; i++) {
    event_base_free(bases[i]);
    evhttp_free(httpds[i]);
  }
  return 0;
}