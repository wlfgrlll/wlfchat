#ifndef WLFCHAT_EVENT_HANDLER_H
#define WLFCHAT_EVENT_HANDLER_H

#include <termbox2.h>
#include <stdint.h>
#include <stdio.h>
#include "wlfchatconstants.h"

void add_to_history(uint32_t *message, uint32_t *historyBuf, uint16_t *historyBufCount, int h);
void handle_resize_event(int *w, int *h, uint32_t *buf);
void handle_key_event(struct tb_event *ev, uint32_t *buf, int *c, int w, uint32_t *historyBuf,
                      uint16_t *historyBufCount, int h);

#endif
