#ifndef __Song_H__
#define __Song_H__
#include<vlc/vlc.h>
#include<stdlib.h>
typedef struct song{
  libvlc_instance_t *instance;
  libvlc_media_player_t *mediaPlayer;
  libvlc_media_t *media;
}Song;
Song* create_song(char* ruta);
void play_song(Song*);
void pause_song(Song*);
void stop_song(Song*);
void open_media(Song*);
void on_playpause(Song*);
#endif
