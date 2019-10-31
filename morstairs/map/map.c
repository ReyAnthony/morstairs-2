#include "map.h"

#define FAILURE 0
#define SUCCESS 1
#define ANIM_FRAME_MS 120

typedef struct delegates_from_submodules_t {
    MAP_SubmoduleDelegation data[255];
    int count;
} MAP_DelegatesFromSubmodules;

static int init_tiles(SDL_Surface* tileset);
static int load_map(char* map_file);
static int load_colliders(char* collider_file);
static int load_animated(char* animated_file);
static Uint8 is_blocking(Tile tile);
static Tile get_animated_tile(Tile tile);
static MAP_Point to_screen_space(Uint32 map_x, Uint32 map_y);
static MAP_Point get_top_left_corner_of_screen_as_map_coordinates();
static MAP_Point get_bottom_right_corner_of_screen_as_map_coordinates();
static void update_animations(Uint32 delta_time);
static void draw_map(SDL_Surface* screen);
static void draw_player(SDL_Surface* screen);

static Animated animated;
static Colliders colliders;
static Map map;
static MAP_Point player_pos = { .x = 15, .y = 5};
static SDL_Rect tiles[512];
static SDL_Surface* tileset;
static Tile player_tile;
static MAP_DelegatesFromSubmodules delegates;

static Uint8 TILE_SIZE;
static Uint32 SCREEN_WIDTH;
static Uint32 SCREEN_HEIGHT;
static int has_collisions = SUCCESS;

int MAP_init(const char* tileset_file, const char* map_file,
                     const char* collision_file, const char* animation_file,
                     Uint8 ts, Uint32 w, Uint32 h) {

    tileset = SDL_LoadBMP(tileset_file);
    TILE_SIZE = ts;
    SCREEN_WIDTH = w;
    SCREEN_HEIGHT = h;

    if(tileset == NULL) {
        return FAILURE;
    }
    if(!init_tiles(tileset)) {
        return FAILURE;
    }
    if(!load_map(map_file)) {
        return FAILURE;
    }
    if(!load_colliders(collision_file)) {
        return FAILURE;
    }
    if(!load_animated(animation_file)) {
        return FAILURE;
    }

    player_tile = 284;
    return SUCCESS;
}

void MAP_extends_with_submodule(MAP_SubmoduleDelegation (*submodule_init)(MAP_SubmodulePackage)) {
    MAP_SubmodulePackage smp =
            {
                .map = &map,
                .colliders = &colliders,
                .animated = &animated,
                .player_tile = &player_tile,
                .player_pos = &player_pos,
                .has_collisions = &has_collisions,
                .tile_size = TILE_SIZE,

                //api
                .is_blocking = is_blocking
            };

    delegates.data[delegates.count] = submodule_init(smp);
    delegates.count++;

    //TODO delete submodules..
}

void MAP_load_another(const char* map_file, MAP_Point p) {
    memset(&map, 0, sizeof(Map));
    load_map(map_file);
    player_pos = p;
}

void MAP_quit() {
    SDL_FreeSurface(tileset);
    memset(&colliders, 0, sizeof(Colliders));
    memset(&map, 0, sizeof(Map));
    memset(&animated, 0, sizeof(Animated));
}

void MAP_draw(SDL_Surface* screen, Uint32 delta_time) {

    update_animations(delta_time);
    draw_map(screen);
    draw_player(screen);
}

void MAP_move(MAP_Directions direction) {

    switch(direction) {
    case NORTH:
        if(player_pos.y > 0) {
            Uint16 tile_at_player_pos = map.data[player_pos.y - 1][player_pos.x];
            if(!is_blocking(tile_at_player_pos) || !has_collisions){
                player_pos.y--;
            }
            else {
                //play feedback sound
            }
        }
        else {
            //play feedback sound
        }
        break;
    case SOUTH:
        if(player_pos.y <= map.height) {
            Tile tile_at_player_pos = map.data[player_pos.y + 1][player_pos.x];
            if(!is_blocking(tile_at_player_pos) || !has_collisions){
                player_pos.y++;
            }
            else {
                //play feedback sound
            }
        }
        else {
            //play feedback sound
        }
        break;
    case EAST:
        if(player_pos.x <= map.width) {
            Tile tile_at_player_pos = map.data[player_pos.y][player_pos.x + 1];
            if(!is_blocking(tile_at_player_pos) || !has_collisions){
                player_pos.x++;
            }
            else {
                //play feedback sound
            }
        }
        else {
            //play feedback sound
        }
        break;
    case WEST:
        if(player_pos.x > 0) {
            Tile tile_at_player_pos = map.data[player_pos.y][player_pos.x - 1];
            if(!is_blocking(tile_at_player_pos) || !has_collisions){
                player_pos.x--;
            }
            else {
                //play feedback sound
            }
        }
        else {
            //play feedback sound
        }
        break;
    default:
        break;
    }

    int i;
    for (i = 0; i < delegates.count; i++) {
        if(delegates.data[i].on_movement_delegate != NULL) {
            delegates.data[i].on_movement_delegate(player_pos);
        }
    }
}

// STATIC FUNCTIONS
static int init_tiles(SDL_Surface* tileset) {
    if(tileset == NULL) {
        return FAILURE;
    }
    Uint16 i = 0;
    Uint16 x;
    Uint16 y;
    for(y = 0; y < tileset->h; y+=TILE_SIZE) {
        for(x = 0; x < tileset->w; x+=TILE_SIZE) {
            tiles[i].h = TILE_SIZE;
            tiles[i].w = TILE_SIZE;
            tiles[i].x = x;
            tiles[i].y = y;
            i++;
        }
    }

   return SUCCESS;
}

