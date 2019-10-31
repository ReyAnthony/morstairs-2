#include "map.h"

#define FAILURE 0
#define SUCCESS 1
#define ANIM_FRAME_MS 120
#define TILE_MAX 65535

//SHOULD MOVE THE NPCS TO SOMEWHERE ELSE
//THEY CAN'T BE A PART OF THE MAP
//MAYBE A LAYER ?

//if we ever need to chunk the map, 1meg or 512k chunks would be good
typedef struct map_t {
    //char title[20]; //TODO ADD title
    Uint32 width;
    Uint32 height;
    Tile data[256][256];
} Map;

typedef struct colliders_t {
    Tile data[128];
    Uint16 count;
    Uint16 real_count_for_saving; //HACK to ignore the -1 from map edit
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

typedef struct event_t {
    MAP_Point event_position;
    void (*event)(void);
} Event;

typedef struct events_t {
    Event data[255];
    int count;
} Events;

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
static void call_callback_event_if_at_point(MAP_Point p);

static Animated animated;
static Colliders colliders;
static Map map;
static MAP_Point player_pos = { .x = 15, .y = 5};
static SDL_Rect tiles[512];
static SDL_Surface* tileset;
static Tile player_tile;
static Events events;

static Uint8 tile_size;
static Uint32 width;
static Uint32 height;

//MAPED
static Uint8 has_collisions;
static Uint8 show_collisions;

int MAP_init(const char* tileset_file, const char* map_file,
                     const char* collision_file, const char* animation_file,
                     Uint8 ts, Uint32 w, Uint32 h) {

    tileset = SDL_LoadBMP(tileset_file);
    tile_size = ts;
    width = w;
    height = h;

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

    has_collisions = SUCCESS;
    show_collisions = FAILURE;
    player_tile = 284;
    return SUCCESS;
}

void MAP_quit() {
    SDL_free(tileset);
    memset(&colliders, 0, sizeof(Colliders));
    memset(&map, 0, sizeof(Map));
    memset(&events, 0, sizeof(Events));
    memset(&animated, 0, sizeof(Animated));
}

void MAP_set_position(MAP_Point new_position) {
    player_pos = new_position;
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

    call_callback_event_if_at_point(player_pos);
}

void MAP_add_event_callback(void (*event)(void), MAP_Point p) {
    events.data[events.count].event = event;
    events.data[events.count].event_position = p;
    events.count++;
}

// Map editor
void MAPED_change_tile_at_cursor() {
    map.data[player_pos.y][player_pos.x] = player_tile;
}

MAP_Point MAPED_get_player_position() {
    return player_pos;
}

//USE CONDITIONAL COMPILATION TO REMOVE FROM THE GAME
int MAPED_save_all_modifications(char* map_file, char* collider_file, char* animated_file){

    //Save map
    FILE* f;
    if((f = fopen(map_file, "w")) == NULL) {
        return FAILURE;
    }
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wformat"
    fprintf(f, "%u %u\n", map.height, map.width);
    #pragma GCC diagnostic pop
    Uint16 x, y;
    for(y = 0; y < map.height; y++) {
        for(x = 0; x < map.width; x++) {
            fprintf(f, "%hu ", map.data[y][x]);
        }
        fprintf(f, "\n");
    }
    fclose(f);

    //Save colliders
    if((f = fopen(collider_file, "w")) == NULL) {
        return FAILURE;
    }

    fprintf(f, "%hu\n", colliders.real_count_for_saving);

    Uint8 i;
    for(i = 0; i < colliders.count; i++) {
        if(colliders.data[i] == TILE_MAX) {
            continue;
        }
        fprintf(f, "%hu\n", colliders.data[i]);
    }
    fclose(f);

    if((f = fopen(animated_file, "w")) == NULL) {
        return FAILURE;
    }

    int to_remove = 0;
     for(i = 0; i < animated.count; i++) {
        if(animated.data[i].frame_count <= 1) {
            to_remove++;
        }
    }

    fprintf(f, "%hu\n", animated.count - to_remove);
    for(i = 0; i < animated.count; i++) {
        if(animated.data[i].frame_count <= 1) {
            continue;
        }
        fprintf(f, "%hu %hu\n", animated.data[i].tile, animated.data[i].frame_count);
    }

    fclose(f);
    return SUCCESS;
}

void MAPED_init(int width, int height) {
    map.width = width;
    map.height = height;
    has_collisions = FAILURE;
    show_collisions = SUCCESS;
    player_tile = 1;

}

void MAPED_change_player_tile(Tile tile) {
    player_tile = tile;
}

void MAPED_set_as_collider(Tile tile) {

    int i;
    for(i = 0; i < colliders.count; i ++) {
        if(colliders.data[i] == tile) {
            return;
        }
    }

    colliders.data[colliders.count] = tile;
    colliders.count++;
    colliders.real_count_for_saving++;
}

void MAPED_unset_as_collider(Tile tile) {

    int i;
    for(i = 0; i < colliders.count; i ++) {
        if(colliders.data[i] == tile) {
            //sets it at -1 then, we will filter when saving
            //TODO HACK
            colliders.data[i] = TILE_MAX;
            colliders.real_count_for_saving--;
        }
    }
}

Tile MAPED_get_hover_tile() {
    MAP_Point p = player_pos;
    return map.data[p.y][p.x];
}

Tile MAPED_get_player_tile() {
    return player_tile;
}

void MAPED_remove_1_frame_to_animation(Tile tile) {

    //do not save if frame_count == 1
    int i;
    for (i = 0; i < animated.count; i ++) {
        Animation* a = &animated.data[i];
        if(a->tile == tile) {
            if(a->frame_count > 1) {
                a->frame_count--;
            }
            return;
        }
    }
}

void MAPED_add_1_frame_to_animation(Tile tile) {

    int i;
    for (i = 0; i < animated.count; i ++) {
        Animation* a = &animated.data[i];
        if(a->tile == tile) {
            a->frame_count++;
            return;
        }
    }

    Animation* a = &animated.data[animated.count];
    a->tile = tile;
    a->frame_count = 2;
    animated.count++;
}

// STATIC FUNCTIONS
static int init_tiles(SDL_Surface* tileset) {
    if(tileset == NULL) {
        return FAILURE;
    }
    Uint16 i = 0;
    Uint16 x;
    Uint16 y;
    for(y = 0; y < tileset->h; y+=tile_size) {
        for(x = 0; x < tileset->w; x+=tile_size) {
            tiles[i].h = tile_size;
            tiles[i].w = tile_size;
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
    colliders.real_count_for_saving = colliders.count;
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
    p.x = width/2 - ((player_pos.x - map_x) * tile_size);
    p.y = height/2 - ((player_pos.y - map_y) * tile_size);
    p.y -= 24; //hack for the wrong position

    if(p.x > width) {
        p.x = width;
    }
    if(p.y > height) {
        p.y = height;
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
    p.x = (player_pos.x) - ((width/2)/tile_size);
    p.y = (player_pos.y) - ((height/2)/tile_size);
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
    p.x = (player_pos.x) + (width/2)/tile_size;
    p.y = (player_pos.y) + (height/2)/tile_size;
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
     SDL_Rect pos = { .w = tile_size, .h = tile_size, .x = position.x, .y = position.y};

     SDL_BlitSurface(tileset, &tiles[get_animated_tile(player_tile)], screen, &pos);

      if(show_collisions) {
                if(is_blocking(player_tile)) {
                    pos.w = tile_size/4;
                    pos.h = tile_size/4;
                    SDL_FillRect(screen, &pos, SDL_MapRGB(screen->format, 255, 0, 0));
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
            SDL_Rect r = { .w = tile_size, .h = tile_size, .x = p2.x, .y = p2.y};
            SDL_BlitSurface(tileset, &tiles[get_animated_tile(tile)], screen, &r);
            if(show_collisions) {
                if(is_blocking(tile)) {
                    r.w = tile_size/4;
                    r.h = tile_size/4;
                    SDL_FillRect(screen, &r, SDL_MapRGB(screen->format, 255, 0, 0));
                }
            }
        }
    }
}

static void call_callback_event_if_at_point(MAP_Point p) {

    int i = 0;
    for(i=0; i < events.count; i++) {
        if(events.data[i].event_position.x == p.x && events.data[i].event_position.y == p.y) {
            events.data[i].event();
            break;
        }
    }
}
