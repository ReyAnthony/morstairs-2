#ifndef PATHS_H_INCLUDED
#define PATHS_H_INCLUDED

#ifdef DC
#define CHARACTER_PATH "rd/character.bmp"
#define TILESET_PATH "rd/ultima.bmp"
#define OVERWORLD_MAP_MAP_PATH "rd/overworld.map"
#define COLLIDERS_PATH "rd/colliders.col"
#define ANIMATED_PATH "rd/animated.ani"
#define MORSTAIRS_MAP_MAP_PATH "rd/morstairs.map"
#define DUNGEON_MAP_MAP_PATH "rd/dungeon.map"
#define FORGEVIELLE_MAP_PATH "rd/forgevielle.map"
#define FONTS_PATH "rd/font.bmp"
#define STONES "rd/stone.ogg"
#define OVERWORLD "rd/music.ogg"
#define DUNGEON "rd/dungeon.ogg"
#else
#define CHARACTER_PATH "romdisk/pc/character.bmp"
#define TILESET_PATH "romdisk/pc/ultima.bmp"
#define OVERWORLD_MAP_MAP_PATH "romdisk/overworld.map"
#define MORSTAIRS_MAP_MAP_PATH  "romdisk/morstairs.map"
#define COLLIDERS_PATH "romdisk/colliders.col"
#define ANIMATED_PATH "romdisk/animated.ani"
#define FORGEVIELLE_MAP_MAP_PATH "romdisk/forgevielle.map"
#define DUNGEON_MAP_MAP_PATH "romdisk/dungeon.map"
#define FONTS_PATH "romdisk/pc/font.bmp"
#define STONES "romdisk/stone.ogg"
#define OVERWORLD "romdisk/music.ogg"
#define DUNGEON "romdisk/dungeon.ogg"
#endif

#endif // PATHS_H_INCLUDED