static int load_map(char* map_file) {
    FILE* f;
    if((f = fopen(map_file, "r")) == NULL) {
        return FAILURE;
    }
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wformat"
    fscanf(f, "%u%u", &map.height, &map.width);
    #pragma GCC diagnostic pop
    Uint16 x, y;
    for(y = 0; y < map.height; y++) {
        for(x = 0; x < map.width; x++) {
            fscanf(f, "%hu", &map.data[y][x]);
        }
    }
    fclose(f);
    return SUCCESS;
}

static int load_colliders(char* collider_file) {
    FILE* f;
    if((f = fopen(collider_file, "r")) == NULL) {
        return FAILURE;
    }

    fscanf(f, "%hu", &colliders.count);
    Uint8 i;
    for(i = 0; i < colliders.count; i++) {
        fscanf(f, "%hu", &colliders.data[i]);
    }
    fclose(f);
    return SUCCESS;
}

static int load_animated(char* animated_file) {
    FILE* f;
    if((f = fopen(animated_file, "r")) == NULL) {
        return FAILURE;
    }

    fscanf(f, "%hu", &animated.count);
    Uint8 i;
    for(i = 0; i < animated.count; i++) {
        fscanf(f, "%hu%hu", &animated.data[i].tile, &animated.data[i].frame_count);
        animated.data[i].anim_index = 0;
        animated.data[i].anim_time = 0;
    }
    fclose(f);
    return SUCCESS;
}

static Uint16 get_animated_tile(Tile tile) {
    Uint8 i;
    for(i = 0; i < animated.count; i++) {
        Animation* animation = &animated.data[i];
        if(animation->tile == tile) {
            return animation->tile + animation->anim_index;
        }
    }
    return tile;
}

static void update_animations(Uint32 delta_time) {

    int i;
    for(i = 0; i < animated.count; i++) {

        Animation* animation = &animated.data[i];

        animation->anim_time += delta_time;
        if(animation->anim_time > ANIM_FRAME_MS) {
            if(animation->anim_index < animation->frame_count-1) {
                animation->anim_index++;
            }
            else {
                animation->anim_index = 0;
            }
            animation->anim_time = 0;
        }
    }
}

static Uint8 is_blocking(Tile tile) {

    Uint8 i;
    for (i = 0; i < colliders.count; i++) {
        if(colliders.data[i] == tile) {
            return SUCCESS;
        }
    }
    return FAILURE;
}

static MAP_Point to_screen_space(Uint32 map_x, Uint32 map_y) {
    MAP_Point p;
    p.x = SCREEN_WIDTH/2 - ((player_pos.x - map_x) * TILE_SIZE);
    p.y = SCREEN_HEIGHT/2 - ((player_pos.y - map_y) * TILE_SIZE);
    p.y -= 12; //hack for the wrong position

    if(p.x > SCREEN_WIDTH) {
        p.x = SCREEN_WIDTH;
    }
    if(p.y > SCREEN_HEIGHT) {
        p.y = SCREEN_HEIGHT;
    }
    if(p.x < 0) {
        p.x = 0;
    }
    if(p.y < 0) {
        p.y = 0;
    }
    return p;
}

static MAP_Point get_top_left_corner_of_screen_as_map_coordinates() {
    MAP_Point p;
    p.x = (player_pos.x) - ((SCREEN_WIDTH/2)/TILE_SIZE);
    p.y = (player_pos.y) - ((SCREEN_HEIGHT/2)/TILE_SIZE);
     if(p.x < 0) {
        p.x = 0;
    }
    if(p.y < 0) {
        p.y = 0;
    }
    return p;
}

static MAP_Point get_bottom_right_corner_of_screen_as_map_coordinates() {
    MAP_Point p;
    p.x = (player_pos.x) + (SCREEN_WIDTH/2)/TILE_SIZE;
    p.y = (player_pos.y) + (SCREEN_HEIGHT/2)/TILE_SIZE;
     if(p.x > map.width) {
        p.x = map.width;
    }
    if(p.y > map.height) {
        p.y = map.height;
    }
    return p;
}

static void draw_player(SDL_Surface* screen) {

     //Drawing the player at the center of the screen
     MAP_Point position = to_screen_space(player_pos.x, player_pos.y);
     SDL_Rect pos = { .w = TILE_SIZE, .h = TILE_SIZE, .x = position.x, .y = position.y};

     SDL_BlitSurface(tileset, &tiles[get_animated_tile(player_tile)], screen, &pos);

     int i;
     for (i = 0; i < delegates.count; i++) {
        if(delegates.data[i].player_draw_delegate != NULL) {
            delegates.data[i].player_draw_delegate(pos, screen, player_tile);
        }
     }
}

static void draw_map(SDL_Surface* screen) {

     //Drawing the map
    Uint32 x, y = 0;
    MAP_Point p = get_top_left_corner_of_screen_as_map_coordinates();
    MAP_Point limits = get_bottom_right_corner_of_screen_as_map_coordinates();

    for(x = p.x; x <= limits.x; x++) {
        for(y = p.y; y <= limits.y; y++) {
            Tile tile = map.data[y][x];
            MAP_Point p2 = to_screen_space(x, y);
            SDL_Rect r = { .w = TILE_SIZE, .h = TILE_SIZE, .x = p2.x, .y = p2.y};
            SDL_BlitSurface(tileset, &tiles[get_animated_tile(tile)], screen, &r);

            int i;
            for (i = 0; i < delegates.count; i++) {
                if(delegates.data[i].map_draw_delegate != NULL) {
                    delegates.data[i].map_draw_delegate(r, screen, tile);
                }
            }
        }
    }
}
