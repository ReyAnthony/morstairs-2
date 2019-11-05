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

typedef struct map_t {
    Uint32 width;
    Uint32 height;
    Tile data[256][256];
} Map;

typedef struct colliders_t {
    Tile data[128];
    Uint16 count;
} Colliders;

typedef struct animation_t {
    Tile tile;
    Uint8 frame_count;
    Uint8 anim_index;
    Uint32 anim_time;
} Animation;

typedef struct animated_t {
    Animation data[32];
    Uint16 count;
} Animated;

//Given to submodules
typedef struct MAP_submodule_t {
    Map* map;
    MAP_Point* player_pos;
    Colliders* colliders;
    Animated* animated;
    Tile* player_tile;
    int* has_collisions;
    int tile_size;
    int* is_move_locked;

    //API exposed to submodules
    int (*is_blocking)(Tile t);
    MAP_Point (*to_screen_space) (Uint32 map_x, Uint32 map_y);
} MAP_SubmodulePackage;

typedef struct MAP_submodule_delegation_t {
    void (*map_draw_delegate)(SDL_Rect r, SDL_Surface* screen, Tile t, MAP_Point position_on_map);
    void (*player_draw_delegate)(SDL_Rect r, SDL_Surface* screen, Tile t, MAP_Point position_on_map);
    void (*on_movement_delegate) (MAP_Point point);
} MAP_SubmoduleDelegation;

int  MAP_init(const char* tileset_file, const char* map_file,
                     const char* collision_file, const char* animation_file,
                     Uint8 ts, Uint32 width, Uint32 height);
void MAP_quit();

void MAP_move(MAP_Directions direction);
void MAP_draw(SDL_Surface* screen, Uint32 delta_time);

void MAP_load_another(const char* map_file, MAP_Point new_position);
void MAP_extends_with_submodule(MAP_SubmoduleDelegation (*submodule_init)(MAP_SubmodulePackage)) ;

#endif // MAP_H_INCLUDED
