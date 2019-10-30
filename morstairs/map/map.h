#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include "SDL/SDL.h"

typedef Uint16 Tile;

typedef struct point_t {
    int32_t x; // Signed
    int32_t y; // Signed
} Point;

typedef enum dir_e {
    NORTH,
    SOUTH,
    WEST,
    EAST
} Directions ;

void MAP_move(Directions direction);
void MAP_draw(SDL_Surface* screen, Uint32 delta_time);
int  MAP_init(char* tileset_file, char* map_file,
                     char* collision_file, char* animation_file,
                     Uint8 ts, Uint32 width, Uint32 height);
void MAP_quit();

#endif // MAP_H_INCLUDED
