#include "event_editor.h"

//FUll of bugs, refactor me !
//bug save when we switch events (overwrite map)
//and sometimes bug at generation

#define FAILURE 0
#define SUCCESS 1

#define DELETED events.data[i].origin[0] == '\0' || events.data[i].destination[0] == '\0'
#define NOT_DELETED !(DELETED)

#define CHECK_ORIGIN_POS(xx, yy) \
    events.data[i].origin_pos.x == xx && events.data[i].origin_pos.y == yy

#define CHECK_DESTINATION_POS(xx, yy) \
    events.data[i].destination_pos.x == xx && events.data[i].destination_pos.y == yy

typedef struct ev_editor_event_t {
    char origin[512];
    char destination[512];
    MAP_Point origin_pos;
    MAP_Point destination_pos;
} EventEditorEvent;

typedef struct ev_editor_events_t {
    EventEditorEvent data[1024];
    int count;
} EventEditorEvents;

typedef enum phase_e {
    ORIGIN_SELECTION,
    DESTINATION_SELECTION
} Phase;

static MAP_SubmodulePackage smp;
static EventEditorEvents events;
static Phase phase;
static char map_name[512];

static void map_draw(SDL_Rect r, SDL_Surface* screen, Tile t, MAP_Point position_on_map);
static void on_new_map_loaded(char* filename);
static int generate_header();

MAP_SubmoduleDelegation EVENTED_submodule_initializer(MAP_SubmodulePackage submod) {
    smp = submod;
    MAP_SubmoduleDelegation delegation =
    {
    .map_draw_delegate = map_draw,
    .player_draw_delegate = NULL,
    .on_movement_delegate = NULL,
    .on_new_map_loaded = on_new_map_loaded
    };

    phase = ORIGIN_SELECTION;
    return delegation;
}

void EVENTED_set_hover_as_event() {
    if(phase == ORIGIN_SELECTION) {
        events.count++;
        strcpy(events.data[events.count-1].origin, map_name);
        events.data[events.count-1].origin_pos = MAPED_get_player_position();
        phase = DESTINATION_SELECTION;
    }
    else {
        strcpy(events.data[events.count-1].destination, map_name);
        events.data[events.count-1].destination_pos = *smp.player_pos;
        phase = ORIGIN_SELECTION;
    }
}

void EVENTED_load_file(char* file) {

    FILE* f;
    if((f = fopen(file, "r")) == NULL) {
        return FAILURE;
    }

    int i;
    fscanf(f, "%d", &events.count);
    for(i = 0; i < events.count; i++) {
        EventEditorEvent* ev = &events.data[i];
        fscanf(f, "%s%d%d%s%d%d",
                &ev->destination,
                &ev->destination_pos.x,
                &ev->destination_pos.y,
                &ev->origin,
                &ev->origin_pos.x,
                &ev->origin_pos.y);
    }

    fclose(f);

    return SUCCESS;
}

int EVENTED_save(char* file) {

    FILE* f;
    if((f = fopen(file, "w")) == NULL) {
        return FAILURE;
    }

     int i;
     int final_count = events.count;
     for(i = 0; i < events.count; i++) {
        if(DELETED) {
            final_count--;
            continue;
        }
    }

    fprintf(f, "%d\n", final_count);
    for(i = 0; i < events.count; i++) {
        if(DELETED) {
            continue;
        }
        EventEditorEvent ev = events.data[i];
        fprintf(f, "%s %d %d %s %d %d\n",
                ev.destination,
                ev.destination_pos.x,
                ev.destination_pos.y,
                ev.origin,
                ev.origin_pos.x,
                ev.origin_pos.y);
    }

    fclose(f);

    return generate_header();
}

void EVENTED_delete() {
    if (phase == DESTINATION_SELECTION) {
        return; //can't do it now
    }
    else {
        int i;
        for (i = 0; i < events.count; i++) {
            if(CHECK_ORIGIN_POS(smp.player_pos->x, smp.player_pos->y)){
                events.data[i].origin[0] = '\0';
                events.data[i].destination[0] = '\0';
            }

            if(CHECK_DESTINATION_POS(smp.player_pos->x, smp.player_pos->y)){
                events.data[i].destination[0] = '\0';
                events.data[i].origin[0] = '\0';
            }
        }
    }
}

int EVENTED_is_destination_selection_phase() {

    //we should lock most actions when we are in this phase
    if (phase == DESTINATION_SELECTION) {
        return SUCCESS;
    }

    return FAILURE;
}

void EVENTED_follow_event(char** file) {
    if (phase == DESTINATION_SELECTION) {
        return; //can't do it now
    }
    else {
        int i;
        for (i = 0; i < events.count; i++) {
            if(CHECK_ORIGIN_POS(smp.player_pos->x, smp.player_pos->y)){
                if(NOT_DELETED) {
                     MAP_load_another(events.data[i].destination, events.data[i].destination_pos);
                     break;
                }
            }

            if(CHECK_DESTINATION_POS(smp.player_pos->x, smp.player_pos->y)){
                if(NOT_DELETED) {
                    MAP_load_another(events.data[i].origin,  events.data[i].origin_pos);
                    strcpy(*file, events.data[i].origin);
                    break;
                }
            }
        }
    }
}

static void on_new_map_loaded(char* filename) {
    if(filename == NULL) {
        return;
    }

    strcpy(map_name, filename);
}

