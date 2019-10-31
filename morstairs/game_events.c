#include "game_events.h"

static void enter_town_of_morstairs();
static void enter_town_of_forgevielle_from_left();
static void enter_town_of_forgevielle_from_down();
static void go_into_dungeon();
static void go_from_dungeon_to_morstairs();

static void go_overworld_from_morstairs();
static void go_overworld_from_forgevielle_left();
static void go_overworld_from_forgevielle_down();
static void town_music();
static void dungeon_music();

static void init_overworld_events();
static void draw_curtain();
static SDL_Surface* screen;

static void town_music() {
    AUDIO_play("rd/stone.ogg", 1);
}

static void dungeon_music() {
    AUDIO_play("rd/dungeon.ogg", 1);
}


static void draw_curtain() {
    int y;
    //for(y = 0; y < HEIGHT; y+= 10 ) {
     //   SDL_Rect r = { .w = WIDTH, .h = y, .x = 0, .y = 0};
        //SDL_FillRect(screen, &r, 0);
        //SDL_Flip(screen);
        //SDL_Delay(10);
    //}
}

void EVENTS_init_overworld(SDL_Surface* s) {
    screen = s;
    init_overworld_events();
}

static void go_overworld_from_morstairs() {
    MAP_quit();

    draw_curtain();

    MAP_init(TILESET_PATH, OVERWORLD_MAP_PATH,
                    COLLIDERS_PATH, ANIMATED_PATH,
                    TILE_SIZE, WIDTH, HEIGHT);

    MAP_Point p = {.x = 16, .y = 5};
    MAP_set_position(p);

    EVENTS_init_overworld(screen);
}

static void go_into_dungeon() {

    MAP_quit();

    draw_curtain();

    MAP_init(TILESET_PATH, DUNGEON_MAP_PATH,
                    COLLIDERS_PATH, ANIMATED_PATH,
                    TILE_SIZE, WIDTH, HEIGHT);

    MAP_Point p = {.x = 15, .y = 3};
    MAP_set_position(p);
    dungeon_music();

    MAP_Point p2 = {.x = 16, .y = 3};
    MAP_add_event_callback(go_from_dungeon_to_morstairs, p2);
}

static void go_from_dungeon_to_morstairs() {
    MAP_quit();

    draw_curtain();

    MAP_init(TILESET_PATH, MORSTAIRS_MAP_PATH,
                COLLIDERS_PATH, ANIMATED_PATH,
                TILE_SIZE, WIDTH, HEIGHT);
    MAP_Point p = {.x = 28, .y = 5};
    MAP_set_position(p);

    //init events for morstairs
    MAP_Point p2 = {.x = 11, .y = 9};
    MAP_add_event_callback(go_overworld_from_morstairs, p2);

    MAP_Point p3 = {.x = 29, .y = 5};
    MAP_add_event_callback(go_into_dungeon, p3);
    town_music();
}

static void go_overworld_from_forgevielle_down() {
    MAP_quit();

    draw_curtain();

    MAP_init(TILESET_PATH, OVERWORLD_MAP_PATH,
                    COLLIDERS_PATH, ANIMATED_PATH,
                    TILE_SIZE, WIDTH, HEIGHT);

    MAP_Point p = {.x = 27, .y = 8};
    MAP_set_position(p);

    EVENTS_init_overworld(screen);
}


static void go_overworld_from_forgevielle_left() {
    MAP_quit();

    draw_curtain();

    MAP_init(TILESET_PATH, OVERWORLD_MAP_PATH,
                    COLLIDERS_PATH, ANIMATED_PATH,
                    TILE_SIZE, WIDTH, HEIGHT);

    MAP_Point p = {.x = 25, .y = 6};
    MAP_set_position(p);

    EVENTS_init_overworld(screen);
}

static void enter_town_of_forgevielle_from_left() {
    MAP_quit();

    draw_curtain();

    MAP_init(TILESET_PATH, FORGEVIELLE_MAP_PATH,
                COLLIDERS_PATH, ANIMATED_PATH,
                TILE_SIZE, WIDTH, HEIGHT);
    MAP_Point p = {.x = 12, .y = 9};
    MAP_set_position(p);

    MAP_Point p2 = {.x = 11, .y = 9};
    MAP_add_event_callback(go_overworld_from_forgevielle_left, p2);

    MAP_Point p3 = {.x = 19, .y = 15};
    MAP_add_event_callback(go_overworld_from_forgevielle_down, p3);

    town_music();
}


static void enter_town_of_forgevielle_from_down() {
    MAP_quit();

    draw_curtain();

    MAP_init(TILESET_PATH, FORGEVIELLE_MAP_PATH,
                COLLIDERS_PATH, ANIMATED_PATH,
                TILE_SIZE, WIDTH, HEIGHT);
    MAP_Point p = {.x = 19, .y = 14};
    MAP_set_position(p);

    MAP_Point p2 = {.x = 11, .y = 9};
    MAP_add_event_callback(go_overworld_from_forgevielle_left, p2);

     MAP_Point p3 = {.x = 19, .y = 15};
    MAP_add_event_callback(go_overworld_from_forgevielle_down, p3);

    town_music();
}

static void enter_town_of_morstairs() {
    MAP_quit();

    draw_curtain();

    MAP_init(TILESET_PATH, MORSTAIRS_MAP_PATH,
                COLLIDERS_PATH, ANIMATED_PATH,
                TILE_SIZE, WIDTH, HEIGHT);
    MAP_Point p = {.x = 12, .y = 9};
    MAP_set_position(p);

    //init events for morstairs
    MAP_Point p2 = {.x = 11, .y = 9};
    MAP_add_event_callback(go_overworld_from_morstairs, p2);

    MAP_Point p3 = {.x = 29, .y = 5};
    MAP_add_event_callback(go_into_dungeon, p3);
    town_music();
}


static void init_overworld_events() {
    MAP_Point p = {.x = 17, .y = 5};
    MAP_add_event_callback(enter_town_of_morstairs, p);
    MAP_Point p2 = {.x = 26, .y = 6};
    MAP_add_event_callback(enter_town_of_forgevielle_from_left, p2);
    MAP_Point p3 = {.x = 27, .y = 6};
    MAP_add_event_callback(enter_town_of_forgevielle_from_left, p3);
    MAP_Point p4 = {.x = 27, .y = 7};
    MAP_add_event_callback(enter_town_of_forgevielle_from_down, p4);
    AUDIO_play("rd/music.ogg", 1);
}
