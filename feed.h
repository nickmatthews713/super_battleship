#ifndef FEED_H
#define FEED_H

#include <ncurses.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <vector>

#include "field.h"

extern std::vector<char*> feed;

int display_feed(field_t &pc, field_t &npc);

#endif
