#include "termbox2.h"
#include "wlfchatutils.h"
#include <stdlib.h>
#include <pwd.h>

#define TEXT_BUFSIZ 64
#define HISTORY_BUFSIZ 256
#define INPUT_BG 0xf1
#define INPUT_FG 226

int render_string(int x, int y, int max_w, uint32_t fg, uint32_t bg, const void *str, int is_wide) {
    int xOffset = 0;
    for (int j = 0; x + xOffset < max_w; j++) {
        uint32_t ch = is_wide ? ((const uint32_t*)str)[j] : (uint32_t)((const char*)str)[j];
        if (ch == 0) break;
        tb_set_cell(x + xOffset++, y, ch, fg, bg);
    }
    return xOffset;
}

int main(int argc, char **argv) {
    uid_t uid = geteuid(); // Get the effective user ID
    struct passwd *pw = getpwuid(uid);
    tb_init();
    tb_set_output_mode(TB_OUTPUT_256);
    tb_set_clear_attrs(TB_DEFAULT,  236);
    int w = tb_width();
    int h = tb_height();
    uint32_t buf[TEXT_BUFSIZ];
    uint16_t historyBufCount = 0;
    buf[0] = 0;
    uint32_t* historyBuf = malloc(sizeof(wchar_t) * TEXT_BUFSIZ * HISTORY_BUFSIZ);
    for (int i = 0; i < HISTORY_BUFSIZ; i++) historyBuf[0] = 0;
    int c = 0;
    struct tb_event ev;

    do {
        //Handle events
        if (ev.type == TB_EVENT_RESIZE) {
            w = tb_width();
            h = tb_height();
            buf[w - 2] = 0;
        }
        if (ev.type == TB_EVENT_KEY && ev.key == 0 && c < TEXT_BUFSIZ - 1 && c < w - 2) {
            buf[c++] = ev.ch;
            buf[c] = 0;
        }
        if (ev.type == TB_EVENT_KEY && (ev.key == TB_KEY_BACKSPACE || ev.key == TB_KEY_BACKSPACE2) && c > 0) {
            buf[--c] = 0;
        }
        if (ev.type == TB_EVENT_KEY && ev.key == TB_KEY_ENTER && c > 0) {
            for (int i = 0; i < TEXT_BUFSIZ; i++) historyBuf[historyBufCount * TEXT_BUFSIZ + i] = buf[i];
            historyBufCount++;
            c = 0;
            buf[0] = 0;
        }
        tb_clear();
        //Print layout
        printBox(1, 1, w - 2, h - 2, TB_HI_BLACK);
        printBox(1, h - 2, w - 2, h - 2, INPUT_BG);
        //Print cursor and text buffer
        tb_set_cursor(1 + c, h - 2);
        for (int i = 0; i < TEXT_BUFSIZ && buf[i]; i++) {
            tb_set_cell(1 + i, h - 2, buf[i], INPUT_FG, INPUT_BG);
        }
        //Print history buffer
        for (int i = historyBufCount - 1, yOffset = 0; i >= 0; i--) {
            if (historyBuf[i * TEXT_BUFSIZ] != 0) {
                int xOffset = 1;
                xOffset += render_string(xOffset, h - 3 - yOffset, w - 3, INPUT_FG - 32, TB_HI_BLACK, pw->pw_name, 0);
                if (xOffset < w - 3) {
                    tb_set_cell(xOffset++, h - 3 - yOffset, ':', INPUT_FG - 32, TB_HI_BLACK);
                    xOffset++;
                    render_string(xOffset, h - 3 - yOffset, w - 3, INPUT_FG, TB_HI_BLACK, &historyBuf[i * TEXT_BUFSIZ], 1);
                }
                yOffset++;
            }
        }
        tb_present();
    } while (tb_poll_event(&ev) == 0);

    free(historyBuf);
    return 0;
}