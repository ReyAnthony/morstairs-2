#ifndef PATHS_H_INCLUDED
#define PATHS_H_INCLUDED

#ifdef DC
    #define RD(path) "rd/"#path
    #define RD_MAPS(path) "romdisk/maps/"#path
    #define PPC(path) "rd/"#path
#else
    #define RD(path) "romdisk/"#path
    #define RD_MAPS(path) "romdisk/maps/"#path
    #define PPC(path) "romdisk/PC/"#path
#endif

#define TILESET_PATH PPC(ultima.bmp)
#define FONTS_PATH PPC(font.bmp)
#define STONES RD(stone.ogg)
#define OVERWORLD RD(music.ogg)
#define DUNGEON RD(dungeon.ogg)

//maps and data
#define OVERWORLD_MAP_MAP_PATH RD_MAPS(overworld.map)
#define COLLIDERS_PATH RD_MAPS(colliders.col)
#define ANIMATED_PATH RD_MAPS(animated.ani)
#define MORSTAIRS_MAP_MAP_PATH RD_MAPS(morstairs.map)
#define DUNGEON_MAP_MAP_PATH RD_MAPS(dungeon.map)
#define FORGEVIELLE_MAP_MAP_PATH RD_MAPS(forgevielle.map)
#define DUNGEON_2_MAP_MAP_PATH RD_MAPS(dungeon_2.map)
#define NONOTOWN_MAP_MAP_PATH RD_MAPS(nonotown.map)

#endif // PATHS_H_INCLUDED
