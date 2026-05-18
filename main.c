#include <termbox2.h>
#include "event_handler.h"
#include "rendering.h"
#include <stdlib.h>
#include <pwd.h>


int main(int argc, char **argv) {
    //Get username
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    const char *username = pw ? pw->pw_name : "???";

    //Initialize termbox2
    tb_init();
    tb_set_output_mode(TB_OUTPUT_256);
    tb_set_clear_attrs(TB_DEFAULT,  236);
    int w = tb_width();
    int h = tb_height();
    struct tb_event ev;

    //Declare buffers
    uint32_t buf[TEXT_BUFSIZ]; //Input text buffer
    int c = 0; //Current cursor position
    uint32_t* historyBuf; //Chat history buffer
    uint16_t historyBufCount = 0; //Chat history buffer message count

    //Initialize buffers
    buf[0] = 0;
    historyBuf = calloc(TEXT_BUFSIZ * HISTORY_BUFSIZ, sizeof(wchar_t));
    if (historyBuf == NULL) {
        tb_shutdown();
        return 1;
    }

    //Main event loop
    do {
        //Handle events
        if (ev.type == TB_EVENT_RESIZE) {
            handle_resize_event(&w, &h, buf);
        }
        if (ev.type == TB_EVENT_KEY) {
            handle_key_event(&ev, buf, &c, w, historyBuf, &historyBufCount, h);
        }
        render_ui(w, h, c, buf, historyBuf, historyBufCount, username);
    } while (tb_poll_event(&ev) == 0);

    free(historyBuf);
    return 0;
}