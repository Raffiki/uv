#include <stdio.h>
#include "uv.h"

uv_loop_t *loop;
const char *command;

void run_command(uv_fs_event_t *handle, const char *filename, int events, int status) {
    fprintf(stderr, "Change detected in %s: ", handle->filename);
    if (events == UV_RENAME)
        fprintf(stderr, "renamed");
    if (events == UV_CHANGE)
        fprintf(stderr, "changed");

    fprintf(stderr, " %s\n", filename ? filename : "");
    system(command);
}

int main(int argc, char **argv) {

    loop = uv_default_loop();
    command = argv[1];
    while (argc-- >2) {

        fprintf(stderr, "adding watch on %s\n", argv[argc]);
        uv_fs_event_init(loop, (uv_fs_event_t*) malloc(sizeof(uv_fs_event_t)), argv[argc], run_command, 0);
    }
    uv_run(loop, UV_RUN_DEFAULT);
    return 0;
}
