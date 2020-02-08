#include <ncurses.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <vector>

#include "feed.h"
#include "field.h"

int display_feed(field_t &pc, field_t &npc) {

  int limit;
  if(feed.size() > 9) {
    limit = 9;
  } else {
    limit = feed.size();
  }
  int j;
  char *curr_info;
  char first_letter;
  for(j = 0; j < limit; j++) {
    curr_info = feed[feed.size() - 1 - j];
    first_letter = curr_info[0];
    if(first_letter == 'E' || first_letter == 'l') {
      attron(COLOR_PAIR(2));
      mvprintw(5+j, FIELD_X, "%s\n", curr_info);
      attroff(COLOR_PAIR(2));
    } else if(first_letter == 'H' || first_letter == 'S') {
      attron(COLOR_PAIR(3));
      mvprintw(5+j, FIELD_X, "%s\n", curr_info);
      attroff(COLOR_PAIR(3));
    } else if(first_letter == '+') {
      attron(COLOR_PAIR(4));
      mvprintw(5+j, FIELD_X, "%s\n", curr_info);
      attroff(COLOR_PAIR(4));
    } else {
      mvprintw(5+j, FIELD_X, "%s\n", curr_info);
    }
  }

  pc.print_field(PC_FIELD_Y, PC_FIELD_X);
  npc.print_field(NPC_FIELD_Y, NPC_FIELD_X);
  
  return 0;
}
