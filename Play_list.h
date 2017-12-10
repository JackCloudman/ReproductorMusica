#ifndef __PLAY_LIST_H__
#define __PLAY_LIST_H__
#include<stdlib.h>
#include <string.h>
#include <dirent.h>
#include"Song.h"
typedef struct play_list{
  Song* s;
  struct play_list* next;
  struct play_list* previous;
  char* title;
}Play_list;
int add_song(Play_list**,char*,char*);
int next_song(Play_list**);
int previous_song(Play_list**);
int play_music_list(Play_list**);
int add_multiple_songs(Play_list**,char*);
#endif
