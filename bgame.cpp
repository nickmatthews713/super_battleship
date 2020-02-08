#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "field.h"
#include "bgame.h"
#include "user.h"
#include "feed.h"

std::vector<char*> feed;

int main(int argc, char *argv[]) {

  init_game();

  long int seed_value = time(0);
  srand(seed_value);

  field_t pc_f;
  field_t npc_f;

  init_field(npc_f, false);
  init_field(pc_f, true);

  pc_f.print_field(PC_FIELD_Y, PC_FIELD_X);
  npc_f.print_field(NPC_FIELD_Y, NPC_FIELD_X);
  
  npc_f.generate_boats();
  pc_f.generate_boats();

  pc_f.print_field(PC_FIELD_Y, PC_FIELD_X);
  npc_f.print_field(NPC_FIELD_Y, NPC_FIELD_X);

  clear_text();

  char *begin_text = (char*) "begin battle";
  feed.push_back(begin_text);
  display_feed(pc_f, npc_f);
  
  while(pc_f.num_boats > 0 && npc_f.num_boats > 0) {
    pc_move(pc_f, npc_f);
    npc_f.print_field(NPC_FIELD_Y, NPC_FIELD_X);
    display_feed(pc_f, npc_f);
    npc_move(pc_f, npc_f);
    pc_f.print_field(PC_FIELD_Y, PC_FIELD_X);
  }

  getch();

  endwin();
  
  return 0;
}

void init_game() {

  initscr();
  noecho();
  keypad(stdscr, TRUE);
  start_color();

  init_color(COLOR_YELLOW, 800, 800, 0);
  init_color(COLOR_GREEN, 0, 800, 400);
  init_color(COLOR_MAGENTA, 550, 400, 800);

  init_pair(1, COLOR_BLUE, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
  init_pair(4, COLOR_YELLOW, COLOR_BLACK);
  init_pair(5, COLOR_CYAN, COLOR_BLACK);
  init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
  /*
  init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
  init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
  init_pair(COLOR_BLACK, COLOR_WHITE, COLOR_BLACK);
  */
}

void clear_text() {

  int x, y;
  for(y = 0; y < 2; y++) {
    for(x = 0; x < 50; x++) {
      mvaddch(y+FIELD_Y+1,x, ' ');
    }
  }

}

