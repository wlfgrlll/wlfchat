//
// Created by wlfgrl on 2026-05-17.
//

#ifndef WLFCHAT_UTILS_H
#define WLFCHAT_UTILS_H
#include <stdbool.h>

int printBox(int x1, int y1, int x2, int y2, uint32_t color);
int render_string(int x, int y, int max_w, uint32_t fg, uint32_t bg, const void *str, bool is_wide);
#endif //WLFCHAT_UTILS_H
