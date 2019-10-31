#ifdef DC
#include <kos.h>
#include <arch/arch.h>
    #ifdef DEBUG
    #include <arch/gdb.h>
    #endif
KOS_INIT_FLAGS(INIT_DEFAULT | INIT_MALLOCSTATS);
extern uint8 romdisk[];
KOS_INIT_ROMDISK(romdisk);
#endif

//common includes
#include "game.h"
#include <time.h>

static SDL_Event event;
static SDL_Surface* screen;

void exit_cleanup();
void exit_cleanup() {
    SDL_FreeSurface(screen);
    GAME_quit();
#ifdef DC
     arch_reboot(); //if we reboot we don't get the stats
#endif // DC
}

int main(int argc, char **argv) {
#if defined(DEBUG) && defined(DC)
	gdb_init();
	printf("Connection established to %s!", PROJECT_NAME);
#endif

    srand(time(NULL));
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK);
    SDL_ShowCursor(0);
    SDL_JoystickOpen(0);

    #ifdef DC
    Uint32 opts = SDL_SWSURFACE;
    screen = SDL_SetVideoMode(WIDTH,HEIGHT, 16, opts);
    #endif
    #ifdef PC
    Uint32 opts = SDL_HWSURFACE|SDL_FULLSCREEN;
    screen = SDL_SetVideoMode(WIDTH,HEIGHT, 16 * 2, opts);
    #endif

    if(screen == NULL) {
        return 1;
    }
    if(!GAME_init_engine()) {
        return 1;
    }
    atexit(exit_cleanup);
    GAME_start(&event, screen);
    return 0;
}
