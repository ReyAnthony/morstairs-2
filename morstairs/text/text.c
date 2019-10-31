#include "text.h"
#define FAILURE 0
#define SUCCESS 1

typedef struct charset_t {
    Uint8 char_height;
    Uint8 char_width;
    Uint8 width;
    Uint8 height;
    SDL_Surface* characters;
    SDL_Rect rects[128];
} Charset;

static Charset loaded_charset;
static SDL_Surface* surface; //TO INIT  //TODO DO NOT FORGOT TO FREE IT ???

int TEXT_init(char* charset, Uint8 char_height, Uint8 char_width) {
    loaded_charset.characters = SDL_LoadBMP(charset);
    if(loaded_charset.characters == NULL) {
        return FAILURE;
    }

    Uint16 width = loaded_charset.characters->w;
    Uint16 height = loaded_charset.characters->h;
    loaded_charset.char_height = char_height;
    loaded_charset.char_width = char_width;
    loaded_charset.height = height / char_height;
    loaded_charset.width = width / char_width;

    Uint16 x, y, i;
    i = 0;
    for(y = 0; y < height; y += char_height) {
        for(x = 0; x < width; x += char_width) {
            loaded_charset.rects[i].w = loaded_charset.char_width;
            loaded_charset.rects[i].h = loaded_charset.char_height;
            loaded_charset.rects[i].x = x;
            loaded_charset.rects[i].y = y;
            i++;
        }
    }

    if(surface != NULL) {
        return FAILURE;
    }
    //max size should be parameterized (use memory of nothing on the DC)
    surface = SDL_CreateRGBSurface(SDL_HWSURFACE, 640, 480, 16, 0, 0, 0, 0);
    SDL_SetColorKey(surface, SDL_SRCCOLORKEY, SDL_MapRGB(surface->format, 0, 0, 0));
    return SUCCESS;
}

SDL_Surface* TEXT_blit_formatted(const char* format, ...) {
      va_list args;
      va_start (args, format);
      char buffer[1024];
      vsnprintf(buffer, sizeof(buffer), format, args);
      return TEXT_blit(buffer);
}

SDL_Surface* TEXT_blit(char* text) {
    SDL_FillRect(surface, NULL, 0x000000);
    Uint16 i;
    Uint16 line_jumps = 0;
    Uint16 x = 0;
    Uint16 longest_line = 0;
    for(i = 0; text[i] != '\0'; i++){
        char c = text[i];
        if(c == '\n') {
            line_jumps++;
            if(x > longest_line) {
                longest_line = x;
            }
            x = 0;
            continue;
        }
        else {
            x++;
        }
        SDL_Rect dest_rect = { .x = (x - 1) * loaded_charset.char_width,
                               .y = line_jumps*loaded_charset.char_height,
                               .w = loaded_charset.char_width,
                               .h = loaded_charset.char_height};
        SDL_BlitSurface(loaded_charset.characters, &loaded_charset.rects[(int) c -1], surface, &dest_rect);
    }
    return surface;
}

void TEXT_quit() {
    SDL_free(surface);
}
