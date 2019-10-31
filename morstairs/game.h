#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#ifdef PC
#include <stdio.h>
#include <stdlib.h>
#endif

#include "SDL/SDL.h"
#include "paths.h"
#include "constants.h"
#include "game_events.h"
#include "audio/audio.h"


#include "text/text.h"
#include "map/map.h"

void GAME_start();
int GAME_init_engine();
void GAME_quit();

#endif // GAME_H_INCLUDED
