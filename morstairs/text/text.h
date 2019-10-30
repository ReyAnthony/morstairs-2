#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#include "SDL/SDL.h"

int TEXT_init(char* charset, Uint8 char_height, Uint8 char_width);
SDL_Surface* TEXT_blit(char* text);
SDL_Surface* TEXT_blit_formatted(const char* format, ...);
void TEXT_quit();

#endif // TEXT_H_INCLUDED
