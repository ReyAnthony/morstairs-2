#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#ifdef PC
#include <stdio.h>
#include <stdlib.h>
#endif

#include "SDL/SDL.h"
#include "paths.h"
#include "constants.h"

#include "text/text.h"
#include "map/map.h"

void start_game();
int init_game_engine();
void quit_game_engine();

#endif // GAME_H_INCLUDED
