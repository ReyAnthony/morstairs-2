#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <stdlib.h>
#include <SDL/SDL.h>
#include <string.h>
#include "tinyfiledialogs/tinyfiledialogs.h"
#include "../morstairs/text/text.h"
#include "map/map_editor.h"
#include "event_editor.h"

#define FONTSET "../morstairs/romdisk/PC/font.bmp"
#define TILESET "../morstairs/romdisk/PC/ultima.bmp"
#define COL_IN "data/colliders.col"
#define COL_OUT "data/colliders.col"
#define EV_IN "data/ev.events"
#define ANIM_IN "data/animated.ani"
#define ANIM_OUT "data/animated.ani"

#define HELP_TXT  "Save : s\nQuit (no save) : Esc\
                  \nHide this panel : h\
                  \nCopy hover : c\
                  \nLoad another map : r\
                  \n- pencil > colliders : 1\
                  \n+ pencil > colliders : 2\
                  \n- hover > colliders : 3\
                  \n+ hover > colliders : 4\
                  \n- pencil > frame : 5\
                  \n+ pencil > frame : 6\
                  \n- hover > frame : 7\
                  \n+ hover > frame : 8\
                  \nPrevious tile : 9\
                  \nNext Tile : 0\
                  \nDest event : e\
                  \nDelete event : d\
                  \nFollow event : g"

#define DUMMY "data/dummy.file"

#define MSG_LIFETIME 1000

#define HEIGHT 720
#define WIDTH 1280

#endif // MAIN_H_INCLUDED
