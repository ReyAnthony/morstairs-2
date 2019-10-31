#include "audio.h"
#define FAILURE 0
#define SUCCESS 1

int AUDIO_init() {
#ifdef DC
    snd_stream_init();
    sndoggvorbis_init();
    return SUCCESS;
#endif
    return SUCCESS;
}

void AUDIO_play(const char* file, int loop) {
#ifdef DC
    sndoggvorbis_stop();
    sndoggvorbis_start(file, loop);
#endif
}

void AUDIO_quit() {
#ifdef DC
    sndoggvorbis_stop();
    sndoggvorbis_shutdown();
    snd_stream_shutdown();
#endif
}