static void map_draw(SDL_Rect r, SDL_Surface* screen, Tile t, MAP_Point position_on_map) {

        int i;
        for (i = 0; i < events.count; i++) {
            if(CHECK_ORIGIN_POS(position_on_map.x, position_on_map.y)) {
                if(strcmp(events.data[i].origin, map_name) == 0) {
                      r.x = r.x + smp.tile_size/2;
                      r.w = smp.tile_size/4;
                      r.h = smp.tile_size/4;
                      SDL_FillRect(screen, &r, SDL_MapRGB(screen->format, 0, 255, 0));
                }
            }

            if(CHECK_DESTINATION_POS(position_on_map.x, position_on_map.y)) {
              if(strcmp(events.data[i].destination, map_name) == 0) {
                    r.y = r.y + smp.tile_size/2;
                    r.w = smp.tile_size/4;
                    r.h = smp.tile_size/4;
                    SDL_FillRect(screen, &r, SDL_MapRGB(screen->format, 0, 0, 255));
                    break;
                }
            }
        }
}

typedef struct event_func_gen_s {
    MAP_Point p;
    char func_name[255];
} AddEvent;

typedef struct event_func_gen_s2 {
    AddEvent addEvent[255];
    int add_ev_count;
    char name[255];
}EventFunc;

static EventFunc event_funcs[256];
static int event_func_count = 0;

static EventFunc* find_func_for_origin(char* origin, int event_func_count, EventFunc* event_funcs);
static EventFunc* find_func_for_origin(char* origin, int event_func_count, EventFunc* event_funcs) {

    int j;
    int has_found = FAILURE;

    for (j = 0; j < event_func_count; j++) {
         EventFunc ev_func = event_funcs[j];
         if(strcmp(ev_func.name, origin ) == 0) {
            has_found = SUCCESS;
            return &event_funcs[j];
         }
    }

    return NULL;
}

static int generate_header() {
    FILE* f;

    if((f = fopen("../morstairs/event_editor_gen.inc.h", "w")) == NULL) {
        return FAILURE;
    }

    fprintf(f, "//Code generated at %ld\n\n", time(NULL));

    int i;
    for(i = 0; i < events.count; i++) {
        if(DELETED) {
            continue;
        }
        EventEditorEvent ev = events.data[i];
        char* dest = basename(ev.destination);
        *strchr(dest, '.') = '_';
        dest = strupr(dest);
        char* origin = basename(ev.origin);
        *strchr(origin, '.') = '_';
        origin = strupr(origin);

        EventFunc* ev_func = find_func_for_origin(origin, event_func_count, event_funcs);
        EventFunc* ev_func2 = find_func_for_origin(dest, event_func_count, event_funcs);

        //hack, here we make a function for the destination, so that we
        //have it even if we don't have made a path
        if(ev_func2 == NULL) {
            ev_func2 = &event_funcs[event_func_count];
            strcpy(ev_func2->name, dest);
            ev_func2->add_ev_count = 0;
            event_func_count++;
        }

        //if we have not started to build it, then do it
        if(ev_func == NULL) {
            ev_func = &event_funcs[event_func_count];
            strcpy(ev_func->name, origin);
            ev_func->add_ev_count = 0;
            event_func_count++;
        }

        //anyway, save in the function
        AddEvent* add_ev = &ev_func->addEvent[ev_func->add_ev_count];
        sprintf(add_ev->func_name, "%s_TO_%s_%d", dest, origin, i);
        MAP_Point p = {.x = ev.origin_pos.x , .y =  ev.origin_pos.y};
        add_ev->p = p;
        ev_func->add_ev_count++;
    }

     int j;
     for(j = 0; j < event_func_count; j++) {
        EventFunc* ev_func = &event_funcs[j];
        fprintf(f, "static void %s_EVENTS_F();\n", ev_func->name);
     }

       for(j = 0; j < event_func_count; j++) {
        EventFunc* ev_func = &event_funcs[j];
        fprintf(f, "#define %s_EVENTS %s_EVENTS_F();\n", ev_func->name, ev_func->name);
     }

    fprintf(f, "\n");

     for(i = 0; i < events.count; i++) {
        if(DELETED) {
            continue;
        }
        EventEditorEvent ev = events.data[i];
        char* dest = basename(ev.destination);
        *strchr(dest, '.') = '_';
        dest = strupr(dest);
        char* origin = basename(ev.origin);
        *strchr(origin, '.') = '_';
        origin = strupr(origin);

        fprintf(f, "GENERATE_FUNC_LEADING_TO_DESTINATION(%s_TO_%s_%d, %d, %d, %s_MAP_PATH, %s_EVENTS)\n",
                dest, origin, i, ev.destination_pos.x, ev.destination_pos.y, dest, dest);
     }

    for(j = 0; j < event_func_count; j++) {
        EventFunc* ev_func = &event_funcs[j];
        fprintf(f, "\nstatic void %s_EVENTS_F() { \n", ev_func->name);
        int k;
        for(k = 0; k < ev_func->add_ev_count; k++) {
            AddEvent* add_ev = &ev_func->addEvent[k];
            fprintf(f, "\tADD_EVENT_AT_POSITION(%d, %d, %s)\n", add_ev->p.x, add_ev->p.y, add_ev->func_name);
        }
        fprintf(f, "\t%s_MUS\n", ev_func->name);
        fprintf(f, "}\n");
    }

    fclose(f);
    return SUCCESS;
}
