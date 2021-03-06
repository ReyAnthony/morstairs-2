#ifndef MAP_EDITOR_H_INCLUDED
#define MAP_EDITOR_H_INCLUDED
#include "../../morstairs/map/map.h"

MAP_SubmoduleDelegation MAPED_submodule_initializer(MAP_SubmodulePackage smp);

void MAPED_change_tile_at_cursor();
void MAPED_change_player_tile(Tile tile);

void MAPED_set_as_collider(Tile tile);
void MAPED_unset_as_collider(Tile tile);

void MAPED_remove_1_frame_to_animation(Tile tile);
void MAPED_add_1_frame_to_animation(Tile tile);
int MAPED_save_all_modifications(char* map_file, char* collider_file, char* animation_file);

MAP_Point MAPED_get_player_position();
Tile MAPED_get_hover_tile();
Tile MAPED_get_player_tile();

#endif // MAP_EDITOR_H_INCLUDED
