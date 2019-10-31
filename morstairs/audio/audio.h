#ifndef AUDIO_H_INCLUDED
#define AUDIO_H_INCLUDED

#ifdef DC
    #include <kos.h>
    #include <oggvorbis/sndoggvorbis.h>
#endif

int AUDIO_init();
void AUDIO_play(const char* file, int loop);
void AUDIO_quit();


#endif // AUDIO_H_INCLUDED
