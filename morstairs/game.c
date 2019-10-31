#include "game.h"

#define FAILURE 0
#define SUCCESS 1
#define BUTTON_REPEAT_DELAY_MS 150

typedef struct Timer_s {
   Uint32 previous_ticks;
   Uint32 current_ticks;
   Uint32 delta_time;
} Timer;

static void update_timer();
static void init_timer();
static void game_draw(SDL_Surface* screen);
static void game_update(SDL_Event* event);

static Timer timer;
static Uint32 repeat_time = 0;

int GAME_init_engine() {

    init_timer();
    if(!MAP_init(TILESET_PATH, OVERWORLD_MAP_PATH,
                    COLLIDERS_PATH, ANIMATED_PATH,
                    TILE_SIZE, WIDTH, HEIGHT)) {
        fprintf(stderr, "Error while loading map engine");
        return FAILURE;
    }

    if(!TEXT_init(FONTS_PATH, FONT_H, FONT_W)) {
        return FAILURE;
    }

    if(!AUDIO_init()) {
        return FAILURE;
    }

    MAP_extends_with_submodule(EVENTS_submodule_initializer);
    return SUCCESS;
}

void GAME_quit() {
    AUDIO_quit();
    MAP_quit();
    TEXT_quit();
}

void GAME_start(SDL_Event* event, SDL_Surface* screen) {

    while(1) {
        game_update(event);
        game_draw(screen);
        SDL_Delay(10);
    }
}

// STATIC FUNCTIONS

static void update_timer() {
    timer.previous_ticks = timer.current_ticks;
    timer.current_ticks = SDL_GetTicks();
    timer.delta_time = timer.current_ticks - timer.previous_ticks;
}

static void init_timer() {
     timer.current_ticks = SDL_GetTicks();
}

static void game_draw(SDL_Surface* screen) {

     MAP_draw(screen, timer.delta_time);
     SDL_BlitSurface(TEXT_blit("Pre-alpha gameplay"), NULL, screen, NULL);
     SDL_Flip(screen);
     SDL_FillRect(screen, NULL, 0);
}

static void game_update(SDL_Event* event){

    update_timer();
    repeat_time += timer.delta_time;
    SDL_PollEvent(event);

    //TODO we should accept the first input at once, but delay the repeat
    #ifdef PC
    if(event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_ESCAPE) {
          exit(0);
    }
    else if (event->type == SDL_KEYDOWN && repeat_time > BUTTON_REPEAT_DELAY_MS) {
        repeat_time = 0;
        if(event->key.keysym.sym == SDLK_LEFT) {
            MAP_move(WEST);
        }
        else if(event->key.keysym.sym == SDLK_RIGHT) {
             MAP_move(EAST);
        }
        else if(event->key.keysym.sym == SDLK_UP) {
             MAP_move(NORTH);
        }
        else if(event->key.keysym.sym == SDLK_DOWN) {
             MAP_move(SOUTH);
        }
    }
    #endif
    #ifdef DC
    if(event->type == SDL_JOYBUTTONDOWN) {

        if(event->jbutton.button == DC_BUTTON_START) {
            exit(0);
        }
    }
    else if(event->type == SDL_JOYHATMOTION && repeat_time > BUTTON_REPEAT_DELAY_MS) {
        repeat_time = 0;
        if(event->jhat.hat == DC_HAT_CROSS) {
            switch(event->jhat.value) {
            case DC_HAT_CROSS_RIGHT:
                MAP_move(EAST);
                break;
            case DC_HAT_CROSS_LEFT:
                MAP_move(WEST);
                break;
            case DC_HAT_CROSS_DOWN:
                MAP_move(SOUTH);
                break;
            case DC_HAT_CROSS_UP:
                MAP_move(NORTH);
                break;
            default:
                break;
            }
        }
    }
    #endif
}
