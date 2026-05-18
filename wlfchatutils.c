//
// Created by wlfgrl on 2026-05-17.
//
#include "termbox2.h"
#include "wlfchatutils.h"

void printBox(const int x1, const int y1, const int x2, const int y2, const uint32_t color)
{
  for (int j = y1; j <= y2; j++) {
    for (int i = x1; i <= x2; i++) {
      tb_set_cell(i, j, ' ', TB_DEFAULT, color);
    }
  }
}
