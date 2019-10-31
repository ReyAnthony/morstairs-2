#ifndef PATHS_H_INCLUDED
#define PATHS_H_INCLUDED

#ifdef DC
#define CHARACTER_PATH "rd/character.bmp"
#define TILESET_PATH "rd/ultima.bmp"
#define OVERWORLD_MAP_PATH "rd/map.map"
#define COLLIDERS_PATH "rd/colliders.col"
#define ANIMATED_PATH "rd/animated.ani"
#define MORSTAIRS_MAP_PATH "rd/morstairs_city.map"
#define DUNGEON_MAP_PATH "rd/dungeon.map"
#define FORGEVIELLE_MAP_PATH "rd/forgevielle.map"
#define FONTS_PATH "rd/font.bmp"
#else
#define CHARACTER_PATH "romdisk/pc/character.bmp"
#define TILESET_PATH "romdisk/pc/ultima.bmp"
#define OVERWORLD_MAP_PATH "romdisk/map.map"
#define MORSTAIRS_MAP_PATH  "romdisk/morstairs_city.map"
#define COLLIDERS_PATH "romdisk/colliders.col"
#define ANIMATED_PATH "romdisk/animated.ani"
#define FORGEVIELLE_MAP_PATH "romdisk/forgevielle.map"
#define DUNGEON_MAP_PATH "romdisk/dungeon.map"
#define FONTS_PATH "romdisk/pc/font.bmp"
#endif

#endif // PATHS_H_INCLUDED
