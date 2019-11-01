#ifndef EVENT_EDITOR_H_INCLUDED
#define EVENT_EDITOR_H_INCLUDED

#include "../morstairs/map/map.h"
#include "map/map_editor.h"
#include <libgen.h>

MAP_SubmoduleDelegation EVENTED_submodule_initializer(MAP_SubmodulePackage submod);
void EVENTED_load_file(char* file);
int EVENTED_is_destination_selection_phase();
void EVENTED_set_hover_as_event();
void EVENTED_remove_hover_as_event();
void EVENTED_set_current_map_filename(char* name);
void EVENTED_follow_event(char** file);
void EVENTED_delete();
int EVENTED_save(char* file);

#endif // EVENT_EDITOR_H_INCLUDED
