#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include "SDL/SDL.h"

typedef Uint16 Tile;

typedef struct point_t {
    int32_t x; // Signed
    int32_t y; // Signed
} MAP_Point;

typedef enum dir_e {
    NORTH,
    SOUTH,
    WEST,
    EAST
} MAP_Directions ;

//SHOULD ADD SOMETHING TO ADD AI FOES
void MAP_move(MAP_Directions direction);
void MAP_draw(SDL_Surface* screen, Uint32 delta_time);
int  MAP_init(const char* tileset_file, const char* map_file,
                     const char* collision_file, const char* animation_file,
                     Uint8 ts, Uint32 width, Uint32 height);
void MAP_set_position(MAP_Point new_position);
void MAP_quit();
void MAP_add_event_callback(void (*event)(), MAP_Point p);
void MAP_load_another(const char* map_file, MAP_Point new_position);

#endif // MAP_H_INCLUDED
