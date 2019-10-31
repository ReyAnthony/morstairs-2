#include "map_editor.h"

#define FAILURE 0
#define SUCCESS 1
#define TILE_MAX 65535

static MAP_SubmodulePackage smp;
static void map_draw(SDL_Rect r, SDL_Surface* screen, Tile t);

MAP_SubmoduleDelegation  MAPED_submodule_initializer(MAP_SubmodulePackage submod) {
    smp = submod;
    *smp.player_tile = 1;
    *smp.has_collisions = FAILURE;

    MAP_SubmoduleDelegation delegation =
    {
    .map_draw_delegate = map_draw,
    .player_draw_delegate = map_draw,
    .on_movement_delegate = NULL
    };

    return delegation;
}

void MAPED_change_tile_at_cursor() {
    smp.map->data[smp.player_pos->y][smp.player_pos->x] = *smp.player_tile;
}

MAP_Point MAPED_get_player_position() {
    return *smp.player_pos;
}

int MAPED_save_all_modifications(char* map_file, char* collider_file, char* animated_file){

    //Save map
    FILE* f;
    if((f = fopen(map_file, "w")) == NULL) {
        return FAILURE;
    }
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wformat"
    fprintf(f, "%u %u\n", smp.map->height, smp.map->width);
    #pragma GCC diagnostic pop
    Uint16 x, y;
    for(y = 0; y < smp.map->height; y++) {
        for(x = 0; x < smp.map->width; x++) {
            fprintf(f, "%hu ", smp.map->data[y][x]);
        }
        fprintf(f, "\n");
    }
    fclose(f);

    //Save colliders
    if((f = fopen(collider_file, "w")) == NULL) {
        return FAILURE;
    }

    int to_rem = 0;
    Uint8 i;
    for(i = 0; i < smp.colliders->count; i++) {
        if(smp.colliders->data[i] == TILE_MAX) {
            to_rem++;
        }
    }

    fprintf(f, "%hu\n", smp.colliders->count - to_rem);
    for(i = 0; i < smp.colliders->count; i++) {
        if(smp.colliders->data[i] == TILE_MAX) {
            continue;
        }
        fprintf(f, "%hu\n", smp.colliders->data[i]);
    }
    fclose(f);

    if((f = fopen(animated_file, "w")) == NULL) {
        return FAILURE;
    }

    int to_remove = 0;
     for(i = 0; i < smp.animated->count; i++) {
        if(smp.animated->data[i].frame_count <= 1) {
            to_remove++;
        }
    }

    fprintf(f, "%hu\n", smp.animated->count - to_remove);
    for(i = 0; i < smp.animated->count; i++) {
        if(smp.animated->data[i].frame_count <= 1) {
            continue;
        }
        fprintf(f, "%hu %hu\n", smp.animated->data[i].tile, smp.animated->data[i].frame_count);
    }

    fclose(f);
    return SUCCESS;
}

void MAPED_change_player_tile(Tile tile) {
    *smp.player_tile = tile;
}

void MAPED_set_as_collider(Tile tile) {

    int i;
    for(i = 0; i < smp.colliders->count; i ++) {
        if(smp.colliders->data[i] == tile) {
            return;
        }
    }

    smp.colliders->data[smp.colliders->count] = tile;
    smp.colliders->count++;
}

void MAPED_unset_as_collider(Tile tile) {

    int i;
    for(i = 0; i < smp.colliders->count; i ++) {
        if(smp.colliders->data[i] == tile) {
            //sets it at -1 then, we will filter when saving
            //TODO HACK
            smp.colliders->data[i] = TILE_MAX;
        }
    }
}

Tile MAPED_get_hover_tile() {
    MAP_Point p = *smp.player_pos;
    return smp.map->data[p.y][p.x];
}

Tile MAPED_get_player_tile() {
    return *smp.player_tile;
}

void MAPED_remove_1_frame_to_animation(Tile tile) {

    //do not save if frame_count == 1
    int i;
    for (i = 0; i < smp.animated->count; i ++) {
        Animation* a = &smp.animated->data[i];
        if(a->tile == tile) {
            if(a->frame_count > 1) {
                a->frame_count--;
            }
            return;
        }
    }
}

void MAPED_add_1_frame_to_animation(Tile tile) {

    int i;
    for (i = 0; i < smp.animated->count; i ++) {
        Animation* a = &smp.animated->data[i];
        if(a->tile == tile) {
            a->frame_count++;
            return;
        }
    }

    Animation* a = &smp.animated->data[smp.animated->count];
    a->tile = tile;
    a->frame_count = 2;
    smp.animated->count++;
}

static void map_draw(SDL_Rect r, SDL_Surface* screen, Tile t) {
    if(smp.is_blocking(t)) {
        r.w = smp.tile_size/4;
        r.h = smp.tile_size/4;
        SDL_FillRect(screen, &r, SDL_MapRGB(screen->format, 255, 0, 0));
    }
}
