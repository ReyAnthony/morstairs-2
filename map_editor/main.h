#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <stdlib.h>
#include <SDL/SDL.h>
#include <string.h>
#include "../morstairs/text/text.h"
#include "../morstairs/map/map_editor.h"

#define MAP_IN "../morstairs/romdisk/map.map"
#define MAP_OUT "../morstairs/romdisk/map.map"
#define COL_IN "data/colliders.col"
#define COL_OUT "../morstairs/romdisk/colliders.col"

#define ANIM_IN "data/animated.ani"
#define ANIM_OUT "../morstairs/romdisk/animated.ani"

#define HELP_TXT  "Save : s\nQuit (no save) : Esc\
                  \nHide this panel : h\
                  \nCopy hover : c\
                  \n- pencil > colliders : 1\
                  \n+ pencil > colliders : 2\
                  \n- hover > colliders : 3\
                  \n+ hover > colliders : 4\
                  \n- pencil > frame : 5\
                  \n+ pencil > frame : 6\
                  \n- hover > frame : 7\
                  \n+ hover > frame : 8\
                  \nPrevious tile : 9\
                  \nNext Tile : 0\n"




#define DUMMY "data/dummy.file"

#define MSG_LIFETIME 1000

#define HEIGHT 360
#define WIDTH 640

#endif // MAIN_H_INCLUDED
