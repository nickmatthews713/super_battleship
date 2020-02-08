#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "field.h"
#include "feed.h"
#include "bgame.h"

void init_field(field_t &f, bool visible) {

  char *title = (char*) "BATTLESHIP";
  char *mission_control = (char*) "Live  Feed";
  char *mid_line = (char*) "------------";
  char *bottom_line = (char*) "--------------------------------------------------";
  
  mvprintw(0, FIELD_X, "%s", mid_line);
  mvprintw(2, FIELD_X, "%s", mid_line);
  mvprintw(FIELD_Y-1, FIELD_X, "%s", mid_line);
  mvprintw(4, FIELD_X, "%s", mid_line);
  mvprintw(FIELD_Y+3, 0, "%s", bottom_line);
  mvprintw(FIELD_Y+7, 0, "%s", bottom_line);
  mvprintw(1, FIELD_X+1, "%s\n", title);
  mvprintw(3, FIELD_X + 1, "%s\n", mission_control);

  int x, y;
  for(y = 0; y < FIELD_Y; y++) {
    for(x = 0; x < FIELD_X; x++) {
      if(y == 0 || x == 0 || y == FIELD_Y-1 || x == FIELD_X-1) {
	f.field[y][x] = BORDER;
      }
      else {
	f.field[y][x] = ' ';
      }
    }
  }

  for(y = 0; y < FIELD_Y; y++) {
    for(x = 0; x < FIELD_X; x++) {
      f.boat_map[y][x] = NULL;
    }
  }
  f.num_boats = BOAT_NUMBER;
  f.visible = visible;
  f.num_bombs = 1;
  f.num_torpedoes = 1;
  f.num_flurry = 1;
  
}

int field_t::print_field(int y_start, int x_start) {

  int x, y;
  for(y = 0; y < FIELD_Y; y++) {
    for(x = 0; x < FIELD_X; x++) {
      if(field[y][x] == BORDER) {
	attron(COLOR_PAIR(BORDER_PAIR));
	mvaddch(y+y_start, x+x_start, field[y][x]);
	attroff(COLOR_PAIR(BORDER_PAIR));
      } else if(field[y][x] == HIT) {
	attron(COLOR_PAIR(HIT_PAIR));
	mvaddch(y+y_start, x+x_start, field[y][x]);
	attroff(COLOR_PAIR(HIT_PAIR));
      } else if(field[y][x] == SHIP && visible == true) {
	attron(COLOR_PAIR(6));
	mvaddch(y+y_start, x+x_start, field[y][x]);
	attroff(COLOR_PAIR(6));
      } else if(field[y][x] == MISS) {
	mvaddch(y+y_start, x+x_start, field[y][x]);
      } else if(visible == true) {
	mvaddch(y+y_start, x+x_start, field[y][x]);
      }
    }
  }

  return 0;
}

int field_t::generate_boats() {

  int sizes[] = {6, 5, 4, 4, 3, 2, 2, 1};
  int spawn_d[][2] = {{0,1},{-1,0},{0,-1},{1,0}};

  boat *curr_boat;
  int size, hits, x_pos, y_pos, scan_x, scan_y, spawn_index;
  bool destroyed, valid_spawn;
  int j,y,i;
  uint32_t key = 'a';

  mvprintw(FIELD_Y+1, 0, "%s\n", "PLACING SHIPS:");
  mvprintw(FIELD_Y+2, 0, "%s\n", "press 'a' to randomize or 'any key to continue");

  while(true) {
    for(y = 0; y < num_boats; y++) {
      curr_boat = new boat(0, 0, 0, 0, false);
      curr_boat->size = sizes[y];
      do {
	valid_spawn = true;
	x_pos = (rand() % (FIELD_X - 1)) + 1;
	y_pos = (rand() % (FIELD_Y - 1)) + 1;
	for(i = 0; i < 4; i++) {
	  spawn_index = rand() % 4;
	  for(j = 0; j < curr_boat->size; j++) {
	    if(field[y_pos + (spawn_d[spawn_index][0]*j)][x_pos+(spawn_d[spawn_index][1]*j)] != ' ') {
	      valid_spawn = false;
	    }
	  }
	  if(valid_spawn == true) {
	    break;
	  }
	}
      } while(valid_spawn == false);
      for(j = 0; j < curr_boat->size; j++) {
	field[y_pos + (spawn_d[spawn_index][0]*j)][x_pos+(spawn_d[spawn_index][1]*j)] = SHIP;
	boat_map[y_pos + (spawn_d[spawn_index][0]*j)][x_pos+(spawn_d[spawn_index][1]*j)] = curr_boat;
      }
      boats.push_back(*curr_boat);
    }
    if(visible == false) {
      key = 'f';
    }
    else {
      print_field(0,0);
      key = getch();
    }
    if(key == 'a') {
      clear_maps();
    }
    else {
      break;
    }
  }

return 0;
}

int field_t::clear_maps() {

  int x, y;
  for(y = 0; y < FIELD_Y; y++) {
    for(x = 0; x < FIELD_X; x++) {
      if(y == 0 || x == 0 || y == FIELD_Y-1 || x == FIELD_X-1) {
	field[y][x] = BORDER;
      }
      else {
	field[y][x] = ' ';
      }
    }
  }
  
  for(y = 0; y < FIELD_Y; y++) {
    for(x = 0; x < FIELD_X; x++) {
      boat_map[y][x] = NULL;
    }
  }

  return 0;
}

int field_t::fire(int x_pos, int y_pos) {

  if(x_pos > FIELD_X-2 || x_pos < 1 || y_pos > FIELD_Y-2 || y_pos < 1) {
    return 1;
  }

  char symbol;
  boat *curr_boat;
  symbol = field[y_pos][x_pos];

  if(boat_map[y_pos][x_pos] != NULL) {
    curr_boat = boat_map[y_pos][x_pos];
    if(symbol == SHIP) {
      if(visible == false) {
	feed.push_back((char*) "HIT");
      } else {
	feed.push_back((char*) "Enemy hit");
      }
      field[y_pos][x_pos] = HIT;
      curr_boat->hits++;
      if(curr_boat->hits == curr_boat->size) {
	curr_boat->destroyed = true;
	num_boats--;
	if(visible == false) {
	  feed.push_back((char*) "SHIP SUNK");
	} else {
	  feed.push_back((char*) "lost ship");
	}
      }
    } else if(symbol == HIT) {
      feed.push_back((char*) "already hit");
    }
  } else {
    if(visible == false) {
      feed.push_back((char*) "MISS");
    } else {
      feed.push_back((char*) "enemy miss");
    }
    field[y_pos][x_pos] = MISS;
  }
  
  return 0;
}

int field_t::update_abilities() {

  mvprintw(FIELD_Y+4, 0, "%s %d\n", "Big_Bombs : ", num_bombs);
  mvprintw(FIELD_Y+5, 0, "%s %d\n", "Torpedoes : ", num_torpedoes);
  mvprintw(FIELD_Y+6, 0, "%s %d\n", "Flurries  : ", num_flurry);

  return 0;
}

