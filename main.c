#include "termbox2.h"
#include "wlfchatutils.h"

#define TEXT_BUFSIZ 64
#define INPUT_BG 0xf1
#define INPUT_FG 226

int main(int argc, char **argv) {
    tb_init();
    tb_set_output_mode(TB_OUTPUT_256);
    tb_set_clear_attrs(TB_DEFAULT,  236);
    int w = tb_width();
    int h = tb_height();
    uint32_t buf[TEXT_BUFSIZ];
    buf[0] = 0;
    int c = 0;
    struct tb_event ev;

    do {
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
        tb_clear();
        printBox(1, 1, w - 2, h - 2, TB_HI_BLACK);
        printBox(1, h - 2, w - 2, h - 2, INPUT_BG);
        tb_set_cell(1, h - 2, ' ', TB_DEFAULT, INPUT_BG);
        tb_set_cursor(1 + c, h - 2);
        for (int i = 0; i < TEXT_BUFSIZ && buf[i]; i++) {
            tb_set_cell(1 + i, h - 2, buf[i], INPUT_FG, INPUT_BG);
        }
        tb_present();
    } while (tb_poll_event(&ev) == 0);
    return 0;
}