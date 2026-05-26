#ifndef WLFCHAT_RENDERING_H
#define WLFCHAT_RENDERING_H

#include <termbox2.h>
#include <stdint.h>
#include "wlfchatconstants.h"

void printBox(int x1, int y1, int x2, int y2, uint32_t color);
void render_ui(int w, int h, int c, uint32_t *buf, uint32_t *historyBuf, uint16_t historyBufCount, const char *username);

#endif
