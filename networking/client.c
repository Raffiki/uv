
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "uv.h"


static void write_data(uv_stream_t *dest, size_t size, uv_buf_t buf, uv_write_cb callback);
static void read_stdin(uv_stream_t *stream, ssize_t nread, uv_buf_t buf);
static void on_connect(uv_connect_t* req, int status);


uv_loop_t* loop;
uv_stream_t* stream;
uv_pipe_t stdin_pipe;


typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

uv_buf_t alloc_buffer(uv_handle_t *handle, size_t suggested_size) {
    return uv_buf_init((char*) malloc(suggested_size), suggested_size);
}

void free_write_req(uv_write_t *req) {
    write_req_t *wr = (write_req_t*) req;
    free(wr->buf.base);
    free(wr);
}

void on_socket_write(uv_write_t *req, int status) {
    free_write_req(req);
}

int main(void) {

    loop = uv_default_loop();

    struct sockaddr_in dest = uv_ip4_addr("0.0.0.0", 7000);

    uv_tcp_t socket;
    uv_tcp_init(loop, &socket);

    uv_pipe_init(loop, &stdin_pipe, 0);
    uv_pipe_open(&stdin_pipe, 0);

    uv_connect_t connect;
    uv_tcp_connect(&connect, &socket, dest, on_connect);

    return uv_run(loop, UV_RUN_DEFAULT);
}


static void on_connect(uv_connect_t* connection, int status) {
    assert(status == 0);
    printf("connected.\n");

    stream = connection->handle;

    uv_read_start((uv_stream_t*)&stdin_pipe, alloc_buffer, read_stdin);
}

void read_stdin(uv_stream_t *inputstream, ssize_t nread, uv_buf_t buf) {
    if (nread == -1) {
        if (uv_last_error(loop).code == UV_EOF) {
            uv_close((uv_handle_t*)&socket, NULL);
        }
    }

    write_data(stream, nread, buf, on_socket_write);
}

static void write_data(uv_stream_t *dest, size_t size, uv_buf_t buf, uv_write_cb callback) {
    write_req_t *req = (write_req_t*) malloc(sizeof(write_req_t));
    req->buf = uv_buf_init((char*) malloc(size), size);
    memcpy(req->buf.base, buf.base, size);
    uv_write((uv_write_t*) req, (uv_stream_t*)dest, &req->buf, 1, callback);
}
