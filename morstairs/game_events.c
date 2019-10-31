#include "game_events.h"

typedef struct event_t {
    MAP_Point event_position;
    void (*event)(void);
} Event;

typedef struct events_t {
    Event data[255];
    int count;
} Events;

static void on_movement_delegate(MAP_Point p);
static void add_event_callback(void (*event)(void), MAP_Point p);
static void clear_events();

#include "game_event_codegen.h"

static void overworld_events();
static Events events;
static MAP_SubmodulePackage smp;

MAP_SubmoduleDelegation  EVENTS_submodule_initializer(MAP_SubmodulePackage submod) {
    smp = submod;

    MAP_SubmoduleDelegation delegation =
    {
    .map_draw_delegate = NULL,
    .player_draw_delegate = NULL,
    .on_movement_delegate = on_movement_delegate
    };

    INIT_EVENTS
    return delegation;
}

static void on_movement_delegate(MAP_Point p) {
    int i = 0;
    for(i=0; i < events.count; i++) {
        if(events.data[i].event_position.x == p.x && events.data[i].event_position.y == p.y) {
            events.data[i].event();
            break;
        }
    }
}

static void add_event_callback(void (*event)(void), MAP_Point p) {
    events.data[events.count].event = event;
    events.data[events.count].event_position = p;
    events.count++;
}

static void clear_events() {
    memset(&events, 0, sizeof(Events));
}
