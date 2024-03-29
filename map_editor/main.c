#include "main.h"

typedef struct message_t {
    int current_lifetime;
    char* format_string;
    void* arg;
} Message;

static int current_tile;
static int show_help = 1;
static Message most_recent_message = { .format_string = "", .arg = NULL, .current_lifetime = 0};
static void update_messages();
static void save();
static char* open_file = NULL;
static char** filters = {"*.map"};

static void load_map_dialog() {
    open_file = tinyfd_openFileDialog("Choose map file", "./", 0, filters, "map files", 0);
}

int main ( int argc, char** argv )
{
    SDL_Init( SDL_INIT_VIDEO );
    SDL_EnableKeyRepeat(100, 100);
    SDL_ShowCursor(0);
    SDL_putenv("SDL_VIDEO_WINDOW_POS=center");

    if(!TEXT_init(FONTSET, 16, 16)) {
        return EXIT_FAILURE;
    }

    load_map_dialog();
    if(open_file == NULL) {
        exit(-1);
    }

    if(!MAP_init(TILESET, open_file, COL_IN, ANIM_IN, 64,  WIDTH, HEIGHT)) {
        return EXIT_FAILURE;
    }
    MAP_extends_with_submodule(MAPED_submodule_initializer);
    MAP_extends_with_submodule(EVENTED_submodule_initializer);
    EVENTED_load_file(EV_IN);
    current_tile = MAPED_get_player_tile();

    atexit(SDL_Quit);
    SDL_Surface* screen = SDL_SetVideoMode(WIDTH, HEIGHT, 16, SDL_HWSURFACE|SDL_DOUBLEBUF);
    SDL_Rect message_pos = {.x = 0, .y = HEIGHT - 64};
    SDL_Rect help_pos = {.x = WIDTH - 400, .y = HEIGHT - 300};

    int done = 0;
    while (!done)
    {
        SDL_Event event;
        MAP_draw(screen, 5);

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                done = 1;
                break;
            case SDL_KEYDOWN:
                {
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        if(tinyfd_messageBox("Sure to quit ?", "If not saved earlier, then nothing is saved", "okcancel", "yesno", 1)) {
                            done = 1;
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_s) {
                        save();
                        most_recent_message.format_string = "Saved !";
                    }
                    else if (event.key.keysym.sym == SDLK_h) {
                        if(show_help == 0) {
                            show_help = 1;
                        }
                        else {
                            show_help = 0;
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_0) {
                        MAPED_change_player_tile(++current_tile);
                    }
                    else if (event.key.keysym.sym == SDLK_9) {
                        if(current_tile > 0) {
                            MAPED_change_player_tile(--current_tile);
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_2) {
                        MAPED_set_as_collider(current_tile);
                        most_recent_message.format_string = "Current pencil added as a collider (%d)";
                        most_recent_message.arg = current_tile;

                    }
                     else if (event.key.keysym.sym == SDLK_1) {
                        MAPED_unset_as_collider(current_tile);
                        most_recent_message.format_string = "Current pencil removed as a collider (%d)";
                        most_recent_message.arg = current_tile;
                    }
                     else if (event.key.keysym.sym == SDLK_4) {
                        MAPED_set_as_collider(MAPED_get_hover_tile());
                        most_recent_message.format_string = "Current hover added as a collider (%d)";
                        most_recent_message.arg = MAPED_get_hover_tile();
                    }
                     else if (event.key.keysym.sym == SDLK_3) {
                        MAPED_unset_as_collider(MAPED_get_hover_tile());
                        most_recent_message.format_string = "Current hover removed as a collider (%d)";
                        most_recent_message.arg = MAPED_get_hover_tile();
                    }
                    else if (event.key.keysym.sym == SDLK_5) {
                        MAPED_remove_1_frame_to_animation(current_tile);
                        most_recent_message.format_string = "Current pencil removed 1 frame to anim (%d)";
                        most_recent_message.arg = current_tile;
                    }
                     else if (event.key.keysym.sym == SDLK_6) {
                        MAPED_add_1_frame_to_animation(current_tile);
                        most_recent_message.format_string = "Current pencil added 1 frame to anim (%d)";
                        most_recent_message.arg = current_tile;
                    }
                     else if (event.key.keysym.sym == SDLK_7) {
                        MAPED_remove_1_frame_to_animation(MAPED_get_hover_tile());
                        most_recent_message.format_string = "Current hover removed 1 frame to anim (%d)";
                        most_recent_message.arg = MAPED_get_player_tile();
                    }
                     else if (event.key.keysym.sym == SDLK_8) {
                        MAPED_add_1_frame_to_animation(MAPED_get_hover_tile());
                        most_recent_message.format_string = "Current hover added 1 frame to anim (%d)";
                        most_recent_message.arg = MAPED_get_hover_tile();
                    }
                    else if (event.key.keysym.sym == SDLK_SPACE) {
                        MAPED_change_tile_at_cursor();
                    }
                    else if (event.key.keysym.sym == SDLK_c) {
                        MAPED_change_player_tile(MAPED_get_hover_tile());
                        current_tile = MAPED_get_hover_tile();
                    }
                    else if (event.key.keysym.sym == SDLK_e) {
                        if(EVENTED_is_destination_selection_phase()) {
                            most_recent_message.format_string = "Current hover added as event destination";
                        }
                        else {
                             most_recent_message.format_string = "Current hover added as event origin\nPlease select a destination";
                        }
                        EVENTED_set_hover_as_event();
                    }
                    else if (event.key.keysym.sym == SDLK_r) {
                        load_map_dialog();
                        if(open_file != NULL) {
                            MAP_load_another(open_file, MAPED_get_player_position());
                        }
                    }
                    else if (event.key.keysym.sym == SDLK_g) {
                        EVENTED_follow_event(&open_file);
                        most_recent_message.format_string = "Following event... \n(%s)";
                        most_recent_message.arg = basename(open_file);
                    }
                    else if (event.key.keysym.sym == SDLK_d) {
                        EVENTED_delete();
                        most_recent_message.format_string = "Deleted event..";
                    }
                    else if(event.key.keysym.sym == SDLK_LEFT) {
                        MAP_move(WEST);
                    }
                    else if(event.key.keysym.sym == SDLK_RIGHT) {
                         MAP_move(EAST);
                    }
                    else if(event.key.keysym.sym == SDLK_UP) {
                         MAP_move(NORTH);
                    }
                    else if(event.key.keysym.sym == SDLK_DOWN) {
                         MAP_move(SOUTH);
                    }
                    break;
                }
            }
        }

        MAP_Point p = MAPED_get_player_position();
        SDL_BlitSurface(TEXT_blit_formatted("X : %d, Y: %d", p.x, p.y), NULL, screen, NULL);
        SDL_BlitSurface(TEXT_blit_formatted(most_recent_message.format_string,
                                            most_recent_message.arg),
                        NULL, screen, &message_pos);


        //help
        if(show_help) {
            SDL_FillRect(screen, &help_pos, SDL_MapRGB(screen->format, 20, 20, 20));
            SDL_Surface* s = TEXT_blit(HELP_TXT);
            SDL_BlitSurface(s, NULL, screen, &help_pos);
        }

        SDL_Flip(screen);
        SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 20, 20, 20));
        update_messages();
    }
    MAP_quit();
    TEXT_quit();
    return 0;
}

static void update_messages() {
    if(strcmp(most_recent_message.format_string, "")) {
            most_recent_message.current_lifetime++;
        }
        else {
            most_recent_message.current_lifetime = 0;
        }

        if(most_recent_message.current_lifetime > MSG_LIFETIME) {
            most_recent_message.current_lifetime = 0;
            most_recent_message.format_string = "";
            most_recent_message.arg = NULL;
        }
}

static void save() {
    MAPED_save_all_modifications(open_file, COL_IN, ANIM_IN);
    MAPED_save_all_modifications(open_file, COL_OUT, ANIM_OUT);
    EVENTED_save(EV_IN);
}
