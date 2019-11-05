#include "fov.h"

#define FAILURE 0
#define SUCCESS 1

static MAP_SubmodulePackage smp;
static SDL_Surface* s;

void test2(SDL_Rect r, SDL_Surface* screen, Tile t, MAP_Point position_on_map);
void test(MAP_Point p);

MAP_SubmoduleDelegation  FOV_submodule_initializer(MAP_SubmodulePackage submod) {
    smp = submod;

    MAP_SubmoduleDelegation delegation =
    {
    .map_draw_delegate = test2,
    .player_draw_delegate = NULL,
    .on_movement_delegate = NULL
    };

    return delegation;
}

void f(int fov) {
     MAP_Point player = *smp.player_pos;

     int y_fov_max = player.y + fov;
     int y_max = player.y + 10;
     int y_min = player.y - 10;

     int x_min = player.x - 10;
     int x_max = player.x + 10;
     int x_min_fov = player.x - fov;
     int x_max_fov = player.x + fov;

     int y;
     for(y = player.y; y < y_max; y++) {

        //let's test the left side
        int hack = FAILURE; //it's for the cqse when you have a wall just beside

        int x;
        for(x = player.x; x > x_min; x--) {

            if(x < x_min_fov) {
                MAP_Point p = smp.to_screen_space(x, y);
                SDL_Rect r = {.x = p.x, .y = p.y, .w = smp.tile_size, .h = smp.tile_size};
                SDL_FillRect(s, &r, SDL_MapRGB(s->format, 0, 0, 0));
                if(hack) {
                    x_min_fov++;
                }
            }
            else {
                 if (smp.is_blocking(smp.map->data[y][x])) {
                    if (y == player.y + 1 && x == player.x && !hack) {
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
                SDL_Rect r = {.x = p.x, .y = p.y, .w = smp.tile_size, .h = smp.tile_size};
                SDL_FillRect(s, &r, SDL_MapRGB(s->format, 0, 0, 0));
                if(hack) {
                    x_max_fov--;
                }
            }
            else {
                if (smp.is_blocking(smp.map->data[y][x])) {
                    if (y == player.y + 1 && x == player.x && !hack) {
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
           SDL_Rect r = {.x = p.x, .y = p.y, .w = smp.tile_size, .h = smp.tile_size};
           SDL_FillRect(s, &r, SDL_MapRGB(s->format, 0, 0, 0));
        }
        else {
            //if the next block on y is a blocking one, then the fov stop
            if (smp.is_blocking(smp.map->data[y][player.x])) {
                y_fov_max = y;
            }
        }
     }
}

void f2(int fov) {
     MAP_Point player = *smp.player_pos;

     int y_fov_min = player.y - fov;
     int y_min = player.y - 10;

     int x_min = player.x - 10;
     int x_max = player.x + 10;
     int x_min_fov = player.x - fov;
     int x_max_fov = player.x + fov;

     int y;
     for(y = player.y; y > y_min; y--) {
         //let's test the left side
        int hack = FAILURE; //it's for the cqse when you have a wall just beside
        int x;
        for(x = player.x; x > x_min; x--) {

            if(x < x_min_fov) {
                 MAP_Point p = smp.to_screen_space(x, y);
                SDL_Rect r = {.x = p.x, .y = p.y, .w = smp.tile_size, .h = smp.tile_size};
                SDL_FillRect(s, &r, SDL_MapRGB(s->format, 0, 0, 0));
                if(hack) {
                    x_min_fov++;
                }
            }
            else {
                 if (smp.is_blocking(smp.map->data[y][x])) {
                    if (y == player.y - 1 && x == player.x && !hack) {
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
                SDL_Rect r = {.x = p.x, .y = p.y, .w = smp.tile_size, .h = smp.tile_size};
                SDL_FillRect(s, &r, SDL_MapRGB(s->format, 0, 0, 0));
                if(hack) {
                    x_max_fov--;
                }
            }
            else {
                if (smp.is_blocking(smp.map->data[y][x])) {
                    if (y == player.y - 1 && x == player.x && !hack) {
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
           SDL_Rect r = {.x = p.x, .y = p.y, .w = smp.tile_size, .h = smp.tile_size};
           SDL_FillRect(s, &r, SDL_MapRGB(s->format, 0, 0, 0));
        }
        else {
            //if the next block on y is a blocking one, then the fov stop
            if (smp.is_blocking(smp.map->data[y][player.x])) {
                y_fov_min = y;
            }
        }
     }
}

void test2(SDL_Rect r, SDL_Surface* screen, Tile t, MAP_Point position_on_map) {
    s = screen;

    SDL_SetAlpha(s, SDL_SRCALPHA, 128);

    f(4);
    f2(4);
}
