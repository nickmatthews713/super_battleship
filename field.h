#ifndef FIELD_H
#define FIELD_H

#include <ncurses.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <vector>

#define FIELD_Y 15
#define FIELD_X 19
#define BOAT_NUMBER 6
#define BOMB_PROB 10
#define TORPEDO_PROB 20
#define FLURRY_PROB 28

#define PC_FIELD_X 0
#define PC_FIELD_Y 0

#define NPC_FIELD_X (50 - FIELD_X)
#define NPC_FIELD_Y 0

#define BORDER '#'
#define SHIP '@'
#define MISS 'M'
#define HIT 'X'

#define BORDER_PAIR 1
#define HIT_PAIR 2
#define SHIP_PAIR 5

class boat {
 public:
  int x, y;
  int size;
  int hits;
  bool destroyed;
 boat(int x, int y, int size, int hits, bool destroyed) : x(x), y(y), size(size), hits(hits), destroyed(destroyed) {}
};

class field_t {
 public:
  int num_boats;
  int num_bombs;
  int num_torpedoes;
  int num_flurry;
  std::vector<boat> boats;
  char field[FIELD_Y][FIELD_X];
  boat* boat_map[FIELD_Y][FIELD_X];
  bool visible;
  
  int generate_boats();
  int clear_maps();
  int print_field(int y_start, int x_start);
  int update_abilities();
  int fire(int x_pos, int y_pos);
  
};

void init_field(field_t &t, bool visible);


#endif
