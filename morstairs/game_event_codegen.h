#ifndef GAME_EVENT_CODEGEN_H_INCLUDED
#define GAME_EVENT_CODEGEN_H_INCLUDED

#define FORGEVIELLE_MAP_MUS\
    AUDIO_play(STONES, 1);

 #define MORSTAIRS_MAP_MUS\
    AUDIO_play(STONES, 1);

#define NONOTOWN_MAP_MUS\
    AUDIO_play(STONES, 1);

#define OVERWORLD_MAP_MUS\
    AUDIO_play(OVERWORLD, 1);

#define DUNGEON_MAP_MUS\
    AUDIO_play(DUNGEON, 1);

#define DUNGEON_2_MAP_MUS\
    AUDIO_play(DUNGEON, 1);

#define ADD_EVENT_AT_POSITION(xx, yy, func) {\
    MAP_Point p = {.x = xx, .y = yy};\
    add_event_callback(func, p);\
}

#define GENERATE_FUNC_LEADING_TO_DESTINATION(name, xx, yy, map_to_load, on_collision_events)\
static void name();\
static void name() {\
    MAP_Point p = {.x = xx, .y = yy}; \
    MAP_load_another(map_to_load, p);\
    clear_events();\
    on_collision_events\
}\


#endif // GAME_EVENT_CODEGEN_H_INCLUDED
