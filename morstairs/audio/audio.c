#include "audio.h"
#define FAILURE 0
#define SUCCESS 1

#ifdef PC
   static Mix_Music* music;
#endif

int AUDIO_init() {
#ifdef DC
    snd_stream_init();
    sndoggvorbis_init();
    return SUCCESS;
#endif
#ifdef PC
   if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) {
        return FAILURE;
   }
#endif
    return SUCCESS;
}

void AUDIO_play(const char* file, int loop) {
#ifdef DC
    sndoggvorbis_stop();
    sndoggvorbis_start(file, loop);
#endif
#ifdef PC
    if(music != NULL) {
        Mix_FreeMusic(music);
    }
    if(loop == 1) {
        loop = -1;
    }
    music = Mix_LoadMUS(file);
    Mix_PlayMusic(music, loop);
#endif
}

void AUDIO_quit() {
#ifdef DC
    sndoggvorbis_stop();
    sndoggvorbis_shutdown();
    snd_stream_shutdown();
#endif
#ifdef PC
   Mix_CloseAudio();
#endif
}



