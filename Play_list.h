#ifndef __PLAY_LIST_H__
#define __PLAY_LIST_H__
#include<stdlib.h>
#include <string.h>
#include <dirent.h>
#include<time.h>
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
int len_play_list(Play_list**);
Play_list* quit_song(Play_list**,int);
void link_play_list(Play_list **l1,Play_list **l2);
int delete_play_list(Play_list**);
void random_list(Play_list**);
#endif
