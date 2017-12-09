#ifndef __PLAY_LIST_H__
#define __PLAY_LIST_H__
#include<stdlib.h>
#include"Song.h"
typedef struct play_list{
  Song* s;
  struct play_list* next;
  struct play_list* previous;
  char* path;
}Play_list;
int add_song(Play_list**,char*);
int next_song(Play_list**);
int previous_song(Play_list**);
int play_music_list(Play_list**);
#endif
