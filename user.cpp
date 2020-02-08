#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <unistd.h>

#include "user.h"
#include "field.h"
#include "bgame.h"
#include "feed.h"

int pc_move(field_t &pc, field_t &npc) {

  pc.update_abilities();

  uint32_t attack;
  char *att;
  bool valid_attack;
  do{
    valid_attack = true;
    mvprintw(FIELD_Y+1, 0, "%s\n", "SELECT ATTACK:");
    mvprintw(FIELD_Y+2, 0, "%s\n", "'a'(normal) || 'q'(bomb) 'w'(torpedo) 'e'(flurry)");
    attack = getch();
    move(FIELD_Y, 0);
    clrtoeol();
    if(attack == 'a') {
      att = (char*) "Normal attack";
    } else if(attack == 'q' && pc.num_bombs > 0) {
      att = (char*) "Bomb attack";
    } else if(attack == 'w' && pc.num_torpedoes > 0) {
      att = (char*) "Torpedo attack";
    } else if(attack == 'e' && pc.num_flurry > 0) {
      att = (char*) "Flurry attack";
    } else {
      mvprintw(FIELD_Y, 0, "%s\n", "invalid key or out of weapon");
      valid_attack = false;
    }
  } while(attack != 'a' && attack != 'q' && attack != 'w' && attack != 'e' || valid_attack == false);

  move(FIELD_Y+8, 0);
  clrtoeol();
  
  uint32_t key = '0';
  int curs_x, curs_y;
  char cursor = '*';
  curs_x = 1;
  curs_y = 1;

  clear_text();
  mvprintw(FIELD_Y+1, 0, "%s %s %s\n", "SELECT LOCATION:", att, "selected");
  mvprintw(FIELD_Y+2, 0, "%s\n", "Use arrow keys to lacate, press 'f' to fire");

  while(key != 'f' && attack != 'e') {
    mvaddch(curs_y, curs_x + (50-FIELD_X), cursor);
    key = getch();
    
    if(npc.field[curs_y][curs_x] == HIT) {
      attron(COLOR_PAIR(HIT_PAIR));
      mvaddch(curs_y, curs_x+(50-FIELD_X), npc.field[curs_y][curs_x]);
      attroff(COLOR_PAIR(HIT_PAIR));
    } else if(npc.field[curs_y][curs_x] == SHIP && npc.visible == true) {
      attron(COLOR_PAIR(SHIP_PAIR));
      mvaddch(curs_y, curs_x+(50-FIELD_X), npc.field[curs_y][curs_x]);
      attroff(COLOR_PAIR(SHIP_PAIR));
    } else if(npc.field[curs_y][curs_x] == MISS) {
      mvaddch(curs_y, curs_x+(50-FIELD_X), npc.field[curs_y][curs_x]);
    } else if(npc.visible == true) {
      mvaddch(curs_y, curs_x+(50-FIELD_X), npc.field[curs_y][curs_x]);
    } else {
      mvaddch(curs_y, curs_x+(50-FIELD_X), ' ');
    }
    switch(key) {
    case KEY_UP:
      if(move_cursor(curs_x, curs_y, -1, 0) == 0) {
	curs_y -= 1;
      }
      break;
    case KEY_DOWN:
      if(move_cursor(curs_x, curs_y, 1, 0) == 0) {
	curs_y += 1;
      }
      break;
    case KEY_RIGHT:
      if(move_cursor(curs_x, curs_y, 0, 1) == 0) {
	curs_x += 1;
      }
      break;
    case KEY_LEFT:
      if(move_cursor(curs_x, curs_y, 0, -1) == 0) {
	curs_x -= 1;
      }
      break;
    }
  }

  if(attack == 'a') {
    npc.fire(curs_x, curs_y);
  } else if(attack == 'q') {
    pc.num_bombs--;
    npc.fire(curs_x, curs_y);
    npc.fire(curs_x+1, curs_y);
    npc.fire(curs_x-1, curs_y);
    npc.fire(curs_x, curs_y+1);
    npc.fire(curs_x, curs_y-1);
    feed.push_back((char*) "Bombs Away");
  } else if(attack == 'w') {
    pc.num_torpedoes--;
    int start, direction, x;
    start = 0;
    direction = 1;
    x = 0;
    do {
      display_feed(pc,npc);
      refresh();
      usleep(250000);
      x++;
      npc.fire(start+(direction*x), curs_y);
    } while(npc.boat_map[curs_y][start+(x*direction)] == NULL && x < FIELD_X);
    
    feed.push_back((char*) "Torpedo Out");
  } else if(attack == 'e') {
    pc.num_flurry--;
    int x, y, j;
    feed.push_back((char*) "Flurry!");
  }

  display_feed(pc,npc);
  refresh();
  usleep(250000);

  int bomb_chance, torpedo_chance, flurry_chance;
  bomb_chance = rand() % BOMB_PROB;
  torpedo_chance = rand() % TORPEDO_PROB;
  flurry_chance = rand() % FLURRY_PROB;
  if(attack == 'a') {
    if(bomb_chance == 0) {
      pc.num_bombs++;
      feed.push_back((char*) "+1 bomb");
    } else if(torpedo_chance == 0) {
      pc.num_torpedoes++;
      feed.push_back((char*) "+1 torpedo");
    } else if(flurry_chance == 0) {
      pc.num_flurry++;
      feed.push_back((char*) "+1 flurry");
    }
  }

  display_feed(pc, npc);
  refresh();
  usleep(250000);
  
  return 0;
}

int move_cursor(int curs_x, int curs_y, int y_off, int x_off) {

  int nxp, nyp;
  nxp = (curs_x + x_off);
  nyp = (curs_y + y_off);

  if(nxp == FIELD_X-1 || nxp == 0 || nyp == FIELD_Y-1  || nyp == 0) {
    return 1;
  }
  
  return 0;
}

int npc_move(field_t &pc, field_t &npc) {

  int shots, j;
  shots = (rand() % 2) + 1;
  clear_text();
  mvprintw(FIELD_Y+8, 0, "%s %s %d %s\n", "ENEMY IS ATTACKING:", "They fire", shots, "SHOTS");

  int x,y, tries;
  for(j = 0; j < shots; j++) {
    tries = 0;
    do {
      x = (rand() % (FIELD_X - 2) + 1);
      y = (rand() % (FIELD_Y - 2) + 1);
      tries++;
    } while(pc.field[y][x] == MISS || tries > 200);
    
    pc.fire(x, y);
    pc.print_field(PC_FIELD_Y, PC_FIELD_X);
    refresh();
    usleep(250000);
    display_feed(pc, npc);
  }

  return 0;
  
}
