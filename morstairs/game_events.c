#include "game_events.h"

#define FAILURE 0
#define SUCCESS 1

typedef struct event_t {
    MAP_Point event_position;
    void (*event)(void);
} Event;

typedef struct events_t {
    Event data[255];
    int count;
} Events;

typedef void (*func_on_curtain_end)();
typedef struct curtain_s {
    int is_enabled;
    int y;
    func_on_curtain_end func_on_end;
} Curtain;

static void on_movement_delegate(MAP_Point p);
static void draw_map_when_switching_scene(SDL_Rect r, SDL_Surface* screen, Tile t, MAP_Point position_on_map);
static void add_event_callback(void (*event)(void), MAP_Point p);
static void clear_events();

#include "game_event_codegen.h"
#include "event_editor_gen.inc.h"

static void overworld_events();
static Events events;
static MAP_SubmodulePackage smp;
static Curtain curtain;

MAP_SubmoduleDelegation  EVENTS_submodule_initializer(MAP_SubmodulePackage submod) {
    smp = submod;

    MAP_SubmoduleDelegation delegation =
    {
    .map_draw_delegate = draw_map_when_switching_scene,
    .player_draw_delegate = draw_map_when_switching_scene,
    .on_movement_delegate = on_movement_delegate
    };

    OVERWORLD_MAP_EVENTS
    return delegation;
}

static void on_movement_delegate(MAP_Point p) {
    int i = 0;
    for(i=0; i < events.count; i++) {
        if(events.data[i].event_position.x == p.x && events.data[i].event_position.y == p.y) {
            curtain.is_enabled = SUCCESS;
            curtain.y = 0;
            curtain.func_on_end = events.data[i].event;
            *smp.is_move_locked = SUCCESS;
            break;
        }
    }
}

static void draw_map_when_switching_scene(SDL_Rect r, SDL_Surface* screen, Tile t, MAP_Point position_on_map) {

    static int hack = 0; //needs delta time

    if(curtain.is_enabled) {
        if(curtain.y > smp.tile_size * 2) {
            curtain.is_enabled = FAILURE;
            *smp.is_move_locked = FAILURE;
            curtain.func_on_end();
        }
        r.w = smp.tile_size;
        r.h = curtain.y;
        SDL_FillRect(screen, &r, SDL_MapRGB(screen->format, 0, 0, 0));
        hack++;
        if(hack > 15) {
            curtain.y++;
            hack = 0;
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
