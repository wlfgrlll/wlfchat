#include <termbox2.h>
#include "event_handler.h"
#include "rendering.h"
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>


int main(int argc, char **argv) {
    //Get username
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    const char *username = pw ? pw->pw_name : "???";

    //Initialize termbox2
    int res = tb_init();
    if (res < 0) {
        fprintf(stderr, "Failed to initialize TUI with error code %d\n", res);
        return 1;
    }
    tb_set_output_mode(TB_OUTPUT_256); //TODO: Don't assume terminal supports 256 colors
    tb_set_clear_attrs(TB_DEFAULT,  236);
    int w = tb_width();
    int h = tb_height();

    //Declare buffers
    uint32_t buf[TEXT_BUFSIZ]; //Input text buffer
    int c = 0; //Current cursor position
    uint32_t* historyBuf; //Chat history buffer
    uint16_t historyBufCount = 0; //Chat history buffer message count

    //Initialize buffers
    buf[0] = 0;
    historyBuf = calloc(TEXT_BUFSIZ * HISTORY_BUFSIZ, sizeof(uint32_t));
    if (historyBuf == NULL) {
        tb_shutdown();
        return 1;
    }

    //Main event loop
    int ttyfd, resizefd;
    tb_get_fds(&ttyfd, &resizefd);

    if (ttyfd != STDIN_FILENO) {
        int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    }

    while (1) {
        render_ui(w, h, c, buf, historyBuf, historyBufCount, username);

        struct tb_event ev;
        int rc = tb_peek_event(&ev, 20);
        if (rc < 0 && rc != TB_ERR_NO_EVENT && rc != -4) break;
        if (rc >= 0) {
            if (ev.type == TB_EVENT_KEY) {
                if (ev.key == TB_KEY_CTRL_C) break;
                handle_key_event(&ev, buf, &c, w, historyBuf, &historyBufCount, h);
            } else if (ev.type == TB_EVENT_RESIZE) {
                handle_resize_event(&w, &h, buf);
            }
        }
    }

    free(historyBuf);
    return 0;
}