//
// Created by wlfgrl on 2026-05-17.
//
#include "termbox2.h"
#include "wlfchatutils.h"

int printBox(const int x1, const int y1, const int x2, const int y2, const uint32_t color)
{
  for (int i = x1; i <= x2; i++) {
    for (int j = y1; j <= y2; j++) {
      tb_set_cell(i, j, ' ', TB_DEFAULT, color);
    }
  }
  return 0;
}

int render_string(const int x, const int y, const int max_w, const uint32_t fg, const uint32_t bg, const void *str, const bool is_wide) {
  int xOffset = 0;
  for (int j = 0; x + xOffset < max_w; j++) {
    const uint32_t ch = is_wide ? ((const uint32_t*)str)[j] : (uint32_t)((const char*)str)[j];
    if (ch == 0) break;
    tb_set_cell(x + xOffset++, y, ch, fg, bg);
  }
  return xOffset;
}