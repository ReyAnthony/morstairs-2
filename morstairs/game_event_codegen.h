#ifndef GAME_EVENT_CODEGEN_H_INCLUDED
#define GAME_EVENT_CODEGEN_H_INCLUDED

static void dungeon_events();
static void morstairs_events();
static void forgevielle_events();
static void overworld_events();
#define INIT_EVENTS OVERWORLD_EVENTS
#define DUNGEON_EVENTS dungeon_events();
#define MORSTAIRS_EVENTS morstairs_events();
#define FORGEVIELLE_EVENTS forgevielle_events();
#define OVERWORLD_EVENTS overworld_events();

#define TOWN_M\
    AUDIO_play(STONES, 1);

#define WORLD_M\
    AUDIO_play(OVERWORLD, 1);

#define DUNGEON_M\
    AUDIO_play(DUNGEON, 1);

#define ADD_EVENT_AT_POSITION(xx, yy, func) {\
    MAP_Point p = {.x = xx, .y = yy};\
    add_event_callback(func, p);\
}

#define MAKE_FUNC(name, xx, yy, map_to_load, on_collision_events)\
static void name();\
static void name() {\
    MAP_Point p = {.x = xx, .y = yy}; \
    MAP_load_another(map_to_load, p);\
    clear_events();\
    on_collision_events\
}\

/**
    THIS SHOULD BE A GENERATED FILE
    Creer un fichier qui recense tout les events pour toutes les cartes
    pour chaque map, generer les proto des functions des events (map_events)
    Ensuite, on genere les MACROS pour inserer ces protos

    par ex le fichier :
        overworld
            orig 12 12 dest 15 15 forgevielle
            orig 10 12 dest 20 32 dungeon
        dungeon
             orig 10 12 dest 20 32 overworld

    Ensuite pour chacune des map, on creer des fonction du style (origin_destination_N) (overworld_forgevielle_1)
    grace à la macro MAKE_FUNC
    par ex : MAKE_FUNC(overworld_forgevielle, 15, 15, FORGEVIELLE_MAP_PATH, FORGEVIELLE_EVENTS)

    Enfin on genere vraiment les fonctiosn d'events poyur chaque map en utilisant ADD_CALLBACK()
    Ex :
        overworld_events() {
            ADD_CALLBACK(12, 12, overworld_forgevielle_1)
            ADD_CALLBACK(20, 32, overworld_dungeon_1)
        }
**/

MAKE_FUNC(go_from_dungeon_to_morstairs, 29, 5,  MORSTAIRS_MAP_PATH, MORSTAIRS_EVENTS)
MAKE_FUNC(enter_town_of_morstairs, 12, 9, MORSTAIRS_MAP_PATH, MORSTAIRS_EVENTS)
MAKE_FUNC(go_overworld_from_forgevielle_down, 27, 7, OVERWORLD_MAP_PATH, OVERWORLD_EVENTS)
MAKE_FUNC(go_overworld_from_forgevielle_left, 26, 6, OVERWORLD_MAP_PATH, OVERWORLD_EVENTS)
MAKE_FUNC(go_from_dungeon_to_overworld, 19, 7, OVERWORLD_MAP_PATH, OVERWORLD_EVENTS)
MAKE_FUNC(go_overworld_from_morstairs, 17, 5, OVERWORLD_MAP_PATH, OVERWORLD_EVENTS)
MAKE_FUNC(enter_town_of_forgevielle_from_left, 12, 9, FORGEVIELLE_MAP_PATH, FORGEVIELLE_EVENTS)
MAKE_FUNC(enter_town_of_forgevielle_from_down, 19, 14, FORGEVIELLE_MAP_PATH, FORGEVIELLE_EVENTS)
MAKE_FUNC(go_into_dungeon_from_overworld, 23, 20, DUNGEON_MAP_PATH, DUNGEON_EVENTS)
MAKE_FUNC(go_into_dungeon, 16, 3, DUNGEON_MAP_PATH, DUNGEON_EVENTS)

static void dungeon_events() {
    ADD_EVENT_AT_POSITION(16, 3, go_from_dungeon_to_morstairs)
    ADD_EVENT_AT_POSITION(23, 20, go_from_dungeon_to_overworld)
    DUNGEON_M
}

static void morstairs_events() {
    ADD_EVENT_AT_POSITION(11, 9, go_overworld_from_morstairs)
    ADD_EVENT_AT_POSITION(29, 5, go_into_dungeon)
    TOWN_M
}

static void forgevielle_events() {
    ADD_EVENT_AT_POSITION(11, 9, go_overworld_from_forgevielle_left)
    ADD_EVENT_AT_POSITION(19, 15, go_overworld_from_forgevielle_down)
    TOWN_M
}

static void overworld_events() {
    ADD_EVENT_AT_POSITION(17, 5, enter_town_of_morstairs)
    ADD_EVENT_AT_POSITION(26, 6, enter_town_of_forgevielle_from_left)
    ADD_EVENT_AT_POSITION(27, 6, enter_town_of_forgevielle_from_left)
    ADD_EVENT_AT_POSITION(27, 7, enter_town_of_forgevielle_from_down)
    ADD_EVENT_AT_POSITION(19, 7, go_into_dungeon_from_overworld)
    WORLD_M
}

#endif // GAME_EVENT_CODEGEN_H_INCLUDED
