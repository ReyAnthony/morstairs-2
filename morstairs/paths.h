#ifndef PATHS_H_INCLUDED
#define PATHS_H_INCLUDED

#ifdef DC
    #define RD(path) "rd/"#path
    #define PPC(path) "rd/"#path
#else
    #define RD(path) "romdisk/"#path
    #define PPC(path) "romdisk/PC/"#path
#endif

#define TILESET_PATH PPC(ultima.bmp)
#define FONTS_PATH PPC(font.bmp)
#define OVERWORLD_MAP_MAP_PATH RD(overworld.map)
#define COLLIDERS_PATH RD(colliders.col)
#define ANIMATED_PATH RD(animated.ani)
#define MORSTAIRS_MAP_MAP_PATH RD(morstairs.map)
#define DUNGEON_MAP_MAP_PATH RD(dungeon.map)
#define FORGEVIELLE_MAP_MAP_PATH RD(forgevielle.map)
#define STONES RD(stone.ogg)
#define OVERWORLD RD(music.ogg)
#define DUNGEON RD(dungeon.ogg)
#define DUNGEON_2_MAP_MAP_PATH RD(dungeon_2.map)
#define NONOTOWN_MAP_MAP_PATH RD(nonotown.map)

#endif // PATHS_H_INCLUDED
