#include "fov.h"

#define FAILURE 0
#define SUCCESS 1

typedef struct fov_case_t {
    int x;
    int y;
} FovUnlitCase;

typedef struct fov_cases_t {
    FovUnlitCase data[400];
    int count;
} FovUnlitCases;

static MAP_SubmodulePackage smp;
static SDL_Surface* s;
static FovUnlitCases fov_cases;
static int first_run;

static void f(int fov_x, int fov_y);
static void f2(int fov_x, int fov_y);
static void add_fov_unlit_case(int x, int y);
static void clear_fov_cases();
static void on_move(MAP_Point player_pos);
static void on_new_map_loaded();

void draw(SDL_Rect r, SDL_Surface* sceeen, Tile t, MAP_Point position_on_map);

MAP_SubmoduleDelegation  FOV_submodule_initializer(MAP_SubmodulePackage submod) {
    smp = submod;

    MAP_SubmoduleDelegation delegation =
    {
    .map_draw_delegate = draw,
    .on_movement_delegate = on_move,
    .on_new_map_loaded = on_new_map_loaded
    };

    fov_cases.count = 0;
    first_run = SUCCESS;
    return delegation;
}

static void on_move(MAP_Point player_pos) {
    clear_fov_cases();
    f(5, 3); //refactor needed
    f2(5, 3); //refactor needed
}

static void on_new_map_loaded() {
    on_move(*smp.player_pos);
}

void draw(SDL_Rect r, SDL_Surface* screen, Tile t, MAP_Point position_on_map) {
    s = screen;

    int i;
    for (i = 0; i < fov_cases.count; i++) {
        SDL_Rect rr = {.x = fov_cases.data[i].x, .y = fov_cases.data[i].y, .w = smp.tile_size, .h = smp.tile_size};
        SDL_FillRect(screen, &rr, SDL_MapRGB(screen->format, 0, 0, 0));
    }
}

static void add_fov_unlit_case(int x, int y) {
    fov_cases.count++;
    fov_cases.data[fov_cases.count].x = x;
    fov_cases.data[fov_cases.count].y = y;
}

static void clear_fov_cases() {
    fov_cases.count = 0;
    memset(fov_cases.data, 0, sizeof(fov_cases.data));
}

static void f(int fov_x, int fov_y) {
    MAP_Point player = *smp.player_pos;
    int y_fov_max = player.y + fov_y;
    int y_max = player.y + 10;
    int y_min = player.y - 10;

    int x_min = player.x - 10;
    int x_max = player.x + 10;

    int x_min_fov = player.x - fov_x;
    int x_max_fov = player.x + fov_x;

    int y;
    for(y = player.y; y < y_max; y++) {

        //let's test the left side
        int hack = FAILURE; //it's for the cqse when you have a wall just beside

        int x;
        for(x = player.x; x > x_min; x--) {

            if(x < x_min_fov) {
                MAP_Point p = smp.to_screen_space(x, y);
                add_fov_unlit_case(p.x, p.y);
                if(hack) {
                    x_min_fov++;
                }
            }
            else {
                 if (smp.is_blocking(smp.map->data[y][x])) {
                    if (x == player.x && !hack) {
                        x_min_fov = x - 1;
                        hack = SUCCESS;
                     }
                    else {
                        x_min_fov = x;
                    }
                }
            }
        }
        //qnd the right
        //let's test the left side
        hack = FAILURE;
        for(x = player.x; x < x_max; x++) {

            if(x > x_max_fov) {
                MAP_Point p = smp.to_screen_space(x, y);
                add_fov_unlit_case(p.x, p.y);
                if(hack) {
                    x_max_fov--;
                }
            }
            else {
                if (smp.is_blocking(smp.map->data[y][x])) {
                    if (x == player.x && !hack) {
                        x_max_fov = x + 1;
                        hack = SUCCESS;
                     }
                    else {
                        x_max_fov = x;
                    }
                }
            }
        }

        //if we are farther than the fov, it's always dark
        if(y > y_fov_max ) {
           MAP_Point p = smp.to_screen_space(player.x, y);
           add_fov_unlit_case(p.x, p.y);
        }
        else {
            //if the next block on y is a blocking one, then the fov stop
            if (smp.is_blocking(smp.map->data[y][player.x])) {
                y_fov_max = y;
            }
        }
     }
}

static void f2(int fov_x, int fov_y) {
     MAP_Point player = *smp.player_pos;

     int y_fov_min = player.y - fov_y;
     int y_min = player.y - 10;

     int x_min = player.x - 10;
     int x_max = player.x + 10;
     int x_min_fov = player.x - fov_x;
     int x_max_fov = player.x + fov_x;

     int y;
     for(y = player.y; y > y_min; y--) {
         //let's test the left side
        int hack = FAILURE; //it's for the cqse when you have a wall just beside
        int x;
        for(x = player.x; x > x_min; x--) {

            if(x < x_min_fov) {
                MAP_Point p = smp.to_screen_space(x, y);
                add_fov_unlit_case(p.x, p.y);
                if(hack) {
                    x_min_fov++;
                }
            }
            else {
                 if (smp.is_blocking(smp.map->data[y][x])) {
                    if (x == player.x && !hack) {
                        x_min_fov = x - 1;
                        hack = SUCCESS;
                     }
                    else {
                        x_min_fov = x;
                    }
                }
            }
        }
        //qnd the right
        //let's test the left side
        hack = FAILURE;
        for(x = player.x; x < x_max; x++) {

            if(x > x_max_fov) {
                MAP_Point p = smp.to_screen_space(x, y);
                add_fov_unlit_case(p.x, p.y);
                if(hack) {
                    x_max_fov--;
                }
            }
            else {
                if (smp.is_blocking(smp.map->data[y][x])) {
                    if ( x == player.x && !hack) {
                        x_max_fov = x + 1;
                        hack = SUCCESS;
                     }
                    else {
                        x_max_fov = x;
                    }
                }
            }
        }

        //if we are farther than the fov, it's always dark
        if(y < y_fov_min) {
           MAP_Point p = smp.to_screen_space(player.x, y);
           add_fov_unlit_case(p.x, p.y);
        }
        else {
            //if the next block on y is a blocking one, then the fov stop
            if (smp.is_blocking(smp.map->data[y][player.x])) {
                y_fov_min = y;
            }
        }
     }
}
