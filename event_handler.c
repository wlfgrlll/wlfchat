#include "event_handler.h"

void add_to_history(uint32_t *message, uint32_t *historyBuf, uint16_t *historyBufCount, int h)
{
    if (*historyBufCount >= HISTORY_BUFSIZ) {
        for (int i = 0; i < (HISTORY_BUFSIZ - 1) * TEXT_BUFSIZ; i++) {
            historyBuf[i] = historyBuf[i + TEXT_BUFSIZ];
        }
        *historyBufCount = HISTORY_BUFSIZ - 1;
        if (*historyBufCount < 0) *historyBufCount = 0;
    }
    for (int i = 0; i < TEXT_BUFSIZ; i++) {
        historyBuf[*historyBufCount * TEXT_BUFSIZ + i] = message[i];
    }
    (*historyBufCount)++;
}

void handle_resize_event(int *w, int *h, uint32_t *buf)
{
    *w = tb_width();
    *h = tb_height();
    if (*w >= 2 && *w - 2 < TEXT_BUFSIZ) {
        buf[*w - 2] = 0;
    }
}

void handle_key_event(struct tb_event *ev, uint32_t *buf, int *c, int w, uint32_t *historyBuf,
                      uint16_t *historyBufCount, int h)
{
    if (ev->key == 0 && *c < TEXT_BUFSIZ - 1 && *c < w - 2) {
        buf[(*c)++] = ev->ch;
        buf[*c] = 0;
    }
    if ((ev->key == TB_KEY_BACKSPACE || ev->key == TB_KEY_BACKSPACE2) && *c > 0) {
        buf[--(*c)] = 0;
    }
    if (ev->key == TB_KEY_ENTER && *c > 0) {
        add_to_history(buf, historyBuf, historyBufCount, h);
        *c = 0;
        buf[0] = 0;
    }
}
