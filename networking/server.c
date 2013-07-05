#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "uv.h"

uv_loop_t* loop;
uv_tcp_t* client;


typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

uv_buf_t alloc_buffer(uv_handle_t *handle, size_t suggested_size) {
    return uv_buf_init((char*) malloc(suggested_size), suggested_size);
}

static void fatal(const char *what);
static void on_connection(uv_stream_t* server, int status);
static void on_read(uv_stream_t* stream, ssize_t nread, uv_buf_t buf);
static void write_data(uv_stream_t* dest, size_t size, uv_buf_t buf, uv_write_cb callback);
static void on_write(uv_write_t* req, int status);
static void free_write_req(uv_write_t *req);

int main(void) {

    loop = uv_default_loop();

    uv_tcp_t server;
    uv_tcp_init(loop, &server);

    struct sockaddr_in bind_addr = uv_ip4_addr("0.0.0.0", 7000);
    if (uv_tcp_bind(&server, bind_addr)) {
        fatal("bind");
    }
    if (uv_listen((uv_stream_t*) &server, 128, on_connection)) {
        fatal("listen");
    }

    return uv_run(loop, UV_RUN_DEFAULT);
}



static void on_connection(uv_stream_t* server, int status) {
    assert(status == 0);

    uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop, client);

    if (uv_accept(server, (uv_stream_t*) client)) {
        uv_close((uv_handle_t*) client, NULL);
    }

    uv_read_start((uv_stream_t*) client, alloc_buffer, on_read);
}


static void on_read(uv_stream_t* stream, ssize_t nread, uv_buf_t buf) {

    if (nread == -1) {
        if (uv_last_error(loop).code == UV_EOF) {
            return uv_close((uv_handle_t*) stream, NULL);
        }
    }

    write_data((uv_stream_t*) stream, nread, buf, on_write);
}

static void write_data(uv_stream_t* dest, size_t size, uv_buf_t buf, uv_write_cb callback) {

   // uv_write_t *req = (uv_write_t *) malloc(sizeof(uv_write_t));
   // req->data = (void*) buf.base;
   // buf.len = size;
    write_req_t *req = (write_req_t*) malloc(sizeof(write_req_t));
    req->buf = uv_buf_init((char*) malloc(size), size);
    memcpy(req->buf.base, buf.base, size);

    if (uv_write((uv_write_t*)req, dest, &buf, 1, callback)) {
        fatal("write");
    }
}

static void on_write(uv_write_t* req, int status) {
    free_write_req(req);
}

static void free_write_req(uv_write_t *req) {
    write_req_t *wr = (write_req_t*) req;
    free(wr->buf.base);
    free(wr);
   //char *base = (char*) req->data;
   //free(base);
   //free(req);
}
static void fatal(const char *what)
{
    uv_err_t err = uv_last_error(uv_default_loop());
    fprintf(stderr, "%s: %s\n", what, uv_strerror(err));
    exit(1);
}
