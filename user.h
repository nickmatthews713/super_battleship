#ifndef USER_H
#define USER_H

#include "field.h"

int pc_move(field_t &pc, field_t &npc);
int npc_move(field_t &pc, field_t &npc);
int move_cursor(int curs_x, int curs_y, int y_off, int x_off);
int print_feed();

#endif
