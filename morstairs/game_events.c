#include "game_events.h"

static void enter_town_of_morstairs();

static void enter_town_of_forgevielle_from_left();
static void enter_town_of_forgevielle_from_down();

static void go_into_dungeon();
static void go_into_dungeon_from_overworld();
static void go_from_dungeon_to_morstairs();
static void go_from_dungeon_to_overworld();

static void go_overworld_from_morstairs();
static void go_overworld_from_forgevielle_left();
static void go_overworld_from_forgevielle_down();

#define MAP_SWAP(xx, yy, map)({ \
    MAP_Point p = {.x = xx, .y = yy}; \
    MAP_load_another(map, p);\
});

#define TOWN\
    AUDIO_play(STONES, 1);

#define WORLD\
    AUDIO_play(OVERWORLD, 1);

#define DUNGEON_M\
    AUDIO_play(DUNGEON, 1);

#define DUNGEON_EVENTS()\
    MAP_Point p2 = {.x = 16, .y = 3};\
    MAP_add_event_callback(go_from_dungeon_to_morstairs, p2);\
    MAP_Point p3 = {.x = 23, .y = 20};\
    MAP_add_event_callback(go_from_dungeon_to_overworld, p3);\
    DUNGEON_M

#define MORSTAIRS_EVENTS()\
    MAP_Point p2 = {.x = 11, .y = 9}; \
    MAP_add_event_callback(go_overworld_from_morstairs, p2);\
    MAP_Point p3 = {.x = 29, .y = 5};\
    MAP_add_event_callback(go_into_dungeon, p3);\
    TOWN


#define FORGEVIELLE_EVENTS() \
    MAP_Point p2 = {.x = 11, .y = 9};\
    MAP_add_event_callback(go_overworld_from_forgevielle_left, p2);\
    MAP_Point p3 = {.x = 19, .y = 15};\
    MAP_add_event_callback(go_overworld_from_forgevielle_down, p3);\
    TOWN

#define OVERWORLD_EVENTS()\
    MAP_Point pe = {.x = 17, .y = 5};\
    MAP_add_event_callback(enter_town_of_morstairs, pe);\
    MAP_Point p2 = {.x = 26, .y = 6};\
    MAP_add_event_callback(enter_town_of_forgevielle_from_left, p2);\
    MAP_Point p3 = {.x = 27, .y = 6};\
    MAP_add_event_callback(enter_town_of_forgevielle_from_left, p3);\
    MAP_Point p4 = {.x = 27, .y = 7};\
    MAP_add_event_callback(enter_town_of_forgevielle_from_down, p4);\
    MAP_Point p5 = {.x = 19, .y = 7};\
    MAP_add_event_callback(go_into_dungeon_from_overworld, p5);\
    WORLD

 void EVENTS_init_overworld() {
    OVERWORLD_EVENTS()
 }

static void go_overworld_from_morstairs(){
    MAP_SWAP(17, 5, OVERWORLD_MAP_PATH)
    OVERWORLD_EVENTS()
}

static void go_into_dungeon() {
    MAP_SWAP(16, 3, DUNGEON_MAP_PATH)
    DUNGEON_EVENTS()
}

static void go_from_dungeon_to_morstairs() {
    MAP_SWAP(29, 5, MORSTAIRS_MAP_PATH)
    MORSTAIRS_EVENTS()
}

static void enter_town_of_morstairs() {
    MAP_SWAP(12, 9, MORSTAIRS_MAP_PATH)
    MORSTAIRS_EVENTS()
}

static void go_overworld_from_forgevielle_down() {
    MAP_SWAP(27, 7, OVERWORLD_MAP_PATH)
    OVERWORLD_EVENTS()
}

static void go_overworld_from_forgevielle_left() {
    MAP_SWAP(26, 6, OVERWORLD_MAP_PATH)
    OVERWORLD_EVENTS()
}

static void enter_town_of_forgevielle_from_left() {
    MAP_SWAP(12, 9, FORGEVIELLE_MAP_PATH)
    FORGEVIELLE_EVENTS()
}

static void enter_town_of_forgevielle_from_down() {
    MAP_SWAP(19, 14, FORGEVIELLE_MAP_PATH)
    FORGEVIELLE_EVENTS()
}

static void go_from_dungeon_to_overworld() {
    MAP_SWAP(19, 7, OVERWORLD_MAP_PATH)
    OVERWORLD_EVENTS()
}

static void go_into_dungeon_from_overworld() {
   MAP_SWAP(23, 20, DUNGEON_MAP_PATH)
   DUNGEON_EVENTS()
}
