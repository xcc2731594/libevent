#include <event.h>
#include <evhttp.h>
#include "event2/http.h"
#include "event2/http_struct.h"
#include "event2/event.h"
#include "event2/buffer.h"
#include "event2/buffer_compat.h"
#include "stdlib.h"
#include "stdio.h"
#include <iostream>

void callback_s(struct evhttp_request* request,void *arg)
{
    const struct evhttp_uri *evhttp_uri = evhttp_request_get_evhttp_uri(request);
    char *post_data = (char *)EVBUFFER_DATA(request->input_buffer);
    const char *path = evhttp_uri_get_path(evhttp_uri);
    const char *query = evhttp_uri_get_query(evhttp_uri);
    printf("path:%s\n",path);
    struct evbuffer *evbuf=evbuffer_new();
    evbuffer_add_printf(evbuf,"path is [%s] and post_body=[%s]\n",path,post_data);
    evhttp_send_reply(request,HTTP_OK,"OK",evbuf);
    evbuffer_free(evbuf);
}

void callback_gen(struct evhttp_request *request,void *arg)
{
    printf("callback_gen");
}

int main(int argc,char **argv)
{
    std::cout<<argv[1]<<std::endl;
    int port = atoi(argv[1]);
    struct event_base *base = event_base_new();
    struct evhttp *http = evhttp_new(base);
    evhttp_bind_socket(http,"0.0.0.0",port);
    // evhttp_set_gencb(http,callback_gen,NULL);
    evhttp_set_cb(http,"/this",callback_s,NULL);
    event_base_dispatch(base);
    event_base_free(base);
    return 0;
}