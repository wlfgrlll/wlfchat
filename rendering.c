#include "rendering.h"

void printBox(const int x1, const int y1, const int x2, const int y2, const uint32_t color)
{
  for (int j = y1; j <= y2; j++) {
    for (int i = x1; i <= x2; i++) {
      tb_set_cell(i, j, ' ', TB_DEFAULT, color);
    }
  }
}

void render_ui(int w, int h, int c, uint32_t *buf, uint32_t *historyBuf, uint16_t historyBufCount, const char *username)
{
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
    for (int i = historyBufCount - 1, yOffset = 0; i >= 0 && yOffset < h - 3; i--) {
        if (historyBuf[i * TEXT_BUFSIZ] != 0) {
            int j = 0, xOffset = 0;
            do {
                tb_set_cell(xOffset++ + 1, h - 3 - yOffset, username[j], INPUT_FG - 32, TB_HI_BLACK);
            } while (username[j++] != 0 && xOffset < w - 4);
            tb_set_cell(xOffset++, h - 3 - yOffset, ':', INPUT_FG - 32, TB_HI_BLACK);
            j = 0;
            do {
                tb_set_cell(xOffset++ + 1, h - 3 - yOffset, historyBuf[i * TEXT_BUFSIZ + j], INPUT_FG, TB_HI_BLACK);
            } while (historyBuf[i * TEXT_BUFSIZ + ++j] != 0 && xOffset < w - 4);
            yOffset++;
        }
    }
    tb_present();
}
